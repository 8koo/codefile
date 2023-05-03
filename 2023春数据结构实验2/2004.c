#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>

struct TreeNode *findNode(struct TreeNode *root, int num);//找到对应数据域的结点
void findPath(struct TreeNode *root, struct TreeNode *p, struct TreeNode *q);//打印路径
bool judge(struct TreeNode *root, int start, int end);//判断两结点是否可达
struct TreeNode *createTreeNode(int val, struct TreeNode *left, struct TreeNode *right);//创建树的节点
struct TreeNode *createTreeWithLevelOrder(int *data, int size);//按照层次序列创建二叉树

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
	if (queue->size == 0)
		queue->tail = queue->dummyHead;
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


//功能：找到对应数据域的结点
//传参：root为整棵树的树根地址，num为想要查找的结点val值
//返回值：返回值为找到的结点的地址
struct TreeNode *findNode(struct TreeNode *root, int num) {
    if (root == NULL) {
        return NULL;
    }
    if(root->left!=NULL)
    {
     struct TreeNode *left = findNode(root->left, num);
     if (left!= NULL){
        return left;
      }
    }
    if(root->right!=NULL)
    {
      struct TreeNode *right = findNode(root->right, num);
      if (right != NULL) {
        return right;
        }
    }
    if (root->val == num) {
        return root;
    }else{
          return NULL;
      }

}


//功能：打印路径
//传参：root为整棵树的树根地址，p为起始结点的地址，q为结束结点的地址，需要打印p到q的路径，路径包含p，q两个端点
//返回值：无返回值，该函数将完成打印路径的功能
void findPath(struct TreeNode *root, struct TreeNode *p, struct TreeNode *q) {
    if (root == NULL || p == NULL || q == NULL) {
        return;
    }

    struct TreeNode *cur = p;  // 从p开始遍历
    
    while (cur != NULL&&cur->val!=-1&&cur->val!=q->val) {
        printf("%d->", cur->val);
        if (judge(cur,cur->right->val,q->val)) {  // 如果目标节点在右子树，则继续遍历右子树
            cur = cur->right;
        } else {  // 否则遍历左子树
            cur = cur->left;
        }
    }        
            printf("%d",q->val);// 找到了目标节点q，退出循环

}
   

    
 






//功能：判断两结点是否可达
//传参：root为整棵树的树根地址，start为起始结点的val，end为结束结点的val
//返回值：返回值为布尔类型，返回true则表示可达，返回false表示不可达，不可达在主函数调用后会打印Too Far!!!，可达则打印两结点间路径
bool judge(TreeNode *root, int start, int end) {
    if (root == NULL) {
        return false;
    }

    TreeNode *node = findNode(root, start);
    if (node == NULL || findNode(node, end) == NULL) {
        return false;
    } else {
        return true;
    }
}

//功能：按照层次序列创建二叉树
//传参：data为输入的层次序列经过处理后的数组，-1代表空结点，size为data数组的长度
//返回值：返回值为创建出来的二叉树的根结点的地址值
//注：本函数直接套用前面三题层次序列创建二叉树的函数即可，做过前三题的本题该函数不必单独设计
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


int main(void) {
	int i, j, num, SIZE = 128;
	char buff[SIZE];
	scanf("%d\n", &num);
	fgets(buff, SIZE, stdin);
	char *pos = strchr(buff, '\n');          //查找换行符
	if (pos)                            //如果find不为空指针
		*pos = '\0';                    //就把一个空字符放在这里

	// Mac/Linux 替换\r
	pos = strchr(buff, '\r');
	if (pos)
		*pos = '\0';
	int *data = (int *) malloc(num * sizeof(int));
	getDigits(buff, data);
	struct TreeNode *root = createTreeWithLevelOrder(data, num), *start, *end;
	
	//以上为层次序列创建二叉树
	
	//以下为本题找路径的主函数代码
	
	scanf("%d %d", &i, &j);
	if (!judge(root, i, j))
		printf("Too Far!!!");
	else {
		start = findNode(root, i);
		end = findNode(root, j);
		findPath(root, start, end);
	}
	return 0;
}