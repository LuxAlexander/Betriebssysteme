#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
 char kdo[100];
 pid_t pid;
 int a = 10;
 int status;

 FILE *fptr = fopen("fork.out", "w");
 if (fptr == NULL) {
  perror("Datei konnte nicht geöffnet werden");
  exit(1);
 }

 for (int i = 1; i < 3; i++) {
  printf("Parent pid is %d\n", getppid());
  printf("ID is %d\n", getpid());
  printf("A: %d %p\n", a, &a);
  fprintf(fptr, "Parent pid is %d\n", getpid());
  pid = fork();
 }

 char *array = malloc(10 * sizeof(char));
 printf("Array: %p\n", array);

 if (pid < 0) {
  perror("fork fehlgeschlagen");
  exit(1);
 }

 if (pid == 0) {
  // Kindprozess
  printf("Ich bin der Kindprozess (PID: %d)\n", getpid());
  pid = fork();
  a += 10;
 } else {
  // Elternprozess
  printf("Ich bin der Elternprozess (PID: %d)\n", getpid());
  a += 4;
 }

 printf("A: %d\n", a);

 fclose(fptr);
 free(array);

 return 0;
}



/*
• Kann eine Applikation mehrfach geforkt werden?
    Ja
• Kann eine Applikation, die durch einen fork() entstanden ist, wieder geforkt werden?
 ja
• Werden bei einem Fork alle Datenelemente (Variablen) geklont oder bestimmte
vielleicht nicht (Wertetypen, Zeiger, Referenzen, Arrays,...)?
 die gleichen Variablen können verwendet werden und änderungen wirken auf alle, getrennte Speicherbereiche
• Welche Speichersegmente einer Applikation werden geklont und welche nicht?
Alle Segmente (Stack, Heap, Daten, Code) werden geklont. Alles andere nicht.

• Gibt es Ressourcen, die sich die Applikationen teilen (können)?
Ja, offene Dateien, Pipes, Shared Mamory und Prozessinfos
Wenn nach dem Forken eine Variable geändert wird: Welche der Prozesse sehen
dann diese Änderung?
 Die Variable ist nur fuer den Eigenenprozess ersichtlich.

• Was passiert, wenn mit malloc(…) ein Speicher allokiert wird:
- Wird dieser Speicher dupliziert?Nein
- Wird der Speicherverbrauch durch zwei Klons dupliziert?Nein, erst wenn beide schreiben.
- Wer ist für einen vor dem Fork allokierten Speicher verantwortlich?
 der Programmierer? jeder Prozess selbstständig.
Wer gibt wann dann den vom Parent allokierten Speicher mit free(…) wieder frei?
sollte von einem Prozess(Elternprozess) am Ende erledigt werden, ansonsten wollen alle frei machen (free(): double free detected in tcache 2)

• Was, wenn nach dem Forken ein Speicher allokiert wird? Wem gehört dieser
Speicher dann? nur dem jeweiligen Prozess

• Was passiert mit geöffneten Dateien: Können dann beide Applikationen
gleichberechtigt diese Dateien verwenden (lesen, schreiben)?
Ja, veränderung sind fuer alle gueltig

• Was passiert, wenn beim Aufruf der Applikation eine Ein-/Ausgabeumleitung
festgelegt wurde? Besteht diese auch gleichermaßen für den Klon?
wirkt für alle Prozesse

• Was muss/kann passieren, damit es zu einem Zombie kommt? Process ist theoretisch fertig, aber es liegen noch Daten vor die abgeholt werden müssen,
Elternprozess noch nicht wait() aufgerufen
Ist ein Zombie die Folge eines Fehlers? Nein, außer er wird vergessen und nicht aufgeloest.

*/