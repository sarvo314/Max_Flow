#include<stdio.h>
#include<time.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 10000

//this is for the increment value
#define MAX 2147483647
int capacity[MAX_NODES][MAX_NODES]; // capacity matrix(original graph)
int Flow[MAX_NODES][MAX_NODES];//stores the flow
int residualGraph[MAX_NODES][MAX_NODES];//same as capacity graph
int level[MAX_NODES];//stores the level of the matrix
int color[MAX_NODES];
int count[MAX_NODES]; //stores how many neighours are visited
int vertex;
int edges;
int head, tail;
int q[MAX_NODES];
int pred[MAX_NODES];
int flow_max;

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
    //returns the eleemnt that is deleted
}

int bfs(int start, int target)
{
    int u,v;
    for( u = 0; u<vertex; u++)
    {
        color[u] = WHITE;
        level[u] = -1;
    }
    head = tail = 0;
    enqueue(start);
    pred[start] = -1;

    level[start] = 0;
    while(head != tail)
    {
        u = dequeue();

        for(v = 0; v<vertex; v++)
        {
            
            if(u != v && residualGraph[u][v] > 0 && level[v] < 0)
            {
                // printf("\nu %d v%d\n", u,v);
                level[v] = level[u] + 1;
                enqueue(v);
            }
        }
    }
     
    // return color[target] == BLACK;
    return level[target] < 0 ? 0 : 1;
} 
int min(int a, int b)
{
    return (a<b)?a:b;
}

//A dfs based function
int dfs(int u, int sink, int flow)
{
    if(u == sink)
        return flow;

    //stores the number of vertex connected to u
    int countOfU = 0;
    for(int i = 0; i<vertex;i++)
    {
        if(capacity[u][i]>0)
            countOfU += 1;
    }
    if(count[u] == countOfU)
        return 0;
    
    for(int v = 0; v <vertex; v++)
    {
        if(residualGraph[u][v]>0)
        {
            count[u]++;
            if(level[v] == level[u]+1)
            {
                int curr_flow = min(flow, residualGraph[u][v]);
                int min_cap = dfs(v, sink,curr_flow);
                // printf("\n%d u %d v %d currflow",u,v, curr_flow);
                // printf("\n%d min cap is\n", min_cap);
                if(min_cap > 0)
                {
                    residualGraph[u][v] -= min_cap;

                    residualGraph[v][u] += min_cap;
                    
                    return min_cap;
                }
            }
        }
    }

    return 0;

}

int max_flow(int source, int sink)
{
    int u;
    int maxFlow;
    for(int i = 0; i<vertex; i++)
    {
        for(int j = 0; j<vertex; j++)
        {
            Flow[i][j] = 0;
        }
    }
    while(bfs(source, sink))
    {
        int increment = MAX;
        //for choosing the minimum value in the flow
        for (u=vertex-1; pred[u]>=0; u=pred[u]) 
        {
            increment = min(increment,capacity[pred[u]][u]-Flow[pred[u]][u]);
        }
            // Now increment the flow.
        for (u=vertex-1; pred[u]>=0; u=pred[u]) 
        {
            Flow[pred[u]][u] += increment;
            // flow[u][pred[u]] -= increment;
        }
        maxFlow += increment;
    }
    return maxFlow;
}
int dinic(int source, int sink)
{
    flow_max = 0;
    int flow;
    
    if(source == sink)
        return -1;

    while(bfs(0, vertex -1))
    {
        for (int i=0; i<vertex; i++) 
        {
            count[i] = 0;
        }
        while(flow = dfs(0, vertex-1,MAX))
        {
            flow_max += flow;
        }
    }
    return flow_max;

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
        count[i] = 0;
        // level[i] = -1;
        for (j=0; j<vertex; j++) 
        {
            capacity[i][j] = 0;
        }

    }
    // read edge capacities
    for (i=0; i<edges; i++) 
    {
        fscanf(input,"%d %d %d",&a,&b,&c);
        capacity[a][b] = c;
        residualGraph[a][b] = c;
        printf("\nA : %d  B : %d  Capacity : %d",a,b,c);
    }
    fclose(input);
}
int main()
{
    clock_t start, end;;

    double cpu_time_used;
    read_input_file();
    
    start = clock();
    printf("\nMax flow is %d\n",dinic(0, vertex-1));
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTime taken is %lf", cpu_time_used);
}