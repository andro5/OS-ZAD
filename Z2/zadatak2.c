#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

// pokrece 2 procesa koristeci fork
// svaki proces izvrsava f. proc(i, j) gdje je i id procesa a j id drugi proces
// ZASTAVICA (po jedna zastavica za 2 procesa) koja govori moze li proces uci u kriticni dio
// PRAVO pokazuje tko ima prednost ako zele uci istovremeno


int *PRAVO, *ZASTAVICA, pravoId, zastavicaId;

// prvo se poziva ulazak u kriticni odsjek
void udi_u_kriticni_odsjecak(int i, int j){
	ZASTAVICA[i] = 1;
	while(!(ZASTAVICA[j] == 0)){
		if(*PRAVO  == j){
			ZASTAVICA[i] = 0;
			while(*PRAVO == j){
			}
		ZASTAVICA[i] = 1;
		}
	}
}

void izadji_iz_kriticnog_odsjecka(int i, int j){

	ZASTAVICA[i] = 0;
	*PRAVO = j;
}

// i - id procesa (0 ili 1), k - broj pokusaja (svaki proces pokusava 5 puta uci u kriticki odsjek), m - broj koraka unutar kriticnog odsjeka
void proc(int i, int j){
	int k, m;
	for(k = 1; k<5; k++){
		udi_u_kriticni_odsjecak(i,j);
		for(m = 1; m<5; m++){
			printf("i: %d, k: %d, m: %d\n", i, k, m);
			sleep(1);
		}
		izadji_iz_kriticnog_odsjecka(i,j);
	}
}

void brisi(){
	shmdt(PRAVO);
	shmdt(ZASTAVICA);

	shmctl(pravoId, IPC_RMID, NULL);
	shmctl(zastavicaId, IPC_RMID, NULL);

	exit(0);
}

void main(){
        pravoId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660);
        zastavicaId = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0660);
        PRAVO = (int*)shmat(pravoId, NULL, 0);
        ZASTAVICA = (int*)shmat(zastavicaId, NULL, 0);


        if(fork() == 0){
                proc(1,0);
        }if(fork() == 0){

		proc(0,1);

	};
	signal(SIGINT, brisi);
	wait(NULL);
        wait(NULL);
        brisi();
}
