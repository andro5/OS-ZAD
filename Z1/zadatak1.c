#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

int statusBr = 0;
int zavrsi = 0;

void sigusr1_handler(int sig) {
    printf("\n[INFO] Trenutni broj: %d\n", statusBr);
}

void sigterm_handler(int sig) {
    printf("\n[INFO] SIGTERM primljen, čuvam status i završavam...\n");
    FILE *status = fopen("status.txt", "w");
    if (status) {
        fprintf(status, "%d", statusBr);
        fclose(status);
    }
    exit(0);
}

void sigint_handler(int sig) {
    printf("\n[INFO] SIGINT primljen, prekid bez čuvanja statusa (status.txt ostaje 0).\n");
    exit(0);
}

int ucitaj_status() {
    FILE *status = fopen("status.txt", "r");
    int broj = 0;
    if (status) {
        fscanf(status, "%d", &broj);
        fclose(status);
    }
    return broj;
}

int zadnji_broj_iz_obrade() {
    FILE *obrada = fopen("obrada.txt", "r");
    int broj = 0, temp;
    if (obrada) {
        while (fscanf(obrada, "%d", &temp) == 1)
            broj = temp;
        fclose(obrada);
    }
    return (int)sqrt(broj);
}

void obradi_i_upisi() {
    FILE *obrada = fopen("obrada.txt", "a");
    FILE *status = fopen("status.txt", "w");

    if (!obrada || !status) {
        perror("Greška pri otvaranju datoteka");
        exit(1);
    }

    // upis nule = obrada u toku
    fprintf(status, "0\n");
    fflush(status);

    sleep(3); // simulacija obrade

    statusBr++;
    int kvadrat = statusBr * statusBr;

    fprintf(obrada, "%d\n", kvadrat);
    printf("[INFO] Dodan broj: %d (kvadrat %d)\n", statusBr, kvadrat);

    // upis novog statusa
    freopen("status.txt", "w", status);
    fprintf(status, "%d\n", statusBr);

    fclose(status);
    fclose(obrada);
    sleep(2);
}

// program u beskonacnoj petlji racuna kvadrate prirodnih brojeva, zapisuju se u obrada.txt, i pamti svako trenutno stanje u status.txt kako bi u slucaju prekida mogao nastaviti

int main() {
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);

    statusBr = ucitaj_status();

    if (statusBr == 0) {
        statusBr = zadnji_broj_iz_obrade();
        printf("[INFO] Detektiran prekid ili prvo pokretanje. Nastavljam od %d\n", statusBr);
    } else {
        printf("[INFO] Učitana vrijednost iz status.txt: %d\n", statusBr);
    }

    while (1) {
        obradi_i_upisi();
    }

    return 0;
}

