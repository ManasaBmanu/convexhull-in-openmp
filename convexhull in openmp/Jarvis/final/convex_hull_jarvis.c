#include<omp.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
 
 
typedef struct node
{
       int x;
       int y;
}node;

long long int n;
int tid;
node points[10000];
double x,y,start1,end1,start2,end2;      

int i,chunk=3;
int  p = 0,l;
  
double crossproduct(node p, node q,node r)
{
    int x1,x2,y1,y2;
    x1=p.x-q.x;
    x2=p.x-r.x;
    y1=p.y-q.y;
    y2=p.y-r.y;
    double val = y2*x1-y1*x2;
    if (val == 0)
       return 0;
    if(val<0)
       return 1;
    else
       return 2;
}
void jarvis_seq()
{
    
       for(i=1;i<n;i++)
       {
        
              if(points[p].x > points[i].x)
              {
                     p = i;
              }
              else if(points[p].x==points[i].x && points[p].y > points[i].y)
              {
                     p = i;
              }
       }
       l =p;
       int  q = (p+1)%n;
 
       node result[10001];
       int  count =0;
       if(n<3)
       {
            
              for(i=0;i<n-1;i++)
              {
                     //printf("%lf %lf\n",points[i].x,points[i].y);
                     result[count]=points[i];
                     count++;
              }
       }
       else
       {
            do
            {
                //printf("%lf %lf\n",points[p].x,points[p].y);
                result[count]=points[p];
                count++;
                for(i=0;i<n;i++)
                {
                    if(crossproduct(points[p],points[i],points[q])==2)
                    {
                        q = i;
                    }
                }
                p = q;
                q = (p+1) % n;
            }while(p!=l);
       }
       printf("\ncoordinates chosen by sequential execution:\n");
       for(int i=0;i<count;i++)
       {
        printf("(%d,%d)",result[i].x,result[i].y);
       }

}

void jarvis_para()
{
    #pragma omp parallel for schedule(static,chunk) private(tid,i) shared(chunk,n,points) num_threads(8)
       for(i=1;i<n;i++)
       {
            //tid=omp_get_thread_num();
            //printf("\nthread %d running\n",tid);
              if(points[p].x > points[i].x)
              {
                     p = i;
              }
              else if(points[p].x==points[i].x && points[p].y > points[i].y)
              {
                     p = i;
              }
       }
       l =p;
       int  q = (p+1)%n;
 
       node result[10001];
       int  count =0;
       if(n<3)
       {
            #pragma omp parallel for schedule(static,chunk) 
              for(i=0;i<n-1;i++)
              {
                     //printf("%lf %lf\n",points[i].x,points[i].y);
                     result[count]=points[i];
                     count++;
              }
       }
       else
       {
            do
            {
                //printf("%lf %lf\n",points[p].x,points[p].y);
                result[count]=points[p];
                count++;

                for(i=0;i<n;i++)
                {
                    if(crossproduct(points[p],points[i],points[q])==2)
                    {
                        q = i;
                    }
                }
                p = q;
                q = (p+1) % n;
            }while(p!=l);
       }
       printf("\ncoordinates chosen by parallel execution:\n");
       for(int i=0;i<count;i++)
       {
        printf("(%d,%d)",result[i].x,result[i].y);
       }

} 
int main()
{
    printf("enter no. of points to be accepted\n");
    scanf("%lld",&n);
    //user gives input:
    /*printf("\nenter %d points\n",n);
    for(i=0;i<n;i++)
    {
        scanf("%lf",&x);
        scanf("%lf",&y);
        points[i].x = x;
        points[i].y = y;
    }*/
       /*points[]={{0,3},{2,2},{1,1},{2,1},{3,0},{0,0},{3,3}};
        0.000000 0.000000
        3.000000 0.000000
        3.000000 3.000000
        0.000000 3.000000*/
    //random points generated:
    printf("\npoints generated:\n");
    for(i=0;i<n;i++)
    {
        points[i].x=rand()%100;
        points[i].x=rand()%100;
        printf("(%d,%d)\t",points[i].x,points[i].y );
    }
    

    start1=omp_get_wtime();
        jarvis_seq();
    end1=omp_get_wtime();
    
    
    start2=omp_get_wtime();
        jarvis_para();
    end2=omp_get_wtime();
    printf("\ntime required for sequential execution = %lf\n",end1-start1);
    printf("\ntime required for parallel execution = %lf\n",end2-start2);
    return 0;
}
 
