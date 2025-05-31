#include <stdio.h>
#include <stdlib.h>
#include "template.h"

// Queue Node structure for BuyerTree
typedef struct QueueNode {
    BuyerTree* treeNode;
    struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Queue functions
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, BuyerTree* node) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->treeNode = node;
    temp->next = NULL;
    if (!q->rear) q->front = q->rear = temp;
    else {
        q->rear->next = temp;
        q->rear = temp;
    }
}

BuyerTree* dequeue(Queue* q) {
    if (!q->front) return NULL;
    QueueNode* temp = q->front;
    BuyerTree* node = temp->treeNode;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    return node;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// 📘 Level Order Traversal
void levelOrderTraversalBuyers(BuyerTree* root) {
    if (!root) return;

    Queue* q = createQueue();
    enqueue(q, root);

    while (!isQueueEmpty(q)) {
        BuyerTree* current = dequeue(q);

        printf("\n--- Buyer Node ---\n");
        for (int i = 0; i < current->numKeys; i++) {
            printf("BuyerID: %u\n", current->buyerdata[i].buyerID);
        }

        if (!current->is_leaf) {
            for (int i = 0; i <= current->numKeys; i++) {
                if (current->children[i])
                    enqueue(q, current->children[i]);
            }
        }
    }
}

