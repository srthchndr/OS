#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Structure for holding clients memory request, job name and private fifo details*/
struct server{
  int clientBurst;
  char privateFIFO[10];
}buf;

typedef struct pcb {  /*structure for PCBs */

	char name[12];
    char privateFIFO[10];
	int burst;                    

} PCB;

typedef struct node{  /*Nodes stored in the linked list*/

	struct pcb elements;

	struct node *next;

} Node;

typedef struct queue{ /*A struct facilitates passing a queue as an argument*/

	Node *head;       /*Pointer to the first node holding the queue's data*/

	Node *tail;       /*Pointer to the last node holding the queue's data*/

	int sz;           /*Number of nodes in the queue*/

} Queue;



int size( Queue *Q ){

	return Q->sz;

}



int isEmpty( Queue *Q ){

	if( Q->sz == 0 ) return 1;

	return 0;

}



void enqueue( Queue *Q, struct pcb elements){

	Node *v = (Node*)malloc(sizeof(Node));/*Allocate memory for the Node*/

	if( !v ){

		printf("ERROR: Insufficient memory\n");

		return;

	}

	v->elements = elements;

	v->next = NULL;

	if( isEmpty(Q) ) Q->head = v;

	else Q->tail->next = v;

	Q->tail = v;

	Q->sz++;

}



PCB dequeue( Queue *Q ){

	PCB temp;
	Node *oldHead;



	if( isEmpty(Q) ){

		printf("ERROR: Queue is empty\n");

		return temp;

	}

	oldHead = Q->head;

	temp = Q->head->elements;

	Q->head = Q->head->next;

	free(oldHead);

	Q->sz--;

	return temp;

}



PCB first( Queue *Q ){

	PCB temp;
	if( isEmpty(Q) ){

		printf("ERROR: Queue is empty\n");

		return temp;

	}
	return(Q->head->elements);
}



void destroyQueue( Queue *Q ){

	while( !isEmpty(Q) ) dequeue(Q);

}



/*A different visit function must be used for each different datatype.*/

/*The name of the appropriate visit function is passed as an argument */

/*to traverseQueue.                                                   */

void visitNode( PCB elements ){

	printf("PCB name and CPU burst:");
	printf("%s \t",elements.name);
	printf("%d", elements.burst);
	printf("\n");

}



/*The following function isn't part of the Queue ADT, however*/

/*it can be useful for debugging.                            */

void traverseQueue( Queue *Q ){

	Node *current = Q->head;

	while( current ){

		visitNode(current->elements);

		current = current->next;

	}

	printf("\n");

}

void main(){
	int fda;	//To read from client
	int fdb;	// to write to client
	int finish;	
	int timeQuantum, numClients, clock = 0;
	int i;
	char firstNode[3];
	PCB temp;

	printf("\n How many number of clients: ");	
	scanf("%d", &numClients);			//Number of clients

    printf("\n Enter the size of Time quantum: ");
    scanf("%d", &timeQuantum);          //Size of time quantum

    Queue Q;
    Q.head = NULL;
    Q.tail = NULL;
    Q.sz = 0;

	if((mkfifo("FIFO_to_server", 0666) < 0 && errno != EEXIST)){	//Creating common FIFO
		perror("Cant create FIFO_to_server");
		exit(-1);
	}

	for(i=0; i<numClients; i++){
	
	if((fda = open("FIFO_to_server", O_RDONLY)) < 0)	//Opening common FIFO
		printf("Cant open fifo to read");

	finish = read(fda, &buf, sizeof(buf));			//Reading from client

    PCB pcb[i];
    sprintf(pcb[i].name, "p%d", i);
    pcb[i].burst = buf.clientBurst;
    //pcb[i].privateFIFO = buf.privateFIFO;
    strcpy(pcb[i].privateFIFO, buf.privateFIFO);
    enqueue(&Q, pcb[i]);
	}
	while(1){
		traverseQueue(&Q);
		temp = Q.head->elements;
		for(i=0; i<timeQuantum; i++){
			clock++;
			Q.head->elements.burst--;
			if(Q.head->elements.burst == 0){
						if((fdb = open(buf.privateFIFO, O_WRONLY)) < 0)			//Opening private FIFO
						printf("Cant open private fifo to write");

						write(fdb, &clock, sizeof(clock));				//Writing to client private fifo

						if(finish < 0)
							printf("\n Server: error");
						close(fdb);	
                        break;						//Closing private FIFO
			}

	}
			if(size(&Q) == 0){
					break;
				}
				else
					continue;
			dequeue(&Q);
			enqueue(&Q, temp);

	}
    close(fda);							//Closing common FIFO
    printf("\n Queue Has: \n");
    traverseQueue(&Q);

	unlink("FIFO_to_server");					//Unlinking common FIFO
}

