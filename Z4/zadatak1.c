#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

#define N 5   // broj mjesta na vrtuljku
#define M 15  // broj posjetitelja

#define SEMNAME_MJESTA "/mjesta"
#define SEMNAME_START "/start_ride"
#define SEMNAME_END "/end_ride"

sem_t *mjesta, *start_ride, *end_ride;

// za prekidanje programa s SIGINT (ctrl+c)
pid_t main_pid;

void prekini(int sig) {
    printf("\nprekid programa (pid: %d)\n", getpid());

    if (getpid() == main_pid) {
        // samo glavni proces salje signal svima
        kill(0, SIGINT);
        // obrisi semafore
        sem_unlink(SEMNAME_MJESTA);
        sem_unlink(SEMNAME_START);
        sem_unlink(SEMNAME_END);
    }

    exit(0);
}

void posjetitelj(int i) {
    mjesta = sem_open(SEMNAME_MJESTA, 0);
    start_ride = sem_open(SEMNAME_START, 0);
    end_ride = sem_open(SEMNAME_END, 0);
    signal(SIGINT, prekini);

    while (1) {
        sem_wait(mjesta);  // čeka slobodno mjesto
        printf("Posjetitelj %d je ušao na vrtuljak\n", i);

        sem_wait(start_ride);  // čeka početak vožnje
        // vožnja traje...
        sem_wait(end_ride);    // čeka kraj vožnje

        printf("Posjetitelj %d silazi s vrtuljka\n", i);
        sem_post(mjesta);      // oslobađa mjesto
        sleep(1 + rand() % 3); // pauza prije nove vožnje
    }
}

void vrtuljak() {
    mjesta = sem_open(SEMNAME_MJESTA, 0);
    start_ride = sem_open(SEMNAME_START, 0);
    end_ride = sem_open(SEMNAME_END, 0);
    signal(SIGINT, prekini);

    int br_voznji = 1;
    int slobodna;

    while (1) {
        // čekaj da se vrtuljak napuni
        do {
            sem_getvalue(mjesta, &slobodna);
            printf("Trenutno slobodnih mjesta: %d\n", slobodna);
            sleep(1);
        } while (slobodna != 0);

        printf("\n=== Vožnja %d počinje! ===\n", br_voznji);
        // pusti sve posjetitelje da krenu
        for (int i = 0; i < N; i++)
            sem_post(start_ride);

        sleep(3);  // trajanje vožnje

        printf("=== Vožnja %d završava! ===\n", br_voznji++);
        for (int i = 0; i < N; i++)
            sem_post(end_ride);

        sleep(2);  // kratka pauza prije sljedeće vožnje
    }
}

int main() {
    // za prekidanje programa ..
    main_pid = getpid();
    setpgid(0, 0);

    // očisti prethodne semafore ako postoje
    sem_unlink(SEMNAME_MJESTA);
    sem_unlink(SEMNAME_START);
    sem_unlink(SEMNAME_END);

    // kreiraj semafore
    mjesta = sem_open(SEMNAME_MJESTA, O_CREAT | O_EXCL, 0644, N);
    start_ride = sem_open(SEMNAME_START, O_CREAT | O_EXCL, 0644, 0);
    end_ride = sem_open(SEMNAME_END, O_CREAT | O_EXCL, 0644, 0);

    if (mjesta == SEM_FAILED || start_ride == SEM_FAILED || end_ride == SEM_FAILED) {
        perror("Greška pri kreiranju semafora");
        exit(1);
    }

    signal(SIGINT, prekini);  // registriraj handler

    // stvori proces vrtuljka
    if (fork() == 0) {
        vrtuljak();
        exit(0);
    }

    sleep(1); // da vrtuljak krene prvi

    // stvori M posjetitelja
    for (int i = 0; i < M; i++) {
        if (fork() == 0) {
            posjetitelj(i);
            exit(0);
        }
        usleep(100000); // mali razmak između kreiranja
    }

    // čekaj sve child procese
    for (int i = 0; i < M + 1; i++) {
        wait(NULL);
    }

    sem_unlink(SEMNAME_MJESTA);
    sem_unlink(SEMNAME_START);
    sem_unlink(SEMNAME_END);
    return 0;
}
