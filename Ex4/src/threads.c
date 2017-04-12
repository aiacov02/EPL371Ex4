#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include "queue.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



#include "functions.h"

void *thread_func(void *);




static pthread_mutex_t mutex;
static pthread_cond_t cond;
Queue *q;

int numofthreads;
int port;
char *serverpath;

void *thread_func(void *arg) {
    int err;
    int x =0;
    while(1){

        if (err = pthread_mutex_lock(&mutex)) { /* lock mutex */
            printf("pthread_mutex_lock: %s\n",strerror(err));
            exit(2);
        }

        // check if queue is empty
        while(isEmpty(q)) {

            // wait producer to put a node in the queue
            if (err = pthread_cond_wait(&cond, &mutex)) {
                printf("pthread_cond_wait: %s\n", strerror(err));
                exit(2);
            }
        }
        // modify shared variable head

        int socketServed = DeQueue(q);


        if (err = pthread_mutex_unlock(&mutex)) { /* unlock mutex */
            printf("pthread_mutex_unlock: %s\n",strerror(err));
            exit(2);
        }

         do{
         printf("\ntest\n");
            threadplay(&x,&socketServed, &port, &serverpath, &numofthreads);

         } while(x==0);

        close(socketServed);

    }


}




int main() {
    int sock, newsock, serverlen, clientlen;
    //char buf[2560];
    struct sockaddr_in server, client;
    struct sockaddr *serverptr, *clientptr;
    struct hostent *rem;

    void *status;
    int err, i;
    struct node *p;
    Create(&q);
    NODE *newnode;



    /* Create socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(-1);
    }

    /* Convert port number to integer */
    getSettings(&port,&serverpath,&numofthreads);

    pthread_t tid[numofthreads];

    printf("\n*********************************************");
    printf("\nCHECK GET SETTINGS FUNCTION");
    printf("\nThe port is : %d",port);
    printf("\nThe server path is : %s",serverpath);
    printf("\nThe number of threds is : %d",numofthreads);
    printf("\n*********************************************\n\n");

    server.sin_family = PF_INET; /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* My Internet address */
    server.sin_port = htons(port); /* The given port */
    serverptr = (struct sockaddr *) &server;
    serverlen = sizeof(server);

    /* Bind socket to address */
    if (bind(sock, serverptr, serverlen) < 0) {
        perror("bind");
        exit(-1);
    }
    /* Listen for connections */
    if (listen(sock, 5) < 0) {
        /* 5
         max. requests in queue */
        perror("listen");
        exit(-1);
    }
    printf("Listening for connections to port %d\n", port);

   // pthread_mutex_init(&mutex, NULL);
    /* create thread */
    for(int i=0; i<numofthreads; i++) {
        if (err = pthread_create(&tid[i], NULL, thread_func, NULL)) {
            printf("pthread_create: %s\n", strerror(err));
            exit(-1);
        }
    }
    while(1){

        //havebody = 0;
        clientptr = (struct sockaddr *) &client;
        clientlen = sizeof(client);
        /* Accept connection */
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0) {
            perror("accept");
            exit(-1);
        }

        /* put nodes in the queue */
        if(q->size<=numofthreads){
            newnode = (NODE *) malloc(sizeof(NODE));
            newnode->n_number = newsock;
            EnQueue(q, newnode);
        }


        /* send signal to condition */
        if (err = pthread_cond_signal(&cond)) {
            printf("pthread_cond_signal: %s\n",strerror(err));
            exit(-1);
        }

        if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
                                 sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
            herror("gethostbyaddr");

            exit(-1);
        }
        printf("\nAccepted connection from %s\n", rem->h_name);
    }




//    if(err=pthread_join(tid, &status)) {
//        printf("pthread_join: %s\n",strerror(err));
//        exit(1);
//    }
//    printf("H ergasia oloklirwthike. H katastasi exodou tou nimatos 2 einai %d\n", (int)status);
    Destroy(q);
    pthread_mutex_destroy(&mutex);
    return 0;
}