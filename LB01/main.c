#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define Version 1
/* ============================================================================
 * Name : just.c
 * Author : Alexander Lux
 * Version : 1.0
 * Description :

 * Wenn ein DATEINAME angegeben wird, dann wird die angegebene Datei gelesen. Wenn
 * kein Dateiname oder alternativ als Platzhalter ein Zeichen “-“ angegeben wird, dann
 * wird der Eingabestrom aus stdin gelesen.

 * -s firstline Beginnt die Ausgabe mit der firstline gelesenen Zeile;
 * default ist 1 für Ausgabe beginnt ab der 1. Zeile

 * -e lastline Beendet die Ausgabe mit der lastline ausgegebenen Zeile;
 * wenn keine Angabe, dann wird bis zur letzten Zeile ausgegeben

 * -w pagewidth Gibt die Breite der Seite in Zeichen an; default ist 72

 * -m margin Einrückung jeder Zeile um margin Leerzeichen; margin ist nicht
 * Teil der Seitenbreite; default ist 0 für keine Einrückung

 * -j Die Ausgabe erfolgt in Blocksatz; alle Zeilen sind gleich lang,
 * Leerräume werden hierfür nötigenfalls mit Leerzeichen aufgefüllt

 * -n Entfernt etwaige Newline (bei Windows CR+LF oder Linux LF) aus
 * dem Texteingabestrom

 * -p Interpretiert ein doppeltes NewLine als Absatzzeichen

 * -h, --help Gibt eine erweiterte Hilfeinformation aus und terminiert

 * --version Gibt eine Versionsinformation aus und terminiert
 *
 * Error Meldung in stderr fprintf(stderr,"Error");


 * ============================================================================ */

int validFile(const char *filename) {
    const char *valid[] = {".txt", ".csv", ".log", ".json", ".xml", ".html", ".htm",
                                      ".ini", ".md", ".yml", ".yaml", ".c", ".h", ".cpp", ".py",
                                      ".java", ".js", ".css", ".php", ".sh", ".bat"};
    size_t len = strlen(filename);
    if (len < 3) return 0; // Too short to be a valid file name

    // Find the last occurrence of a dot (.)
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return 0; // No dot found or filename starts with a dot

    // Compare the extension with known valid endings
    for (size_t i = 0; i < sizeof(valid) / sizeof(valid[0]); i++) {
        if (strcmp(dot, valid[i]) == 0) {
            return 1; // Valid file
        }
    }
    return 0; // Not a valid file
}

int main(int argc, char* args[]) {
    int firstline=1,lastline=EOF,pagewidth=72,margin=0;
    char *filename;
    bool blocksatz=false,newline=true,dopplenl=false,help=false,version=false;

    for(int i=1;i<argc;i++) {

        //printf("%s\n",args[i]);

        if (strcmp(args[i],"--help")==0 || strcmp(args[i],"-h")==0) {
            help=true;
        }//Get Input Arguments and set the parameters
        else if (strcmp(args[i],"--version")==0) {
            version=true;
        }else if (strcmp(args[i],"-p")==0) {
            dopplenl=true;
        }else if (strcmp(args[i],"-n")==0) {
            newline=true;
        }else if (strcmp(args[i],"-j")==0) {
            blocksatz=true;
        }else if (strcmp(args[i],"-m")==0) {
            //GET THE MARGIN
            margin=atoi(args[++i]);
            fprintf(stdout,"margin=%d\n",margin);
        }else if (strcmp(args[i],"-w")==0) {
            //GET PAGEWIDTH
            pagewidth=atoi(args[++i]);
            fprintf(stdout,"pagewidth=%d\n",pagewidth);
        }else if (strcmp(args[i],"-e")==0) {
            //GET LASTLINE
            lastline=atoi(args[++i]);
            fprintf(stdout,"lastline=%d\n",lastline);
        }else if (strcmp(args[i],"-s")==0) {
            //GET FIRSTLINE
            firstline=atoi(args[++i]);
            fprintf(stdout,"firstline=%d\n",firstline);
        }

    }

    //Could use this Function to check if there is a file, need more substring checks or high fault tolerance
    if (strstr(args[argc-1],".")!=NULL) {
        //\xe2\x80\xa9\n might want to use this as a Absatz zeichen.
        //might need to install fonts that support paragraph sign sudo apt install fonts-noto
        //might just print a "¶" instead
        fprintf(stdout,"File might be %s ¶",args[argc-1]);
    }

    //Common file types could be txt,csv,log,json,xml,html,yaml
    if (validFile(args[argc-1])==1) {
        fprintf(stdout,"\nFound file %s\n",args[argc-1]);
    }else fprintf(stderr,"No Valid File Specified\n");

    if (version==true||help==true) {
        if (version==true) printf("Version: %d", Version);//Give the User a Helpful Help

        if (help==true) {

            FILE *fptr=NULL;
            char str[100]=" ";
            fptr=fopen("help.txt","r");//Honestly pretty dumb since the file needs to be in the same or a specified folder
            //might want to make an option if the file wasn't found give a smaller version out or create the file.
            if (fptr==NULL) {
                fprintf(stderr,"Error opening file\n");
                exit(1);
            }

            while (fgets(str, 64, fptr)!=NULL) {
                fprintf(stdout,"%s\n",str);//Give the User a Helpful Help
            }

            fclose(fptr);
        }

        return 0;
    }

    return 0;
}