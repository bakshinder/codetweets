#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <algorithm>
#include<string.h>
#include "bloom.h"
#include "background.h"
#include<time.h>
//using namespace std;
#define DEBUG 0
void setup_background();
int belongs(int a, std::vector<int> arr);
int internal( std::vector<int> past,std::vector<int> neigh);
void following();

//k is the number of hash functions
//m is the VECT_SIZE

#define TOTAL_USER 7695880

std::vector<struct background > matrix;
std::vector<struct background > incoming;
int neigh[TOTAL_USER+1], incom[TOTAL_USER+1]={0};// Storew the number of elements in N(u)
int main()
{
    
    /*******SETUP THE BACKGROUND to be used for every topic graph*/
    std::vector<int> arr;
    
    FILE *fp2;
    
     clock_t t1, t2;
    t1 = t2 = clock();

    int nod;
    //printf("%d %d %d %d\n",arr[0],arr[1],arr[2],arr[3]);
    setup_background();
    following();
    long pivot = 1395964799;

    
    printf("Background Complete\n");
    
    int window = 0;
    int diff,prev_e=0,e,day=0;

    
    for(window=0;window<30*24;window++)
    {
        FILE *fp;
        fp = fopen("a.txt","r");
        
        int user=0;
        /*******************************/
        std::vector<int> past;
        
        int a,con=0,den=0;
        
        
        int num=0,total=0;
        char b,c[1000];
        float time_taken=0.0;
        
     
        while(1)
        {
            long b_in,c_in;
            char d[100];
            fscanf(fp,"%s\t%ld\t%ld",d,&c_in,&b_in);
            
             day = window/24;
            int time   = window%24;
            c_in=c_in-pivot;
            if((c_in/(24*3600) == day && c_in%(24*3600) >= time*3600)||(c_in/(24*3600) == day+1  && c_in%(24*3600) <= time*3600))
            {
                a = b_in;
                e = c_in;
                
                user++;
                if(user%1000==0)
                    printf("%d %d %d %f\n",user,neigh[a],incom[a],con*1.0/den);
                
                if(!belongs(a,past))
                {
                    if(neigh[a]>0)
                    {
                        
                        FILE *fp;
                        std::vector<int> vect;
                    
                        int fil = a/100000;
                        char e[100];
                        sprintf(e,"%d",fil);
                        char d[100] = "../exact/";
                        strcat(d,"following/");
                        strcat(d,e);
                        fp = fopen(d,"r");
                        while(1)
                        {
                        
                            char str[50000];
                        
                            if(fgets(str,50000,fp)==NULL)
                                break;
                        //printf("%s\n",str);
                            char *a1 = strtok(str,"\t");
                            if(atoi(a1)==a)
                            {
                                int init=-1;
                                while(a1!=NULL)
                                {
                                    if(init>=0)
                                        vect.push_back(atoi(a1));
                                    
                                    init++;
                                
                                    a1 = strtok(NULL,"\t");
                                }
                            
                            }
                            else if(atoi(a1)>a)
                                break;
                        }
                        fclose(fp);
                    
                  
                    
                        
			t1=clock();                    
                        con+=neigh[a]-internal(vect,past);
			t2=clock();
			time_taken+=(t2-t1);
                        den+=neigh[a];
                    }
                
                    
                    
                    if(incom[a]>0)
                    {
                        
                        
                        FILE *fp;
                        std::vector<int> vect;
                        
                        int fil = a/100000;
                        char e[100] ;
                        sprintf(e,"%d",fil);
                        char d[100] = "../exact/";
                        strcat(d,"friends/");
                        strcat(d,e);
                        fp = fopen(d,"r");
                        //printf("%s %d\n",d,a);
                        while(1)
                        {
                            
                            char str[50000];
                            
                            if(fgets(str,50000,fp)==NULL)
                                break;
                            //printf("%s\n",str);
                            char *a1 = strtok(str,"\t");
                            if(atoi(a1)==a)
                            {
                                int init=-1;
                                while(a1!=NULL)
                                {
                                    if(init>=0)
                                        vect.push_back(atoi(a1));
                                    
                                    init++;
                                    
                                    a1 = strtok(NULL,"\t");
                                }
                                
                            }
                            else if(atoi(a1)>a)
                                break;
                        }
                        fclose(fp);
                        
			t1=clock();
                        con-=internal(vect,past);
                        t2=clock();
                        time_taken+=(t2-t1);
                    }
                    
                    
                    
                    
                }
                past.push_back(a);
                // printf("%d %d %d %f\n",a,con,den,con*1.0/den);
                
                prev_e=e;
                
            }
            
            
            if(b_in==-1)
            {
       		fp2 = fopen("output.txt","a");
	         if(den==0)
                {
                    printf("%d \t %d \t %d \t1\t0\n",a,user,day);
                    fprintf(fp2,"%d \t %d \t %d \t 1\t0\n",a,user,day);

                }
                else
                {
                    printf("%d \t %d \t %f\t%f\n",user,day,con*1.0/den,time_taken);
                    fprintf(fp2,"%d \t %d \t %d \t %f\t%f\n",a,user,day,con*1.0/den,time_taken/CLOCKS_PER_SEC * 1000);
                }
		fclose(fp2);
                break;
                
            }
            
            
        }
        
        fclose(fp);
        
    }
    
    
    
//    printf("TOTAL EDGES %d\n",total);
    return 0;
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

int internal( std::vector<int> past,std::vector<int> neigh)
{
    int num=0;
    int i;

    for(i=0;i<neigh.size();i++)
    {
        if(past.end() != std::find(past.begin(),past.end(),neigh[i]))
            num++;
        
    }
    return num;
}
int belongs(int a, std::vector<int> arr)
{
    return arr.end() != std::find(arr.begin(), arr.end(), a);
}
