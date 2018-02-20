// C++ program to implement Quick Hull algorithm
// to find convex hull.
#include<bits/stdc++.h>
#include<omp.h>
#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
using namespace std;
 
// iPair is integer pairs
#define iPair pair<int, int>
 
// Stores the result (points of convex hull)
set<iPair> hull;
 
// Returns the side of point p with respect to line
// joining points p1 and p2.
int findSide(iPair p1, iPair p2, iPair p)
{
    int val = (p.second - p1.second) * (p2.first - p1.first) -
              (p2.second - p1.second) * (p.first - p1.first);
 
    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}
 
// Returns the square of distance between
// p1 and p2.
int dist(iPair p, iPair q)
{
    return (p.second - q.second) * (p.second - q.second) +
           (p.first - q.first) * (p.first - q.first);
}
 
// returns a value proportional to the distance
// between the point p and the line joining the
// points p1 and p2
int lineDist(iPair p1, iPair p2, iPair p)
{
    return abs ((p.second - p1.second) * (p2.first - p1.first) -
               (p2.second - p1.second) * (p.first - p1.first));
}
 
// End points of line L are p1 and p2.  side can have value
// 1 or -1 specifying each of the parts made by the line L
void quickHull(iPair a[], int n, iPair p1, iPair p2, int side)
{
    int ind = -1;
    int max_dist = 0;
 
    // finding the point with maximum distance
    // from L and also on the specified side of L.
    for (int i=0; i<n; i++)
    {
        int temp = lineDist(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }
 
    // If no point is found, add the end points
    // of L to the convex hull.
    if (ind == -1)
    {
        hull.insert(p1);
        hull.insert(p2);
        return;
    }
 
    // Recur for the two parts divided by a[ind]
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}
 
void printHull(iPair a[], int n)
{
    // a[i].second -> y-coordinate of the ith point
    if (n < 3)
    {
        cout << "Convex hull not possible\n";
        return;
    }
 
    // Finding the point with minimum and
    // maximum x-coordinate
    int min_x = 0, max_x = 0;
    for (int i=1; i<n; i++)
    {
        if (a[i].first < a[min_x].first)
            min_x = i;
        if (a[i].first > a[max_x].first)
            max_x = i;
    }
 
    // Recursively find convex hull points on
    // one side of line joining a[min_x] and
    // a[max_x]
    quickHull(a, n, a[min_x], a[max_x], 1);
 
    // Recursively find convex hull points on
    // other side of line joining a[min_x] and
    // a[max_x]
    quickHull(a, n, a[min_x], a[max_x], -1);
 
    cout << "The points in Convex Hull are:\n";
    while (!hull.empty())
    {
        cout << "(" <<( *hull.begin()).first << ", "
             << (*hull.begin()).second << ") ";
        hull.erase(hull.begin());
    }
}
 
// Driver code
int main()
{
    
    iPair a[] = {{83,86},{77,15},{93,35},{86,92},{49,21},{62,27},{90,59},{63,26},{40,26},{72,36},{11,68},{67,29},{82,30},{62,23},{67,35},{29,2},{22,58},{69,67},{93,56},{11,42},{29,73},{21,19},{84,37},{98,24},{15,70},{13,26},{91,80},{56,73},{62,70},{96,81},{5,25},{84,27},{36,5},{46,29},{13,57},{24,95},{82,45},{14,67},{34,64},{43,50},{87,8},{76,78},{88,84},{3,51},{54,99},{32,60},{76,68},{39,12},{26,86},{94,39},{95,70},{34,78},{67,1},{97,2},{17,92},{52,56},{1,80},{86,41},{65,89},{44,19},{40,29},{31,17},{97,71},{81,75},{9,27},{67,56},{97,53},{86,65},{6,83},{19,24},{28,71},{32,29},{3,19},{70,68},{8,15},{40,49},{96,23},{18,45},{46,51},{21,55},{79,88},{64,28},{41,50},{93,0},{34,64},{24,14},{87,56},{43,91},{27,65},{59,36},{32,51},{37,28},{75,7},{74,21},{58,95},{29,37},{35,93},{18,28},{43,11},{28,29}};
    int n = sizeof(a)/sizeof(a[0]);
    double x = omp_get_wtime();
    printHull(a, n);
    double time=omp_get_wtime()-x;
  printf("%lf\n",time);
    return 0;
}
