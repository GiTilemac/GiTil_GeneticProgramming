/*
	Author: Tilemachos S. Doganis
*/

#include "headers.h"

Tree::Tree()
{
    cur_x = INIT_X;
    cur_y = INIT_Y;
    tsize = 1;
    depth = 1;
    path_x.push_back(INIT_X);
    path_y.push_back(INIT_Y);

    //Create new tree root
    Node* A = new Node();
    A->id = 0;
    A->root=true;
    A->lvl=1;
    A->cid = 5;
    Troot = A;

}

Tree::~Tree()
{

}

// Calculate Subtree Depth
int Tree::subTreeDepth(Node *inTree)
{
    int max_dep;
    int temp;

    if (inTree->numOfChildren > 0)
    {
        //Find maximum depth subtree
        max_dep = 0;
        for (unsigned int i=0; i<inTree->numOfChildren; i++)
        {
            temp = subTreeDepth(inTree->children[i]);
            if ( temp >= max_dep)
                max_dep = temp;
        }
    }
    else max_dep = 0;
    return max_dep + 1;
}

// Delete a Subtree starting from given Node
void Tree::deleteSubTree(Node* inTree)
{

	for (unsigned int i=0; i<inTree->numOfChildren; i++) //free any children first
		deleteSubTree(inTree->children[i]);
	delete inTree;
}

// Add a Terminal Value to given Node
void Tree::addT(int T_num, Node* inTree)
{
    inTree->value = T_num;
    inTree->leaf = true;
}

// Add IF Node with 3 children: 1. Condition Check, 2. If True, 3. If False
void Tree::addIF(Node* inTree)
{
    unsigned int l;
    Node *N = NULL;
	
	//Make changes to current node
    inTree->value = INSERT_IF;
    inTree->numOfChildren = 3;
    inTree->children.clear();
	
    for (l=0; l<3; l++)
    {
        //Create child node
        N = new Node();
        inTree->children.push_back(N);
        N->father = inTree;
        N->lvl = inTree->lvl+1;
        N->id = inTree->id+1+l;
        N->cid=l;

        //Expand child subtree
        expand(N);
    }
    //Increase tree depth if at last lvl
    if (inTree->lvl == depth)
        depth++;
    tsize = tsize+3;
}

// Expand Node by creating new nodes beneath it
void Tree::expand(Node* inTree)
{
    int new_val;

    //If at max depth, generate only terminal values
    if (inTree->lvl >= MAX_DEPTH)
        new_val = rand()%8;
    else if (inTree->root == true) //If root, generate IF
        new_val = INSERT_IF;
    else
        new_val = rand()%9;

    switch (new_val)
    {
        case MV_RIGHT:
            addT(MV_RIGHT,inTree);
            break;
        case MV_DOWN:
            addT(MV_DOWN,inTree);
            break;
        case MV_LEFT:
            addT(MV_LEFT,inTree);
            break;
        case MV_UP:
            addT(MV_UP,inTree);
            break;
        case CHK_RIGHT:
            addT(CHK_RIGHT,inTree);
            break;
        case CHK_DOWN:
            addT(CHK_DOWN,inTree);
            break;
        case CHK_LEFT:
            addT(CHK_LEFT,inTree);
            break;
        case CHK_UP:
            addT(CHK_UP,inTree);
            break;
        case INSERT_IF:
            addIF(inTree);
            break;
        default:
            break;
    }
}

//Robot executes program (Current node first, then child nodes)
int Tree::execute(Node* n)
{
    int arg = n->value;
    switch (arg)
    {
        case MV_RIGHT:
            if (cur_y+1 < B_Y)
            {
                steps_taken++;
                cur_y++;
                path_x.push_back(cur_x);
                path_y.push_back(cur_y);
                if (labynth[cur_x][cur_y] == 1)
                    walls++;
            }

            else
                walls++;
            return 1;
            break;

        case MV_DOWN:
            if (cur_x+1 < B_X)
            {
                cur_x++;
                steps_taken++;
                path_x.push_back(cur_x);
                path_y.push_back(cur_y);
                if (labynth[cur_x][cur_y] == 1)
                    walls++;
            }
            else
                walls++;
            return 1;
            break;

        case MV_LEFT:
            if (cur_y-1 >= 0)
            {
                cur_y--;
                steps_taken++;
                path_x.push_back(cur_x);
                path_y.push_back(cur_y);
                if (labynth[cur_x][cur_y] == 1)
                    walls++;
            }
            else
                walls++;
            return 1;
            break;

        case MV_UP:
            if (cur_x-1 >= 0)
            {
                cur_x--;
                steps_taken++;
                path_x.push_back(cur_x);
                path_y.push_back(cur_y);
                if (labynth[cur_x][cur_y] == 1)
                    walls++;
            }
            else
                walls++;
            return 1;
            break;

        case CHK_RIGHT:
            if (cur_y+1 < B_Y)
            {
                if (labynth[cur_x][cur_y+1] == 1)
                    return 0;
                else
                    return 1;
            }
            else
                return 0;
            break;

        case CHK_DOWN:
            if (cur_x+1 < B_X)
            {
                if (labynth[cur_x+1][cur_y] == 1)
                    return 0;
                else
                    return 1;
            }
            else
                return 0;
            break;

        case CHK_LEFT:
            if (cur_y-1 >= 0)
            {
                if (labynth[cur_x][cur_y-1] == 1)
                    return 0;
                else
                    return 1;
            }
            else
                return 0;
            break;

        case CHK_UP:
            if (cur_x-1 >= 0)
            {
                if (labynth[cur_x-1][cur_y] == 1)
                    return 0;
                else
                    return 1;
            }
            else
                return 0;
            break;

        case INSERT_IF:

            if (execute(n->children[0]))
                return execute(n->children[1]);
            else
                return execute(n->children[2]);
            break;

        default:
            return -1;
            break;
    }
}


// Evaluate a Tree (Give program to robot and assess fitness function)
double Tree::evaluate()
{
    cur_x = INIT_X;
    cur_y = INIT_Y;
    path_x.clear();
    path_y.clear();
    path_x.push_back(INIT_X);
    path_y.push_back(INIT_Y);
    walls = 0;
    steps_taken = 0; //Not used
    runs = 0; //To avoid endless loop

	// Start recursively following program commands from root node
    do
    {
        execute(Troot);
        runs++;
    }
    while (((cur_x!=FIN_X) || (cur_y!=FIN_Y)) && (runs < MAX_STEPS));

	// Fitness function
    return floor(   pow((  MAX_SCORE  -  pow(cur_x-FIN_X,2)  -   pow(cur_y-FIN_Y,2)  -  walls*WALL_PEN   -   steps_taken*STEP_PEN   ),2)   / pow(MAX_SCORE,2)*100  );
}

// Print the maze
void Tree::print_maze()
{
    cout << "Maze:" << endl;
    for ( int x = 0; x < B_X; x ++ )
    {
        for ( int y = 0; y < B_Y; y++ )
        {
            if ((x == cur_x) && (y == cur_y))
                cout << 5 << " ";
            else
                cout << labynth[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;

}

// Print path
void Tree::print_path()
{
    unsigned int i;
    cout << "Path:";
    for (i=0; i<path_x.size(); i++)
        cout << "(" << path_x[i] << "," << path_y[i] << ")";
    cout << endl;
}

/* Print a Program Tree
	MR/CR: MOVE/CHECK RIGHT
	MD/CD: MOVE/CHECK DOWN
	ML/CL: MOVE/CHECK LEFT
	MU/CU: MOVE/CHECK UP
	IF: DECIDE NEXT MOVE BASED ON IF NODES
*/
void Tree::printTree(Node* inTree)
{
	for (int j=0; j<inTree->lvl-1; j++)
		cout << "...";
    cout << inTree->id << ":";
    switch (inTree->value)
    {
        case 0:
            printf("MR\n");
            break;
        case 1:
            printf("MD\n");
            break;
        case 2:
            printf("ML\n");
            break;
        case 3:
            printf("MU\n");
            break;
        case 4:
            printf("CR\n");
            break;
        case 5:
            printf("CD\n");
            break;
        case 6:
            printf("CL\n");
            break;
        case 7:
            printf("CU\n");
            break;
        default:
            printf("IF\n");
            break;
    }
	for (unsigned int i=0; i<inTree->numOfChildren; i++)
		printTree(inTree->children[i]);
}

// Initialize a node's subtree's attributes
void Tree::initTreeId(Node *inTree, int dep)
{
    inTree->id = id_init;
    inTree->lvl = dep;
    id_init++;
    dep++;

    for (unsigned int i=0; i<inTree->numOfChildren; i++)
        initTreeId(inTree->children[i],dep);
}

// Find crossover Node in a Subtree
void Tree::searchNode(Node *checknod, int checkid)
{
    if (checknod->id == (unsigned) checkid)
    {
        xover = checknod;
        return;
    }
    else
    {
        for (unsigned int i=0; i<checknod->numOfChildren;i++)
            searchNode(checknod->children[i],checkid);
    }
}
