#include <iostream>
#include <stack>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <stdio.h>
using namespace std;
 
struct Point
{
    int x, y;
};
 long long int n;

Point p0;
 
Point nextelement(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}
 

int swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}
 

int distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}
 
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0;  
    return (val > 0)? 1: 2; 
}
 
int compare(const void *vp1, const void *vp2)
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;
 
   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;
 
   return (o == 2)? -1: 1;
}
 
void convexHull(Point points[], int n)
{
   
   int ymin = points[0].y, min = 0,i;
    #pragma omp parallel for schedule(dynamic,4) shared(i,min,ymin,n,points)
   for (i = 1; i < n; i++)
   {
     int y = points[i].y;
 
    
     if ((y < ymin) || (ymin == y &&
         points[i].x < points[min].x))
        ymin = points[i].y, min = i;
   }
 
   swap(points[0], points[min]);
 

   p0 = points[0];
   qsort(&points[1], n-1, sizeof(Point), compare);
  int m=1;
  #pragma omp parallel for schedule(dynamic,4) shared(i,m,n,points)
   for (int i=1; i<n; i++)
   {

       while (i < n-1 && orientation(p0, points[i],points[i+1]) == 0)
          i++;
 
       points[m] = points[i];
       m++; 
   }
 
   if (m < 3) return;


   stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);
 
  #pragma omp parallel for schedule(dynamic,4) shared(i,n,points,S)
   for (int i = 3; i < m; i++)
   {
      while (orientation(nextelement(S), S.top(), points[i]) != 2)
         S.pop();
      S.push(points[i]);
   }


   while (!S.empty())
   {
       Point p = S.top();
       cout << "(" << p.x << ", " << p.y <<")" << endl;
       S.pop();
   }
}
 
int main()
{
    int n;
    cin>>n;
    Point *points;
    points=(Point*)malloc(n* sizeof(Point));

    for(int i=0;i<n;i++)
       	{
              points[i].x=rand()%100;
              points[i].y=rand()%100;
        }  
   
   
    double start_time = omp_get_wtime();
    #pragma omp parallel num_threads(8)
    #pragma omp single
    convexHull(points, n);
    double time = omp_get_wtime() - start_time;
    printf("%lf\n",time);
    return 0;
}
