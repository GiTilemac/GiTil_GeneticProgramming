/*
	Author: Tilemachos S. Doganis
*/

#include "headers.h"

using namespace std;

int main()
{
    cout << "PARAMETERS "<<endl;
    cout << "Crossover Probability: " << PXOVER << endl;
    cout << "Mutation Probability: " << PMUTATION << endl;
    cout << "Population Size: "<< POPSIZE << endl;
    cout << "Max Generations: " << MAX_GENS << endl;
    cout << "Max Tree Depth: "<< MAX_DEPTH << endl;
    cout << "Wall Penalty: "<< WALL_PEN << endl;

    Forest *F = new Forest();
    Tree *T = NULL;
    srand(time(NULL));
    unsigned int i,j;
    Tree *B = NULL;


    //Create Initial Population
    for (i=0; i<POPSIZE; i++)
    {
        //Create Tree
        F->trees.push_back(new Tree());
        T = F->trees.back();
        T->tree_id = F->trees.size()-1;

        //Expand Tree
        T->expand(T->Troot);

        //Initialize Variables
        T->id_init=0;
        T->initTreeId(T->Troot,1);
        T->depth = T->subTreeDepth(T->Troot);

        //Evaluate
        T->Fitness = T->evaluate();
    }

    B = T; // Current Best Tree
    j = 0; // Generation index
    while ((j < MAX_GENS ) && (B->Fitness < 100))
    {
        //Select survivors
        F->selection();

        //Copy them to new forest
        F->copyForest();

        //CrossOver and mutate offspring
        F->crossOver();

        //Re-Evaluation & finding Best Tree
        F->avg_fit = 0;
        B = F->trees[0];
        for (i=0; i<POPSIZE; i++)
        {
            F->trees[i]->Fitness = F->trees[i]->evaluate(); //Evaluate trees
            F->avg_fit = F->avg_fit + F->trees[i]->Fitness; //Fitness sum

            //Check for best tree
            if (F->trees[i]->Fitness > B->Fitness)
                B = F->trees[i];
        }
        F->avg_fit = floor(F->avg_fit/POPSIZE);
        j++;
    }

    //Print best Member and position in maze
    cout << "Best Program:" << endl << endl;
    B->printTree(B->Troot);
    B->print_maze();
    B->print_path();
    cout << "Best Fitness:" << B->Fitness << "%\n";
    cout << "Average Fitness:" << F->avg_fit << "%\n";
    cout << "Generation: " << j << " Steps:" << B->steps_taken << " Walls:" << B->walls << endl;
    cout << "Manhattan distance from exit: " << abs(B->cur_x - FIN_X) + abs(B->cur_y - FIN_Y) << endl;
	cout << endl;
	cout << "Press Enter to close console" << endl;
	
	cin.get();

    return 0;
}
