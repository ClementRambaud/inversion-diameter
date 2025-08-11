#include "graph_lib.h"
#include "stdlib.h"

int inverse(graph* g, int n, set X)
{
    int res = 0;
    for (int u=0; u < n-1; u++)
    {
        for (int v=u+1; v < n; v++)
        {
            if (IS_ADJ(g, u, v))
            {
                res = res << 1;
                if ((IN(u,X)) && (IN(v,X)))
                {
                    res |= 1;
                }
            }
        }
    }
    return res;
}

bool diam_L(graph* g, int n, int k)
{
    // test if diam(L(G))<=k
    int m = number_of_arcs(g, n)/2;
    int X, col, col2;

    char* colorings = calloc(1<<m, sizeof(char)); // 1st bit = reached; 2nd bit = to look at this round; 3rd bit = to look at at next round

    colorings[0] = 0b011;

    /*
    int* inversions = calloc(1<<n, sizeof(int)); // we store all the inversions
    for (X=0; X<(1<<n); X++)
    {
        inversions[X] = inverse(g,n,X);
    }
    */

    char* temp = calloc(1<<m, sizeof(char)); // to compute uniquely the inversions
    int number_of_inv = 0;

    for (X=0; X<(1<<n); X++)
    {
        temp[inverse(g,n,X)] = 1;
    }

    int* inversions = calloc(1<<n, sizeof(int));
    for (col=0; col < (1<<m); col++){
        if (temp[col])
        {
            inversions[number_of_inv] = col;
            number_of_inv++;
        }
    }

    fprintf(stderr, "number_of_inversions = %d, 2^n = %d \n", number_of_inv, 1<<n);

    free(temp);
    
    for (int i=0; i<k; i++)
    {
        // fprintf(stderr, "i = %d \n", i);
        for (col=0; col < (1<<m); col++)
        {
            if (colorings[col] & 0b10) // if this col was reached only the step before
            {
                for (X=0; X < number_of_inv; X++)
                {
                    col2 = inversions[X] ^ col;
                    if (! (colorings[col2] & 0b1))
                    {
                        colorings[col2] |= 0b101;
                    }
                }
            }
        }
        for (col=0; col < (1<<m); col++)
        {
            if (colorings[col] & 0b100)
            {
                colorings[col] = 0b011;
            }
        }
    }

    free(inversions);
    // fprintf(stderr, "plouf\n");

    for (col=0; col < (1<<m); col++)
    {
        // printf("colorings[%d] = %d \n", col,colorings[col]);
        if (colorings[col] == 0)
        {
            free(colorings);
            fprintf(stderr, "%b\n", col);
            return 0;
        }
    }
    free(colorings);
    return 1;
}


int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Expect one argument, zero given.\n");
        return 0;
    }
    char k = argv[1][0] - 48;
    graph g[MAXN];
    int n;
    while (1)
    {
      read_graph6(stdin, g, &n);
      if (feof(stdin)) break;
      if (diam_L(g,n,k)) {
          fprintf(stderr, "diam(L(G)) <= %d \n", k);
          fflush(stderr);
      }
      else
      {
          write_graph6(stdout, g, n);
          fprintf(stderr, "diam(L(G)) >  %d \n", k);
          fflush(stderr);
      }
    }
}

