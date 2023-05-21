//本题不提供模板代码
#include <stdio.h>
#include <stdlib.h>

// 定义边的结构体
typedef struct Edge {
    int src, dest, weight;
} Edge;

// 定义图的结构体
typedef struct Graph {
    int V, E;
    Edge* edge;
} Graph;

// 创建图
Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge*)malloc(E * sizeof(Edge));
    return graph;
}

// 查找父节点
int findParent(int parent[], int i) {
    if (parent[i] == -1)
        return i;
    return findParent(parent, parent[i]);
}

// 合并两个子集
void unionSets(int parent[], int x, int y) {
    int xset = findParent(parent, x);
    int yset = findParent(parent, y);
    parent[xset] = yset;
}

// 比较函数用于边的排序
int compare(const void* a, const void* b) {
    Edge* edge1 = (Edge*)a;
    Edge* edge2 = (Edge*)b;
    return edge1->weight - edge2->weight;
}

// 使用Kruskal算法求解最小生成树
void kruskalMST(Graph* graph) {
    int V = graph->V;
    Edge result[V];  // 存储最小生成树的边
    int e = 0;       // 记录已加入结果集的边的数量
    int i = 0;       // 遍历排序后的边集合的索引
    int totalWeight = 0; // 最小生成树的总权值

    // 对所有边进行排序
    qsort(graph->edge, graph->E, sizeof(Edge), compare);

    // 分配内存用于创建V个子集
    int* parent = (int*)malloc(V * sizeof(int));

    // 初始化所有子集为独立的单元集
    for (int v = 0; v < V; ++v)
        parent[v] = -1;

    // 不断选择权值最小的边并判断是否形成环路
    while (e < V - 1 && i < graph->E) {
        Edge next_edge = graph->edge[i++];

        int x = findParent(parent, next_edge.src);
        int y = findParent(parent, next_edge.dest);

        // 如果加入该边不会形成环路，则将其加入最小生成树集合
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, x, y);
            totalWeight += next_edge.weight;
        }
    }


    printf("%d\n", totalWeight);

    free(parent);
}

int main() {
    int V = 0;  // 城市数量
    int E = 0;  // 道路数量
    scanf("%d",&V);
    scanf("%d",&E);
    Graph* graph = createGraph(V, E);

    // 添加道路及其权值
    for (int i = 0; i < E; ++i) {
       
        scanf("%d %d %d", &graph->edge[i].src, &graph->edge[i].dest, &graph->edge[i].weight);
    }

    kruskalMST(graph);

    return 0;
}
