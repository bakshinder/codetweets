#ifndef BACKGROUN_H
#define BACKGROUND_H
#include<vector>
struct background{
       
    std::vector<int> neigh;
    int num,nid;
    int occured;
       };
struct background * insert_neighbour(int a, struct background *node);

#endif
