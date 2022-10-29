#include<stdio.h>
#include<time.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 10000

//this is for the increment value
#define MAX 1000000000
int capacity[MAX_NODES][MAX_NODES]; // capacity matrix
int flow[MAX_NODES][MAX_NODES];
int color[MAX_NODES];
int vertex;
int edges;
int head, tail;
int q[MAX_NODES];
int pred[MAX_NODES];
void enqueue(int x)
{
    q[tail++] = x;
    color[x] = GRAY;

}
int dequeue()
{
    int temp = q[head];
    ++head;
    color[temp] = BLACK;
    return temp;
}

int bfs(int start, int target)
{
    int u,v;
    for( u = 0; u<vertex; u++)
    {
        color[u] = WHITE;
    }
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;
    // printf("%d ", start);
    while(head != tail)
    {
        u = dequeue();
        for(v = 0; v<vertex; v++)
        {
            if(color[v] == WHITE && capacity[u][v]>flow[u][v])
            {
                enqueue(v);
                pred[v] = u;
            }
        }
    }
    return color[target] == BLACK;
} 
int min(int a, int b)
{
    return (a<b)?a:b;
}
int max_flow(int source, int sink)
{
    int u;
    int maxFlow = 0 ;
    for(int i = 0; i<vertex; i++)
    {
        for(int j = 0; j<vertex; j++)
        {
            flow[i][j] = 0;
        }
    }
    while(bfs(source, sink))
    {
        // printf("hey");
        int increment = MAX;
        // //for choosing the minimum value in the flow
        for (u=vertex-1; pred[u]>=0; u=pred[u]) 
        {
            increment = min(increment,capacity[pred[u]][u]-flow[pred[u]][u]);
        }
            // Now increment the flow.
        for (u=vertex-1; pred[u]>=0; u=pred[u]) 
        {
            flow[pred[u]][u] += increment;
            // flow[u][pred[u]] -= increment;
        }
        maxFlow += increment;
    }
    return maxFlow;
}
void read_input_file() {
    int a,b,c,i,j;
    FILE* input = fopen("data.txt","r");
    // read number of nodes and edges
    fscanf(input,"%d %d",&vertex,&edges);
    printf("\nNumber of Vertices : %d   Edges : %d",vertex,edges);
    // initialize empty capacity matrix 
    for (i=0; i<vertex; i++) 
    {
        for (j=0; j<vertex; j++) 
        {
            capacity[i][j] = 0;
        }
    }
    // edges = vertex + 1;
    // read edge capacities
    for (i=0; i<edges; i++) 
    {
        fscanf(input,"%d %d %d",&a,&b,&c);
        capacity[a][b] = c;
        printf("\nA : %d  B : %d  Capacity : %d",a,b,c);
    }
    fclose(input);
}
int main()
{
    clock_t start, end;

    double cpu_time_used;
    read_input_file();
    start = clock();
    printf("\nMaxflow is %d",max_flow(0, vertex-1));
    // edges = vertex + 1;
    // bfs(0,vertex-1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTime taken is %lf", cpu_time_used);
}