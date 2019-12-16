#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Sankoff su singolo carattere
int main() {

   const float inf = INFINITY;

   float cost[4][4] = {{0, 1, 2, 3},{3, 2, 1, 1},{5, 4, 2, 1},{1, 4, 2, 3}};

   int n = 4;

   
   float a[4] = {0, inf, inf, inf};
   float c[4] = {inf, 0, inf, inf};
   float g[4] = {inf, inf, 0, inf};
   float t[4] = {inf, inf, inf, 0};


   float *join(float node1[], float node2[], int n){
      float *joined = malloc(n);

   for(int i = 0; i<n; i++){

      float Lmin = inf;
      float Rmin = inf; 
      for(int j = 0; j<n; j++){
         if(Lmin > cost[i][j]+ node1[j])
            Lmin = cost[i][j]+ node1[j];

         if(Rmin > cost[i][j]+ node2[j])
            Rmin = cost[i][j]+ node2[j];
      }
      joined[i] = Lmin + Rmin;
   }
      return joined;
   }



   float *p = join(t, a, n);
   for(int i = 0; i<n-1; i++)
   printf("%.0f,", *(p + i));
   printf("%.0f\n", *(p + n));

   float *p1 = join(g, t, n);
   for(int i = 0; i<n-1; i++)
   printf("%.0f,", *(p1 + i));
   printf("%.0f\n", *(p1 + n));

   float *p2 = join(a, p1, n);
   for(int i = 0; i<n-1; i++)
   printf("%.0f,", *(p2 + i));
   printf("%.0f\n", *(p2 + n));

   float *p3 = join(p, p2, n);
   for(int i = 0; i<n-1; i++)
   printf("%.0f,", *(p3 + i));
   printf("%.0f\n", *(p3 + n));

   free(p);
   free(p1);
   free(p2);
   free(p3);

   return 0;
}