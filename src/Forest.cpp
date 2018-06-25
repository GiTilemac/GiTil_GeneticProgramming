/*
	Author: Tilemachos S. Doganis
*/

#include "headers.h"
Forest::Forest()
{
    trees.clear();
}

Forest::~Forest()
{
}

/* Genetic Algorithm for Forest Evolution based on:

  A Simple Genetic Algorithm (SGA)
  created by Stratos Georgopoulos,
  based on the algorithm presented in the book:
  Michalewicz, Z., "Genetic Algorithms + Data Structures = Evolution Programs",
  Springer-Verlag, 3rd edition, 1996.
  
*/
  
void Forest::selection()
{
  	int    member, spin_num, mem; /* Some counter variables       */
  	double Total_Fitness;       /* The total population fitness */
  	double roulette=0;
  	/* a variable for temporary storing of the population */
  	int Found;                    /* A flag */

  	/* First, find the total fitness of the population    */
  	Total_Fitness=0;
  	for (member=0; member<POPSIZE; member++)
    {
      	Total_Fitness += trees[member]->Fitness;
      	survivors[member]=-1;

    }
  	/* Next, calculate the probability of a selection of each genotype      */
  	for(member=0; member<POPSIZE; member++)
    	trees[member]->Selection_Prob = trees[member]->Fitness/Total_Fitness;

	/* Now, calculate the cumulative probability of each genotype     */
  	trees[0]->Cumulative_Prob = trees[0]->Selection_Prob;
  	for(member=1; member<POPSIZE; member++)
    	trees[member]->Cumulative_Prob = trees[member-1]->Cumulative_Prob + trees[member]->Selection_Prob;
  	/* Finally, select the survivors using the cumulative probability */
  	/* and create the new Population                                  */
  	for(spin_num=0; spin_num<POPSIZE; spin_num++)
    {
    	Found = false;
    	roulette = (double) rand()/RAND_MAX;

    	if(roulette <= trees[0]->Cumulative_Prob)
            survivors[spin_num] = 0;

        else if(roulette > trees[POPSIZE-1]->Cumulative_Prob) /* select the best member of the population */
            survivors[spin_num] = POPSIZE-1;

        else
        	for(mem=1; mem<POPSIZE && Found==false; mem++)
           		if( (roulette > trees[mem-1]->Cumulative_Prob) && (roulette <= trees[mem]->Cumulative_Prob) )
           		{
                    survivors[spin_num] = mem;
              		Found = true;
           		}
	}

}

// Clone Tree
void Forest::clone(Node *inTree, Node* cur_node)
{
    unsigned int i;
    Node *N;

    inTree->value = cur_node->value;
    inTree->numOfChildren = cur_node->numOfChildren;
    inTree->root = cur_node->root;
    inTree->leaf = cur_node->leaf;
    inTree->lvl = cur_node->lvl;
    inTree->id = cur_node->id;
    inTree->cid = cur_node->cid;

    inTree->children.clear();
    for (i=0; i<inTree->numOfChildren; i++)
    {
        N = new Node();
        inTree->children.push_back(N);
        N->father = inTree;

        //Clone children
        clone(inTree->children[i],cur_node->children[i]);
    }
}

// Cross over two random subtrees of two trees
void Forest::crossOver()
{
    vector<Node*>::iterator citor;
    unsigned int i;
    Tree *par1 = NULL;
    Tree *par2 = NULL;

    int mem,
    cnt=0,
    lovers=0,
    dep1=0,
    dep2=0,
    indiv=0;

  	for(mem=0; mem<POPSIZE; mem++)
    	trees[mem]->Mate=false;

  	/* Find Parents */
  	for (mem=0;mem<POPSIZE;mem++)
    	if (frand() < PXOVER)
        {
       		trees[mem]->Mate = true;
       		lovers++;
    	}

	/* Even out number of Parents*/
  	if((lovers%2) != 0)
    {
        /* add a non parent */
    	do
    	    indiv = rand()%POPSIZE;
    	while (trees[indiv]->Mate == true);
  		trees[indiv]->Mate=true;
  		lovers++;
	}

  	/* second mate the "lovers" */
  	mem=0;
  	for(cnt=0; cnt<(lovers/2); cnt++)
    {
        par1 = NULL;
        par2 = NULL;
    	while ((trees[mem]->Mate==false) && (mem<POPSIZE-1)) mem++; /* find the first parent */
    	par1=trees[mem];
		mem++;
    	while ((trees[mem]->Mate==false) && (mem<POPSIZE-1)) mem++; /* find the second parent */
    	par2=trees[mem];
		mem++;

        //Select valid Crossover points
		do
        {

            int xoverpoint1 = (rand()%(par1->tsize-1))+1;
            int xoverpoint2 = (rand()%(par2->tsize-1))+1;

            //Find the Nodes of Crossover points
            par1->searchNode(par1->Troot, xoverpoint1);
            par2->searchNode(par2->Troot, xoverpoint2);

            dep1 = par1->subTreeDepth(par1->xover);
            dep2 = par2->subTreeDepth(par2->xover);
        }
        while (((par1->xover->lvl + dep2 - 1) > MAX_DEPTH) || ((par2->xover->lvl + dep1 - 1) > MAX_DEPTH)); //MAX_DEPTH must not be overpassed

        //f1,f2 father nodes of crossover nodes
        Node *f1 = par1->xover->father;
        Node *f2 = par2->xover->father;

        //Crossover Operations
        par1->xover->father = f2; //New father for T1 subtree
        citor = f1->children.begin();
        f1->children.erase(citor+par1->xover->cid); //Remove T1 subtree from f1 children vector
        f1->children.insert(citor+par1->xover->cid,par2->xover); //Insert T2 subtree to f1 children

        par2->xover->father = f1; //New father for T2 subtree
        citor = f2->children.begin();
        f2->children.erase(citor+par2->xover->cid); //Remove T2 subtree from f2 children vector
        f2->children.insert(citor+par2->xover->cid,par1->xover); //Insert T1 subtree to f2 children

        //Restore ID variables to default values
        for(i=0; i<f1->children.size();i++)
            f1->children[i]->cid=i;

        for(i=0; i<f2->children.size();i++)
            f2->children[i]->cid=i;

        //Initializations
        par1->id_init=0;
        par2->id_init=0;
        par1->initTreeId(par1->Troot,1);
        par2->initTreeId(par2->Troot,1);
        par1->tsize = par1->id_init;
        par2->tsize = par2->id_init;
        par1->depth = par1->subTreeDepth(par1->Troot);
        par2->depth = par2->subTreeDepth(par2->Troot);

        //Chance for mutation in offspring
        if(frand()<PMUTATION)
            mutate(par1);
        if(frand()<PMUTATION)
            mutate(par2);
  	}

  	/* set Mate flag to False */
	for(mem=0;mem<POPSIZE;mem++)
    	trees[mem]->Mate=false;
}

// Mutate target Tree at random depth
void Forest::mutate(Tree *T)
{
    unsigned int i;
    int point_m;
    if (T->tsize>1)
    {
        //Find mutation node in tree
        point_m = (rand()%(T->tsize-1))+1;
        T->searchNode(T->Troot, point_m);

        //Clear mutation node's children
        for ( i=0; i<T->xover->numOfChildren; i++)
            T->deleteSubTree(T->xover->children[i]);
        T->xover->numOfChildren=0;

        T->expand(T->xover);

        //Restore ID variables to default values
        T->id_init=0;
        T->initTreeId(T->Troot,1);
        T->tsize = T->id_init;
        T->depth = T->subTreeDepth(T->Troot);
    }
}

// Copy current Forest to new vector and clear previous Forest
void Forest::copyForest()
{
    unsigned int i;

    //Add survivors to trees_new vector
    for (i=0; i<POPSIZE;i++)
    {
        trees_new.push_back(new Tree());
        trees_new[i]->tree_id = i;
        trees_new[i]->tsize = trees[survivors[i]]->tsize;
        trees_new[i]->depth = trees[survivors[i]]->depth;

        clone(trees_new[i]->Troot,trees[survivors[i]]->Troot);
    }

    //Swap vectors
    trees.swap(trees_new);

    //Clear memory
    for ( unsigned int i = 0; i < trees_new.size(); i++ )
    {
        trees_new[i]->deleteSubTree(trees_new[i]->Troot);
        delete trees_new[i];
    }
    trees_new.clear();

}
