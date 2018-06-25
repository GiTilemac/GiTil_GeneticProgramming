/*
	Author: Tilemachos S. Doganis
*/

#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cstring>


//RUNNING PARAMETERS
#define PXOVER    0.2
#define PMUTATION 0.9
#define POPSIZE   300 //ONLY ODD VALUES
#define MAX_GENS  5000
#define WALL_PEN  5

#define STEP_PEN  0 //Not used
#define MAX_STEPS 15
#define MAX_DEPTH 4


//TERMINALS & FUNCTIONS
#define MV_RIGHT  0
#define MV_DOWN   1
#define MV_LEFT   2
#define MV_UP     3
#define CHK_RIGHT 4
#define CHK_DOWN  5
#define CHK_LEFT  6
#define CHK_UP    7
#define INSERT_IF 8


//PROBLEM PARAMETERS
#define FIN_X     2
#define FIN_Y     6
#define INIT_X    1
#define INIT_Y    0
#define B_X       5
#define B_Y       7

#define MAX_SCORE (40 + MAX_STEPS * MAX_DEPTH * WALL_PEN + MAX_STEPS * MAX_DEPTH * STEP_PEN)
#define frand() ((double) rand()/RAND_MAX)

using namespace std;

static int labynth[B_X][B_Y] =
{
    {1,1,1,1,1,1,1},
    {0,0,1,0,0,0,1},
    {1,0,1,0,1,0,0},
    {1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1}
};

class Node
{
    public:
        Node();
        Node(int);
        virtual ~Node();
        int value; //Values used for accessing Tree nodes
        vector<Node*> children; //contains pointers to the children of each node
        vector<Node*>::iterator citor;
        unsigned int numOfChildren;
        bool root;
        bool leaf;
        int lvl; //Node's level within the tree (root lvl = 0)
        unsigned int id;
        unsigned int cid;
        Node* father; //Pointer to the Node's father
    protected:
    private:
};

class Tree
{
    public:
        Tree();
        virtual ~Tree();
        Node *Troot;
        vector<int> path_x;
        vector<int> path_y;
        int tree_id;
        int depth;
        int runs;
        int steps_taken;
        int cur_x;
        int cur_y;
        int walls;
        int id_init;
        unsigned int tsize;
        double Fitness;
        double Selection_Prob, Cumulative_Prob;
        int Mate;
        Node *xover;
        int subTreeDepth(Node *);
        void addIF(Node *);
        void addT(int, Node *);
        void printTree(Node *);
        int execute(Node *);
        double evaluate();
        void expand(Node *inTree);
        void deleteSubTree(Node *);
        void searchNode(Node *, int);
        void initTreeId(Node *,int);
        void print_maze();
        void print_path();

    protected:
    private:
};

class Forest
{
    public:
        Forest();
        virtual ~Forest();
        Tree *Best;
        vector<Tree*> trees; //vector containing all the nodes within the Tree
        vector<Tree*> trees_new;
        int survivors[POPSIZE];
        float avg_fit;
        double RandVal(double,double);
        void copyForest();
        void selection(void);
        void crossOver();
        void mutate(Tree *);
        void clone(Node *, Node *);

    protected:
    private:
};
