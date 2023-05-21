#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define max_dis 100000

typedef char vextype[20];

typedef struct {
    int N, E;//N是顶点数，E是边数
    int** matrix;//储存邻接矩阵
    vextype* vertex;//存储节点的名字
} Graph;

Graph createGraph(int n);
int isConnected(Graph g);
int Diameter(Graph g);
int Radius(Graph g);
int dijkstra(Graph g, int start, int end, int* path);
void printPath(int d, int* diameter_path, Graph g);

void DFS(Graph g, int i, int* visited, int* count);
int DFSTraverse(Graph g);
void floyd(Graph g, int** dist, int** path);


/**
 * 创建一个节点数为n的图
 * @param n 节点个数
 * @return 返回这个图
 */
Graph createGraph(int n) {
    int i, j;
    Graph g;
    g.N = n;
    g.matrix = (int**)malloc(sizeof(int*) * g.N);
    for (i = 0; i < n; i++) {
        g.matrix[i] = (int*)malloc(sizeof(int) * g.N);
    }
    for (i = 0; i < g.N; i++) {
        for (j = 0; j < g.N; j++) {
            g.matrix[i][j] = max_dis;
        }
    }
    for (i = 0; i < g.N; i++) {
        g.matrix[i][i] = 0;
    }
    g.vertex = (vextype*)malloc(sizeof(vextype) * g.N);
    return g;
}


/**
 * 根据距离d和路径数组path输出路径，这样就不需要路径的节点数也能正确输出路径
 * @param d 路径长度
 * @param diameter_path 储存路径的数组
 * @param g 图
 */
void printPath(int d, int *diameter_path, Graph g)
{
    int k = 0;
    int path_length = 0;
    printf("Path: ");
    do {
        printf("%s->", g.vertex[diameter_path[k]]);
        path_length += g.matrix[diameter_path[k]][diameter_path[k + 1]];
        k++;
    } while (path_length < d);
    printf("%s\n", g.vertex[diameter_path[k]]);
}

/**
 * 判断图是否连通
 * @param g 图
 * @return 连通返回1，否则返回0
 */
int isConnected(Graph g) {
    int i, j, k;
    int** dist = (int**)malloc(sizeof(int*) * g.N); // 存储任意两个节点之间的最短距离

    // 初始化数据
    for (i = 0; i < g.N; i++) {
        dist[i] = (int*)malloc(sizeof(int) * g.N);
        for (j = 0; j < g.N; j++) {
            dist[i][j] = g.matrix[i][j];
        }
    }

    // 使用Floyd算法计算最短路径
    for (k = 0; k < g.N; k++) {
        for (i = 0; i < g.N; i++) {
            for (j = 0; j < g.N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // 检查是否存在不可达的节点
    for (i = 0; i < g.N; i++) {
        for (j = 0; j < g.N; j++) {
            if (dist[i][j] == max_dis) {
                // 存在不可达节点，图不连通
                for (i = 0; i < g.N; i++) {
                    free(dist[i]);
                }
                free(dist);
                return 0;
            }
        }
    }

    // 释放内存
    for (i = 0; i < g.N; i++) {
        free(dist[i]);
    }
    free(dist);

    return 1;
}


/**
 * 使用dijkstra算法计算单源最短路径
 * @param g 图
 * @param start 起点
 * @param end 终点
 * @param path 从start到end的路径, [start,...,end]
 * @return 路径长度
 */
int dijkstra(Graph g, int start, int end, int *path) {
    int i, j, u, v;
    int* dist = (int*)malloc(sizeof(int) * g.N); // 存储起点到各个节点的最短距离
    int* visited = (int*)malloc(sizeof(int) * g.N); // 标记节点是否已访问
    int* prev = (int*)malloc(sizeof(int) * g.N); // 记录最短路径中的前驱节点
    int minDist, nextNode;

    // 初始化数据
    for (i = 0; i < g.N; i++) {
        dist[i] = g.matrix[start][i];
        visited[i] = 0;
        prev[i] = start;
    }
    visited[start] = 1;

    // 寻找最短路径
    for (i = 0; i < g.N; i++) {
        minDist = max_dis;
        nextNode = -1;

        // 选择未访问节点中距离起点最近的节点
        for (j = 0; j < g.N; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                nextNode = j;
            }
        }

        // 如果没有找到下一个节点，则跳出循环
        if (nextNode == -1) {
            break;
        }

        visited[nextNode] = 1;

        // 更新最短路径
        for (v = 0; v < g.N; v++) {
            if (!visited[v] && dist[nextNode] + g.matrix[nextNode][v] < dist[v]) {
                dist[v] = dist[nextNode] + g.matrix[nextNode][v];
                prev[v] = nextNode;
            }
        }
    }

    // 构建最短路径
    int pathLength = 0;
    u = end;
    while (u != start) {
        path[pathLength++] = u;
        u = prev[u];
    }
    path[pathLength++] = start;

    // 反转路径数组
    for (i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - i - 1];
        path[pathLength - i - 1] = temp;
    }

    // 释放内存
    free(dist);
    free(visited);
    free(prev);

    return dist[end];
}

/**
 * 计算图的直径。提示：Floyd算法
 * @param g 图
 * @return 直径的长度
 */
int Diameter(Graph g) {
    int i, j, k;
    int** dist = (int**)malloc(sizeof(int*) * g.N); // 存储任意两个节点之间的最短距离
    int maxDist = 0;

    // 初始化数据
    for (i = 0; i < g.N; i++) {
        dist[i] = (int*)malloc(sizeof(int) * g.N);
        for (j = 0; j < g.N; j++) {
            dist[i][j] = g.matrix[i][j];
        }
    }

    // 使用Floyd算法计算最短路径
    for (k = 0; k < g.N; k++) {
        for (i = 0; i < g.N; i++) {
            for (j = 0; j < g.N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // 寻找直径
    for (i = 0; i < g.N; i++) {
        for (j = 0; j < g.N; j++) {
            if (dist[i][j] > maxDist) {
                maxDist = dist[i][j];
            }
        }
    }

    // 释放内存
    for (i = 0; i < g.N; i++) {
        free(dist[i]);
    }
    free(dist);

    return maxDist;
}



/**
 * 计算图的半径
 * @param g 图
 * @return 半径长度
 */
int Radius(Graph g) {
    int i, j, k;
    int** dist = (int**)malloc(sizeof(int*) * g.N); // 存储任意两个节点之间的最短距离
    int minEccentricity = max_dis; // 初始值设置为最大值

    // 初始化数据
    for (i = 0; i < g.N; i++) {
        dist[i] = (int*)malloc(sizeof(int) * g.N);
        for (j = 0; j < g.N; j++) {
            dist[i][j] = g.matrix[i][j];
        }
    }

    // 使用Floyd算法计算最短路径
    for (k = 0; k < g.N; k++) {
        for (i = 0; i < g.N; i++) {
            for (j = 0; j < g.N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // 计算Eccentricity，并找到最小值
    for (i = 0; i < g.N; i++) {
        int maxDist = 0;
        for (j = 0; j < g.N; j++) {
            if (dist[i][j] > maxDist) {
                maxDist = dist[i][j];
            }
        }
        if (maxDist < minEccentricity) {
            minEccentricity = maxDist;
        }
    }

    // 释放内存
    for (i = 0; i < g.N; i++) {
        free(dist[i]);
    }
    free(dist);

    return minEccentricity;
}



int main() {
    int node_num;
    int edge_num;


    scanf("%d %d", &node_num, &edge_num);

    Graph g = createGraph(node_num);
    for(int i = 0; i < node_num; i++) {
        sprintf(g.vertex[i], "%d", i);
    }
    for (int i = 0; i < edge_num; i++) {
        int start_idx, end_idx, weight;
        scanf("%d %d %d", &start_idx, &end_idx, &weight);
        g.matrix[start_idx][end_idx] = weight;
        g.matrix[end_idx][start_idx] = weight;
    }
    printf("connected: %d\n", isConnected(g));

    if (isConnected(g))
    {
        int* short_path = (int*)malloc(sizeof(int) * g.N);
        int dis = dijkstra(g, 1, 3, short_path);
        printf("the minimum fare between 1 and 3: %d\n", dis);
        printPath(dis, short_path, g);
        free(short_path);
        int d = Diameter(g);
        printf("diameter:%d\n", d);

        int r = Radius(g);
        printf("radius:%d\n", r);
    }

    return 0;
}