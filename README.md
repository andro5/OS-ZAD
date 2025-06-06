# Operacijski sustavi Zadaće '25 FSRE
Andrej Kesten 2299/RR (www.github.com/andro5)

## Opis
20.5.'25
##
-> Zad 1.
Neka program simulira neki dugotrajni posao (slično servisima) koji koristi dvije datoteke: 
u jednu dodaje do sada izračunate vrijednosti (npr. kvadrati slijednih brojeva), a u drugu 
podatak do kuda je stigao. Npr. u obrada.txt zapisuje 1 4 9 … (svaki broj u novi red) a u 
status.txt informaciju o tome gdje je stao ili kako nastaviti. Npr. ako je zadnji broj u 
obrada.txt 100 u status.txt treba zapisati 10 tako da u idućem pokretanju može nastaviti 
raditi i dodavati brojeve.
 Prije pokretanja te je datoteke potrebno ručno napraviti i inicijalizirati. Početne vrijednosti 
mogu biti iste – broj 1 u obje datoteke.
 Pri pokretanju programa on bi trebao otvoriti obje datoteke, iz status.txt, pročitati tamo 
zapisanu vrijednost. Ako je ona veća od nule program nastavlja s proračunom s prvom 
idućom vrijednošću i izračunate vrijednosti nadodaje u obrada.txt. Prije nastavka rada u 
status.txt upisuje 0 umjesto prijašnjeg broja, što treba označavati da je obrada u tijeku, da 
program radi.
 Na signal (npr. SIGUSR1) program treba ispisati trenutni broj koji koristi u obradi. Na 
signal SIGTERM otvoriti status.txt i tamo zapisati zadnji broj (umjesto nule koja je tamo) 
te završiti s radom.
 Na SIGINT samo prekinuti s radom, čime će u status.txt ostati nula (čak se taj signal niti 
ne mora maskirati – prekid je pretpostavljeno ponašanje!). To će uzrokovati da iduće 
pokretanje detektira prekid – nula u status.txt, te će za nastavak rada, tj. Određivanje 
idućeg broja morati „analizirati“ datoteku obrada.txt i od tamo zaključiti kako nastaviti 
(pročitati zadnji broj i izračunati korijen). Operacije s datotekama, radi jednostavnosti, 
uvijek mogu biti u nizu open+fscanf/fprintf+close, tj. ne držati datoteke otvorenima da se 
izbjegnu neki drugi problemi. Ali ne mora se tako. U obradu dodati odgodu (npr. sleep(5)) 
da se uspori izvođenje.
##
-> Zad 2.
Dekkerov postupak međusobnog isključivanja
 Ostvariti sustav paralelnih procesa/dretvi. Struktura
 procesa/dretvi dana je sljedećim pseudokodom:
 proces
 za
 proc(i)         
k = 1 do
 /* i [0..n-1] */   
5 čini
 uđi u kritični odsječak
 za
 m= 1 do
 5 čini
 ispiši (i, k, m)      
izađi iz kritičnog odsječka
 kraj
##
 26.5.'25
 ##
 -> Zad 3.
 Ostvariti program koji simulira tijek rezervacije stolova u nekom
 restoranu. Program na početku treba stvoriti određeni broj dretvi koji
 se zadaje u naredbenom retku. Svaka dretva/proces nakon isteka
 jedne sekunde provjerava ima li slobodnih stolova te slučajno
 odabire jedan od njih. Nakon odabira dretva ulazi u kritični odsječak
 te ponovo provjerava je li odabrani stol slobodan. Ako jest, označava
 stol zauzetim i izlazi iz kritičnog odsječka. U oba slučaja, nakon
 obavljene operacije ispisuje trenutno stanje svih stolova te podatke o
 obavljenoj rezervaciji. Prilikom ispisa za svaki stol mora biti vidljivo
 je li slobodan ili broj dretve/procesa koja je taj stol rezervirala. Broj
 stolova se također zadaje u naredbenom retku. Svaka dretva
 ponavlja isti postupak sve dok više nema slobodnih stolova.
 Program završava kada sve dretve završe.
 ##
 2.6.'25
 ##
 -> Zad 4.
Modelirati
 vrtuljak
 (ringišpil)
 s
 dva
 tipa
 dretvi/procesa:
 dretvama/procesima posjetitelj (koje predstavljaju posjetitelje koji žele na
 vožnju) te dretvom/procesom vrtuljak. Dretvama/procesima posjetitelj se ne
 smije dozvoliti ukrcati na vrtuljak kada više nema praznih mjesta (kojih je
 ukupno N) te prije nego li svi prethodni posjetitelji siđu. Vrtuljak se može
 pokrenuti tek kada je pun. Za sinkronizaciju koristiti opće semafore i dodatne
 varijable
 ##
 -> Zad 5.
  S pomoću više dretvi riješiti problem pet filozofa koristeći
 koncept monitora. Pri svakoj promjeni program mora
 vizualno prikazati za sve filozofe što oni rade. Npr. kada
 filozof 4 ide jesti, tada treba ispis izgledati otprilike ovako:
 "Stanje filozofa: X o O X o" (X-jede, O-razmišlja, o-čeka na
 vilice).
 Problem pet filozofa. Filozofi obavljaju samo dvije različite
 aktivnosti: misle ili jedu. To rade na poseban način. Na
 jednom okruglom stolu nalazi se pet tanjura te pet štapića
 (između svaka dva tanjura po jedan). Filozof prilazi stolu,
 uzima lijevi štapić, pa desni te jede. Zatim vraća štapiće na
 stol i odlazi misliti.


## Instalacija

```bash
git clone https://github.com/andro5/OS-ZAD.git
cd OS-ZAD
