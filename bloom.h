#ifndef BLOOM_H
#define BLOOM_H
#define NUM_BITS 28

#define VECT_SIZE 40000
struct bloom{
       
       int bloom_vector[(VECT_SIZE)/NUM_BITS + 1];
       };
#define K 3
void init(struct bloom *bl);
void insert(int val, struct bloom* bl);
int is_in(int val, struct bloom* bl);
int hash1(int a);
int hash2(int a);
int hash3(int a);
int intersect_bloom(struct bloom *a, struct bloom *b);
struct bloom* union_bloom(struct bloom *a, struct bloom *b);
int dot_product(struct bloom *a, struct bloom *b);
int num_zero(struct bloom *a);
#endif
