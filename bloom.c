#include<stdio.h>
#include "bloom.h"
#define BLOOM_DEBUG 0

void init(struct bloom *bl)//Initialise the bloom filter
{
    int i=0;
    
    for(i=0;i<(VECT_SIZE)/NUM_BITS + 1;i++)
    {
        bl->bloom_vector[i]=0;
    }
}
void insert(int val, struct bloom* bl)// INsert the element
{
    int a = hash1(val);
    int loc = a/NUM_BITS;
    int off=a%NUM_BITS;
    int new = 1<<off;
    bl->bloom_vector[loc] = bl->bloom_vector[loc] | new;
    if(BLOOM_DEBUG)
        printf("loc %d %d %d %d\n",loc,off,new,bl->bloom_vector[0]);
    
    a = hash2(val);
    loc = a/NUM_BITS;
    off=a%NUM_BITS;
    new = 1<<off;
    bl->bloom_vector[loc] = bl->bloom_vector[loc] | new;
    if(BLOOM_DEBUG)
        printf("loc %d %d\n",loc,off);
    
    
    a = hash3(val);
    loc = a/NUM_BITS;
    off=a%NUM_BITS;
    new = 1<<off;
    bl->bloom_vector[loc] = bl->bloom_vector[loc] | new;
    if(BLOOM_DEBUG)
        printf("loc %d %d\n",loc,off);
    
}

int is_in(int val, struct bloom* bl)
{
    //Get locations from different hash functions and if all are 1 then it is present else absent
    // printf("aaaa%d\n",bl->bloom_vector[0]);
    int a = hash1(val);
    int loc = a/NUM_BITS;
    int off=a%NUM_BITS;
    int new = 1<<off;
    if(bl->bloom_vector[loc] & new)
        ;//printf("aaaa%d\n",bl->bloom_vector[loc]);
    else
        return 0;
    
    
    a = hash2(val);
    loc = a/NUM_BITS;
    off=a%NUM_BITS;
    new = 1<<off;
    if(bl->bloom_vector[loc] & new)
        ;
    else
        return 0;
    
    
    a = hash3(val);
    loc = a/NUM_BITS;
    off=a%NUM_BITS;
    new = 1<<off;
    if(bl->bloom_vector[loc] & new)
        return 1;
    else
        return 0;
    
}

int dot_product(struct bloom *a, struct bloom *b)
{
    int i,num=0;
    for(i=0;i< (VECT_SIZE/NUM_BITS) + 1;i++)
    {
        int j=0;
        int n1=a->bloom_vector[i];
        int n2 = b->bloom_vector[i];
        int iter=1;
        while(j<NUM_BITS)
        {
            if( (n1 & iter) & (n2 & iter) )
                num++;
            iter=iter<<1;
            j++;
        }
    }
    return num;
}

int num_zero(struct bloom *a)
{
    int i,num=0;
    for(i=0;i< (VECT_SIZE/NUM_BITS) + 1;i++)
    {
        int j=0;
        int n1=a->bloom_vector[i];
        int iter=1;
        while(j<NUM_BITS)
        {
            if( (n1 & iter) )
                num++;
            iter=iter<<1;
            j++;
        }
    }
    return VECT_SIZE - num;
}
int intersect_bloom(struct bloom *a, struct bloom *b)
{
    int i,num=0;
    
    for(i=0;i< (VECT_SIZE/NUM_BITS) + 1;i++)
    {
        int j=0;
        int n1=a->bloom_vector[i];
        int n2 = b->bloom_vector[i];
        int iter=1;
        while(j<NUM_BITS)
        {
            if( (n1 & iter) && (n2 & iter) )
                num++;
            iter=iter<<1;
            j++;
        }
        if(num>=K)//3 because we have 3 hash functions
            return 1;
    }
    return 0;
}
struct bloom* union_bloom(struct bloom *a, struct bloom *b)
{
    int i,num=0;
    for(i=0;i< (VECT_SIZE/NUM_BITS) + 1;i++)
    {
        a->bloom_vector[i] = a->bloom_vector[i] | b->bloom_vector[i];
        
    }
    return a;
    
    
}
//Different hash functions

int hash1(int a)
{
    
    return a%VECT_SIZE;
}

int hash2(int a)
{
    
    return (a+(a%100)*(a%89)+567)%VECT_SIZE;
}

int hash3(int a)
{
    
    return (a+(a%123)*(a%1000)+239)%VECT_SIZE;
}
