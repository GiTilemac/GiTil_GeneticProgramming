/*
	Author: Tilemachos S. Doganis
*/

#include "headers.h"
Node::Node()
{
    root = false;
    numOfChildren = 0;
}
Node::Node(int N)
{
    root = false;
    value = N;
    numOfChildren = 0;
}

Node::~Node()
{

}
