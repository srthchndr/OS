#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/* Structure for holding clients memory request, job name and private fifo details*/
struct client{
	int memoryRequest;
	char jobName[10];
	char privateFIFO[10];
}ref;

/*Structure for holding process details*/
struct processDetails{
	int startingAdd;
	int endingAdd;
	int numFrames;
	int fragmentation;
	char error[30];
}buff;

void main(){
	int fda;	//To Write to server
	int fdb;	//To read from server
	int clientID;	
	int startingAdd, endingAdd, numFrames, fragmentation;
	int i = 0;

	printf("\n Enter a Job name: ");
	scanf("%s", &ref.jobName);		//Name of the Job

	printf("\n Enter amount of memory to be requested: ");
	scanf("%d", &ref.memoryRequest);	//size of memory request

	clientID = getpid();
	sprintf(ref.privateFIFO, "FIFO_%d", clientID);	//assigning name for private FIFO

	if((mkfifo(ref.privateFIFO, 0666) < 0 && errno != EEXIST)){	//Creating private FIFO
		perror("Cant create Private fifo");
		exit(-1);
	}

	if((fda = open("FIFO_to_server", O_WRONLY)) < 0)	//opening common FIFO
		printf("\n Cant open Fifo to Write");

	write(fda, &ref, sizeof(ref));				//writing to the server
			
	close(fda);							//Closing common FIFO
	
	if((fdb = open(ref.privateFIFO, O_RDONLY)) < 0)			//Opening private FIFO
		printf("\n Cant open private fifo to read");

	read(fdb, &buff, sizeof(buff));					//Reading from the server

	if(buff.error[0] != 0){						//Checking any error message
		printf("\n %s \n", buff.error);
		exit(-1);
	}
	
	/*Printing process details*/
	printf("\n Starting Address: %d", buff.startingAdd);	
	printf("\n Ending Address: %d", buff.endingAdd);
	printf("\n Number of Frames: %d", buff.numFrames);
	printf("\n Fragmentation: %d \n", buff.fragmentation);
	
	close(fdb);							//Closing private FIFO
	unlink(ref.privateFIFO);				//Unlinking private FIFO
}


