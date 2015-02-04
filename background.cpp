#include<stdio.h>
#include "background.h"

struct background * insert_neighbour(int a, struct background *node)
{
    node->neigh.push_back(a);
    return node;
    
}
