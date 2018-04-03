#include <stdio.h>
#include <stdlib.h>

struct sizeandNumber{
	int size;
	int number;
}val;
void main(){
	int i, n, startingAddress, finalAddress, totalFragmentation=0;
	int *request;

	printf("\n Enter size of Partitions: ");
	scanf("%d",&val.size);

	printf("\n Enter number of Partitions: ");
	scanf("%d",&val.number);

	printf("\n Enter number of requests: ");
	x: scanf("%d",&n);
	
	if(n > val.number){
		printf("\n Enter value less than %d", val.number);
		goto x;
	}
	
	request = (int *) malloc(n*sizeof(int));

	printf("\n Input the memory requests");
	for(i=0;i<n;i++){
		printf("\n Memory request %d = ", i+1);
		scanf("%d", &request[i]);
	}

	for(i=0;i<n;i++){
		
		startingAddress = i * val.size;
		finalAddress = (i*val.size) + request[i]-1;
		totalFragmentation += val.size-request[i];
		
		printf("\n Request %d Starting Address = %d", i+1, startingAddress);
		printf("\n Request %d final Address = %d", i, finalAddress);

	}
	printf("\n Total Fragmentation = %d \n", totalFragmentation);

}

