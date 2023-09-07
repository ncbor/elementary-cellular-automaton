#include <stdio.h>
#define X 15
#define Y 7
int to_binary(int*, int []);
int ca_check_cell(int, int [], int []);

int main()
{
  int grid[Y][X]={0}, newgrid[Y][X], wr=30, rules[8];
  grid [0][X/2]=1;
  int (*g)[X] = grid,(*newg)[X] = newgrid;
  to_binary(&wr,rules);
  for(int k=0;k<Y;++k)
  {
    for(int i = Y-1;i>=0;--i)
    {
      for(int j = 0;j<X;++j)
      {
        printf("%d",*(g[i]+j));
        if(!i){*(newg[i]+j)=ca_check_cell(j,g[i],rules);}
        else{*(newg[i]+j)=*(g[i-1]+j);}
      }
      printf("\n");
    }
    int (*swap)[X] = g;
    g = newg;
    newg = swap;
    printf("\n");
  }

  return 0;
}

//Transform number between 0-255 into an array of its binaries
int to_binary(int*n, int bn[])
{
  for(int i=0;i<8;++i)
  {
    bn[i]=(*n)%2;
    (*n)/=2;
  }
  return 0;
}

int ca_check_cell(int i, int old[], int rules[])
{
  int c1 = i-1, c2 = i+1;
  if(c1<0){c1+=X;};
  if(c2>=X){c2-=X;};
  int n1=old[c1], n2=old[c2];
  switch(old[i])
  {
    case 0:
      if(!n1&&!n2&&rules[0])
      {
        return 1;
      }
      if(!n1&&n2&&rules[1])
      {
        return 1;
      }
      if(n1&&!n2&& rules[4])
      {
        return 1;
      }
      if(n1&&n2&&rules[5])
      {
        return 1;
      }
      break;
    case 1:
      if(!n1&&!n2&&rules[2])
      {
        return 1;
      }
      if(!n1&&n2&&rules[3])
      {
        return 1;
      }
      if(n1&&!n2&&rules[6])
      {
        return 1;
      }
      if(n1&&n2&&rules[7])
      {
        return 1;
      }
      break;
  }
  return 0;
}
