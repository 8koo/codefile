#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct TreeNode {
    int id;
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode, *TreeNodePtr;

typedef struct ListNode {
    struct TreeNode *node; // 队列的值的类型是树节点指针
    struct ListNode *next;
} ListNode, *ListNodePtr;

typedef struct Queue {
    ListNodePtr dummyHead;
    ListNodePtr tail;
    int size;
} *QueuePtr;

// 创建链表的节点
ListNodePtr createListNode(TreeNodePtr node, ListNodePtr next) {
    ListNodePtr curr = (ListNodePtr) (malloc(sizeof(ListNode)));
    curr->node = node;
    curr->next = next;
    return curr;
}

// 创建树的节点
int TreeId = 0;

TreeNodePtr createTreeNode(int val, TreeNodePtr left, TreeNodePtr right) {
    TreeNodePtr curr = (TreeNodePtr) (malloc(sizeof(TreeNode)));
    curr->id = TreeId++;
    curr->val = val;
    curr->left = left;
    curr->right = right;
    return curr;
}

// 单链表队列初始化
QueuePtr InitQueue() {
    QueuePtr queue = (QueuePtr) malloc(sizeof(struct Queue));
    TreeNodePtr dummyTreeNode = createTreeNode(-1, NULL, NULL);
    queue->size = 0;
    queue->dummyHead = createListNode(dummyTreeNode, NULL);
    queue->tail = queue->dummyHead;
    return queue;
}

// 在 queue 的尾部添加一个元素的副本
void EnQueue(QueuePtr queue, TreeNodePtr node) {
    ListNodePtr curr = createListNode(node, NULL);
    queue->tail->next = curr;
    queue->tail = queue->tail->next;
    queue->size++;
}

// 删除 queue 中的第一个元素
void DeQueue(QueuePtr queue) {
    if (queue->size == 0) {
        perror("error! the size of queue is zero when call DeQueue().");
        return;
    }
    ListNodePtr head = queue->dummyHead->next;
    queue->dummyHead->next = head->next;
    queue->size--;
    if (queue->size == 0) queue->tail = queue->dummyHead;
    free(head);
}

// 如果 queue 中没有元素, 返回 true
bool QueueEmpty(QueuePtr queue) {
    return queue->size == 0;
}

// 返回 queue 中第一个元素的引用
TreeNodePtr GetHead(QueuePtr queue) {
    if (QueueEmpty(queue)) {
        perror("error! the size of queue is zero when call front().");
        return NULL;
    } else {
        return queue->dummyHead->next->node;
    }
}

int max(int a, int b) {
    return (a >= b) ? a : b;
}

// 将输入转换为数组
void getDigits(char *buff, int *data) {
    int len = strlen(buff);
    int index = 0;
    for (int i = 0; i < len; i++) {
        int num = 0;
        if (buff[i] == '#') {
            num = -1;
            i++;
        } else {
            while (buff[i] != ' ' && buff[i] != '\0') {
                num = num * 10 + (buff[i++] - '0');
            }
        }
        data[index++] = num;
    }
}

// 创建 dot 可视化文件
void createDotFile(const char *filename, TreeNodePtr root, int MaxSize) {
    FILE *fp = fopen(filename, "w");    // 文件指针
    if (fp == NULL) {   // 为NULL则返回
        printf("File cannot open!");
        exit(0);
    }
    fprintf(fp, "digraph G {\n");   // 开头
    // 利用层次遍历构造
    QueuePtr queue = InitQueue();
    EnQueue(queue, root);
    int id = 1;
    while (!QueueEmpty(queue)) { // 若队列不空，继续遍历。否则，遍历结束
        TreeNodePtr curr = GetHead(queue);
        DeQueue(queue);
        if (curr == NULL) continue;
        fprintf(fp, "%d [shape=circle, label=\"%d\"];\n", curr->id, curr->val);
        if (curr->left != NULL) { // 如果有左孩子，左孩子入队
            EnQueue(queue, curr->left);
            fprintf(fp, "%d->%d;\n", curr->id, curr->left->id);
        }
        id++;
        // 中间虚拟节点
        fprintf(fp, "_n%d [shape=circle, label=\"#\", style=invis];\n", id);
        fprintf(fp, "%d->_n%d [style=invis, weight=10];\n", curr->id, id);
        if (curr->right != NULL) { // 如果有右孩子，右孩子入队
            EnQueue(queue, curr->right);
            fprintf(fp, "%d->%d;\n", curr->id, curr->right->id);
        }
        id++;
    }
    fprintf(fp, "}\n"); // 结尾
    fclose(fp); // 关闭IO
}


// 绘制二叉树图片,调用 createDotFile 并使用 system 执行命令
void plot(TreeNodePtr tree_root, int i, int size, char *name) {
    char tree_filename[50], paint_tree[100];
    sprintf(tree_filename, "./%s_%d.dot", name, i);
    createDotFile(tree_filename, tree_root, size);
    sprintf(paint_tree, "dot -Tpng %s -o ./%s_%d.png", tree_filename, name, i);
    // puts(paint_tree);
    system(paint_tree);
}


// 删除二叉树
void destoryTree(TreeNodePtr root) {
    if (!root) return;
    if (root->left) {
        destoryTree(root->left);
        root->left = NULL;
    }
    if (root->right) {
        destoryTree(root->right);
        root->right = NULL;
    }
    free(root);
}

void inOrderTraverse(TreeNodePtr root) {
    if (root) {
        inOrderTraverse(root->left);
        if (root->val != -1) {
            printf(" %d", root->val);
        }
        inOrderTraverse(root->right);
    }
}

TreeNodePtr createTreeWithLevelOrder(int *data, int size) {
    if (size == 0) {
        return NULL;
    }
    // 创建根节点
    TreeNodePtr root = createTreeNode(data[0], NULL, NULL);
    // 创建一个队列，并将根节点加入队列中
    QueuePtr queue = InitQueue();
    EnQueue(queue, root);
    int i = 1;
    while (i < size) {
        // 取出队列头部的节点
        TreeNodePtr parent = GetHead(queue);
        // 如果该节点的左子树为空，则新建节点作为该节点的左子树，否则将该节点的左子树加入队列
        if (parent->left == NULL) {
            TreeNodePtr child = createTreeNode(data[i], NULL, NULL);
            parent->left = child;
            EnQueue(queue, child);
            i++;
        }
        // 如果该节点的右子树为空，则新建节点作为该节点的右子树，否则将该节点的右子树加入队列
        else if (parent->right == NULL) {
            TreeNodePtr child = createTreeNode(data[i], NULL, NULL);
            parent->right = child;
            EnQueue(queue, child);
            i++;
        }
        // 如果该节点的左右子树均不为空，则将该节点出队
        else {
            DeQueue(queue);
        }
    }
    // 释放队列
    free(queue->dummyHead);
    free(queue);
    return root;
}

/** TODO: 任务二 通过递归求取该树的镜像，即翻转该二叉树 */
TreeNodePtr invertTree(TreeNodePtr root) {
    if (root == NULL) {
        return NULL;
    }
    // 交换左右子树
    TreeNodePtr temp = root->left;
    root->left = root->right;
    root->right = temp;
    // 递归交换左右子树的左右子树
    invertTree(root->left);
    invertTree(root->right);
    return root;
}

int main() {
    int SIZE = 128;
    int MAX_NUM = 10;
    char buff[SIZE];
    int num;
    bool use_graphviz = false;


    int i = 0;
    while (scanf("%d\n", &num) == 1) {
        fgets(buff, SIZE, stdin);
        char *pos = strchr(buff, '\n');          //查找换行符
        if (pos)                            //如果find不为空指针
            *pos = '\0';                    //就把一个空字符放在这里

        // Mac/Linux 替换\r
        pos = strchr(buff, '\r');
        if (pos)
            *pos = '\0';


        int size = num;
        int data[size];
        getDigits(buff, data);

        TreeNodePtr tree_root = createTreeWithLevelOrder(data, size);
        
        /** 任务二 */
        TreeNodePtr invert_tree_root = invertTree(tree_root);
        printf("inOrderTraverse of invert trees is:");
        inOrderTraverse(invert_tree_root);
    }
}
