//
// Created by andreas on 11/04/2017.
//

typedef struct Node {
    int n_number;
    struct Node *prev;
} NODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
} Queue;

void Create(Queue **q);
void Destroy(Queue *q);
void EnQueue(Queue *q, NODE *newnode);
int DeQueue(Queue *q);
int isEmpty(Queue* q);
