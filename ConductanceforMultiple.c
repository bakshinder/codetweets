#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "bloom.h"
#include<time.h>
#define DEBUG 0
#define NUM_FILTER 1
#define MAX_FILTERS 50
void setup_background();
void following();
void get_bloom(int a, struct bloom *incoming_a, int whi);

//k is the number of hash functions
//m is the VECT_SIZE

#define TOTAL_USER 7695880

//Background follower - followed graph
int neigh[TOTAL_USER+1], incom[TOTAL_USER+1]={0};// Storew the number of elements in N(u)
//struct bloom *incoming[TOTAL_USER+1],*outgoing[TOTAL_USER+1];

int main()
{
    /*******SETUP THE BACKGROUND to be used for every topic graph*/
    FILE *fp1;
    clock_t t1, t2;
    t1 = t2 = clock();
//    float time_taken=0.0;
    following();
    long pivot = 1395964799;
    
    printf("Background Complete\n");
    setup_background();
    
    int day=0;
    
    if(DEBUG)
    {
        int i=0;
        for(i=0;i<TOTAL_USER;i++)
        {
            printf("%d %d\n",i,neigh[i]);
        }
    }
    
    printf("Background Complete\n");
    int diff,prev_e=0,e;
    
    int window = 0;
    for(window = 0;window<30*24;window++)
    {
        FILE *fp;
        fp = fopen("a.txt","r");
        
        int user=0;
        /*******************************/
        struct bloom *filter[MAX_FILTERS];
        
        int a,con=0,den=0,er=0;
	for(er=0;er<MAX_FILTERS;er++)
        {
		filter[er] = (struct bloom *)malloc(sizeof(struct bloom));// Bloom filter corresponding to the topic graph
		init(filter[er]);
	}
	float time_taken=0.0;  
      
        char name[MAX_FILTERS][100];
        
        int num=0,counter=1;
        char b,c[1000];
       long long degree=0; 
        int present=0;
        while(1)
        {
            long b_in,c_in;
            char d[100];
            fscanf(fp,"%s\t%ld\t%ld",d,&c_in,&b_in);
            
            int day = window/24;
            int time   = window%24;
            c_in = c_in-pivot;
	
	for(present=0;present<counter;present++)
	{
		if(strcmp(d,name[present])==0)
		{
			break;
		}
	}
	if(present==counter)
	{
		if(counter==MAX_FILTERS)
		{
			present=rand()%50;
			init(filter[present]);
			counter--;
		}
		strcpy(name[present],d);
		counter++;
	}


            if((c_in/(24*3600) == day && c_in%(24*3600) >= time*3600)||(c_in/(24*3600) == day+1  && c_in%(24*3600) <= time*3600))
            {
                
                
                a = b_in;
                e = c_in;
                
                user++;

                //if a is in filter
                if(is_in(a,filter[present]))// User has already occured
                    ;
                else
                {
                    //                   printf("ccccccaaaaaa\n");
                    num++;
		t1 =clock();
                    insert(a,filter[present]);
		t2=clock();
		time_taken+= (t2-t1);
                    if(a>TOTAL_USER)
                    {
                        ;
                    }
                    else if(num_bloom(a)>0)
                    {
                        if(num%1000==0)
                        printf(" a is %d %d %f\n",a,num,con*1.0/den);
                        // printf("aaaaaaaa\n");
                        struct bloom *outgoing_a;
                        outgoing_a=(struct bloom *)malloc(sizeof(struct bloom));
                        init(outgoing_a);
                        
                        get_bloom(a,outgoing_a,0);//0 for following and 1 for friends i.e. incoming
                    degree+=num_bloom(a);
			   t1=clock();
                        con+= num_bloom(a) - intersection_app(filter[present],a,num, outgoing_a);
			t2=clock();
			time_taken+=(t2-t1);
                        free(outgoing_a);
                        den+= num_bloom(a);//den = denominator
                        // printf("%d \t %d \t %d \t %f\n",a,num_bloom(a),day,con*1.0/den);
                        if(a==4397106)
                            printf("e%d \n",num_bloom(a));
                    }
                    if(a>TOTAL_USER)
                        ;
                    else if(incom[a]>0)
                    {
                        
                        struct bloom *incoming_a;
                        incoming_a=(struct bloom *)malloc(sizeof(struct bloom));
                        init(incoming_a);
                        get_bloom(a,incoming_a,1);
                       t1=clock();
                        con -= intersection_app(filter[present],a,num,incoming_a);//con = numerator
			t2=clock();
			time_taken+=(t2-t1);
                        free(incoming_a);
                        
                    }
                }
                
                prev_e=e;
                //printf("a is %d %d %d\n",a,neigh[a],incom[a]);
            }
            //printf("a is %d %d %d\n",a,neigh[a],incom[a]);

            if(b_in==-1)
            {
                fp1=fopen("ff10.txt","a");

                if(den!=0)
                {
                    printf(" %d \t %d \t %f\t %f\t%llu\n",num,day,con*1.0/den,time_taken/CLOCKS_PER_SEC * 1000,degree);
			//exit(0);
                    fprintf(fp1,"%d \t %d \t %d \t %f\n",a,num,day,con*1.0/den);
                }
                else
                {
                    printf("%d \t %d \t %d \t 1\n",a,user,day);
                    fprintf(fp1,"%d \t %d \t %d \t 1\n",a,user,day);
                }
                fclose(fp1);
                break;
                
            }
            
        }
        fclose(fp);
        
    }
    
    
    
    return 0;
}
int num_bloom (int a)
{
    
    return neigh[a];
}

int intersection_app (struct bloom *filter, int a, int num, struct bloom *f1)
{
    double dot = (double)dot_product(filter, f1);
    int z12 = VECT_SIZE - dot,z1 = num_zero(filter),z2 = num_zero(f1);
    double app = (z1+z2-z12)*1.0/(z1*z2);
    
    app=app*VECT_SIZE;
    app=log(app)/log(1-1.0/VECT_SIZE);
    app= -1* app*1.0/K;
    if(a==3644449)
        printf("f is %f\n",app);
    if(app<0)
    {
        return 0;
    }
    int ret = app;
    if( ret<0)
        ret = 0;
        
    return ret;
}
void setup_background()
{
    FILE *fp,*fp1;
    //fp = fopen("followers_cl.txt","r");// File containing background graph
    fp1 = fopen("../len40/following/neigh.txt","r");
    
    
    int a;
    int b;
    while(1)
    {
        if(fscanf(fp1,"%d\t%d",&a,&b)==-1)
            break;
        neigh[a] = b;
        
        
    }
    fclose(fp1);
    
}
void following()
{
    FILE *fp,*fp1;
    //fp = fopen("friends_cl.txt","r");// File containing background graph
    fp1 = fopen("../len40/friends/incom.txt","r");
    int a;
    int b;
    while(1)
    {
        if(fscanf(fp1,"%d\t%d",&a,&b)==-1)
            break;
        incom[a] = b;
        
        
    }
    
    fclose(fp1);
}

void get_bloom(int a, struct bloom *incoming_a, int whi)
{
    
    
    FILE *fp;
    
    int fil = a/100000;
    char e[100] ;
    sprintf(e,"%d",fil);
    char d[100] = "../len40/";
    if(whi==1)
    {
        strcat(d,"friends/");
    }
    else
        strcat(d,"following/");
    strcat(d,e);
    fp = fopen(d,"r");
    //printf("%s %d\n",d,a);
    while(1)
    {
        
        char str[20000];
        
        if(fgets(str,20000,fp)==NULL)
            break;
        char *a1 = strtok(str,"\t");
        //printf("atoi %s %d\n",a1,fil);
        if(atoi(a1)==a)
        {
            
            
            int init=-1;
            while(a1!=NULL)
            {
                if(init>=0 && init<VECT_SIZE/NUM_BITS+1 )
                    incoming_a->bloom_vector[init] = atoi(a1);
                
                init++;
                
                a1 = strtok(NULL,"\t");
            }
            break;
            
        }
        else if(atoi(a1)>a)
            break;
    }
    fclose(fp);
    
}
