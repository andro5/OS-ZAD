#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <pthread.h>
#define SHARED_MEMORY_SIZE (0x6400) // 25 kb memorije

// id segment djeljene memorije, pokazivac na memorijski prostor
int segment_Id;
int *sh_mem;

void brisi(){
	printf("Signal primljen\n");
	shmdt(sh_mem); // odspajanje od memorije
	shmctl(segment_Id, IPC_RMID, NULL); // brisanje segmenta
	exit(0);
}

// dekkerov algoritam - 2 procesa koordiniraju ulazak u kriticni odsjek koristeci flag i turn
// dakle - u programu kreiramo 2 procesa koji pristupaju istoj djeljenoj memoriji, simulira kriticni odsjecak (proces pristupa zajednickom resursu)
void main(){
	segment_Id = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0660 ); // IPC_PRIVATE - privatni kljuc, 0660 - rw prava
	sh_mem = (int*) shmat(segment_Id, NULL, 0);
	while(1){
		if(fork() == 0){ // fork kreira "dijete" proces
			*sh_mem = 1; // sh_mem vrijednost dijeljene memorije
			printf("%d\n", *sh_mem);
			sleep(3);
			printf("%d\n", *sh_mem);
		  	exit(0);
		}

			signal(SIGINT, brisi); // kreira "roditelj" proces
			*sh_mem = 2; // ..
			printf("%d\n", *sh_mem);
			sleep(3);
			printf("%d\n", *sh_mem);
			wait(NULL);

	}
}
