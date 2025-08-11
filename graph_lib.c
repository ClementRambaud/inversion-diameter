#include "graph_lib.h" 

inline void add_edge(graph* g, int u, int v)
{
  ADJ_SET(g,u) = ADD(v, ADJ_SET(g,u));
}

inline void remove_edge(graph* g, int u, int v)
{
  ADJ_SET(g,u) = DEL(v, ADJ_SET(g,u));
}

inline void empty_graph(graph* g, int n)
{
  int i;
  for (i=0; i<n; ++i)
  {
    g[i] = EMPTY;
  }
}

inline void copy_graph(graph* g, graph* g2, int n)
{
  int v;
  for(v=0; v<n; ++v)
  {
    ADJ_SET(g2, v) = ADJ_SET(g, v);
  }
}

inline int deg_out(graph* g, int n, int v)
{
  return CARD(ADJ_SET(g, v));
}

int deg_in(graph* g, int n, int v)
{
  int res = 0;
  int w;
  for (w=0; w<n; ++w)
  {
    if (IS_ADJ(g, w, v)) ++res;
  }
  return res;
}

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
int deg_out_min(graph* g, int n)
{
  int res = 1<<30;
  int v;
  for (v=0; v<n; ++v)
  {
    res = MIN(res, deg_in(g,n,v));
  }
  return res;
}

int deg_in_min(graph* g, int n)
{
  int res = 1<<30;
  int v;
  for (v=0; v<n; ++v)
  {
    res = MIN(res, deg_out(g,n,v));
  }
  return res;
}

inline int deg_min(graph* g, int n)
{
  return deg_out_min(g, n);
}

inline int number_of_arcs(graph* d, int n)
{
  int res = 0;
  int v;
  for (v=0; v<n; ++v)
  {
    res += CARD(ADJ_SET(d, v));
  }
  return res;
}

void read_digraph6(FILE *fi, graph* d, int *n)
/* fi: file;
 * n: the number of vertices will be written here;
 */
{
  char c; /* current character */
  int i,j; /* the current pair of vertices */
  int nb;
  int index;
  int matij;

  empty_graph(d, MAXN); /* clear the graph */

  c = fgetc(fi);
  if (c != '&') /* if it is not a directed graph */
  {
    return;
  }

  *n = fgetc(fi) - 63; /* we get the  numbe rof vertices */

  /* we read the matrix */
  nb = 0;
  for (i=0; i<*n; ++i)
  {
    for (j=0; j<*n; ++j)
    {
      // nb = (*n) * i + j;
      index = nb % 6;
      if (index == 0) /* we need another character */
      {
        c = fgetc(fi) - 63;
      }
      matij = (c >> (5-index)) & 1;
      if (matij)
      {
        // fprintf(stderr, "is_adj %d %d \n", i ,j);
        add_edge(d, i, j);
      }
      ++nb;
    }
  }
  if (fgetc(fi) != '\n' && fgetc(fi) != EOF)
  {
    fprintf(stderr, "ATTENTION, mauvaise lecture! \n");
  }
}

void read_graph6(FILE *fi, graph* g, int *n)
/* fi: file;
 * n: the number of vertices will be written here;
 */
{
  char c=0; /* current character */
  int i,j; /* the current pair of vertices */
  int nb;
  int index;
  int matij;

  empty_graph(g, MAXN); /* clear the graph */

  *n = fgetc(fi) - 63; /* we get the  numbe rof vertices */

  /* we read the matrix */
  nb = 0;
  for (i=0; i<*n; ++i)
  {
    for (j=0; j<i; ++j)
    {
      // nb = (*n) * i + j;
      index = nb % 6;
      if (index == 0) /* we need another character */
      {
        c = fgetc(fi) - 63;
      }
      matij = (c >> (5-index)) & 1;
      if (matij)
      {
        // fprintf(stderr, "is_adj %d %d \n", i ,j);
        add_edge(g, i, j);
        add_edge(g, j, i);
      }
      ++nb;
    }
  }
  if (fgetc(fi) != '\n' && fgetc(fi) != EOF)
  {
    fprintf(stderr, "ATTENTION, mauvaise lecture! \n");
  }
}

void print_graph(FILE* fi, graph* g, int n)
{
  int i,j;
  fprintf(fi, "graph of order %d: \n", n);
  for (i=0; i<n; ++i)
  {
    fprintf(fi, "%d: ", i);
    for (j=0; j<n; ++j)
    {
      if IS_ADJ(g, i, j)
      {
        fprintf(fi, "%d ", j);
      }
    }
    fprintf(fi, "\n");
  }
  fprintf(fi, "\n");
}

void write_digraph6(FILE* fi, graph* d, int n)
{
  int taille = CEILING(n*n, 6); /* nb of needed char */
  char result[CEILING(MAXN*MAXN, 6)];
  int i,j;
  int v;
  int nb, index_char, index;

  /* clear the result */
  for (i=0; i<taille; ++i)
  {
    result[i] = 0;
  }

  nb = 0;
  for (i=0; i<n; ++i)
  {
    for (j=0; j<n; ++j)
    {
      if (IS_ADJ(d, i, j))
      {
        // fprintf(stderr, "is_adj %d %d \n", i, j);
        v = 1;
      }
      else
      {
        v = 0;
      }
      index_char = nb / 6;
      index = nb % 6;
      // fprintf(stderr, "index = %d, index_char = %d, (v << (5-index)) = %d \n",
      //         index, index_char, (v << (5 - index)));
      result[index_char] |= (v << (5 - index));
      // fprintf(stderr, "%x\n", result[index_char]);
      ++nb;
    }
  }
  fputc('&', fi); /* directed */
  fputc(63 + n, fi); /* nb of vertices */
  for (i=0; i<taille; ++i)
  { /* then we print the adj matrix */
    fputc(result[i] + 63, fi);
  }
  fputc('\n', fi);
}
void write_graph6(FILE* fi, graph* d, int n)
{
  int taille = CEILING(n*(n-1), 12); /* nb of needed char */
  // fprintf(stderr, "taille = %d \n", taille);
  char result[CEILING(MAXN*MAXN, 6)];
  int i,j;
  int v;
  int nb, index_char, index;

  /* clear the result */
  for (i=0; i<taille; ++i)
  {
    result[i] = 0;
  }

  nb = 0;
  for (i=0; i<n; ++i)
  {
    for (j=0; j<i; ++j)
    {
      if (IS_ADJ(d, i, j))
      {
        v = 1;
      }
      else
      {
        v = 0;
      }
      index_char = nb / 6;
      index = nb % 6;
      result[index_char] |= (v << (5 - index));
      ++nb;
    }
  }
  fputc(63 + n, fi); /* nb of vertices */
  for (i=0; i<taille; ++i)
  { /* then we print the adj matrix */
    fputc(result[i] + 63, fi);
  }
  fputc('\n', fi);
}


