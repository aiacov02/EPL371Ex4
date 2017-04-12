
#include <malloc.h>
#include "queue.h"




void Create(Queue** q) {
    *q = (Queue*) malloc(sizeof (Queue));
    if (*q == NULL) {
        return;
    }

    (*q)->size = 0;
    (*q)->head = NULL;
    (*q)->tail = NULL;


}

void Destroy(Queue *queue) {
    while (!isEmpty(queue)) {
        DeQueue(queue);
    }
    free(queue);
}

void EnQueue(Queue *q, NODE *newnode) {
    /* Bad parameter */
    if ((q == NULL) || (newnode == NULL)) {
        return;
    }

    /*the queue is empty*/
    newnode->prev = NULL;
    if (q->size == 0) {
        q->head = newnode;
        q->tail = newnode;

    } else {
        /*adding item to the end of the queue*/
        q->tail->prev = newnode;
        q->tail = newnode;
    }
    q->size++;
    return;
}

int DeQueue(Queue *q) {
    /*the queue is empty or bad param*/
    NODE *newnode;
    if (isEmpty(q)) return 0;
    newnode = q->head;
    q->head = (q->head)->prev;
    q->size--;
    int temp = newnode->n_number;
    free(newnode);
    return temp;
}

int isEmpty(Queue *q) {
    if (q == NULL) {
        return 1;
    }else if (q->size == 0) {
        return 1;
    }
    else{
        return 0;
    }
}


//int main() {
//    int i;
//    Queue *pQ = ConstructQueue(7);
//    NODE *pN;
//	int x;
//    for (i = 0; i < 9; i++) {
//        pN = (NODE*) malloc(sizeof (NODE));
//        pN->data = 100 + i;
//        Enqueue(pQ, pN);
//    }
//    while (!isEmpty(pQ)) {
//        x = Dequeue(pQ);
//        printf("\nDequeued: %d", x);
//
//    }
//    DestructQueue(pQ);
//    return (EXIT_SUCCESS);
//}