#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define Version 1
/* ============================================================================
 * Name : just.c
 * Author : Alexander Lux, Dominik Zach
 * Version : 1.0
 * Description :

 * Aufruf: just [OPTION]... [DATEINAME]
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


int validFile(const char*);//Tried to make it like regex, to pattern check for valid file endings

void printMargin(const char *, int);//print the margin

void printFile(FILE*,int,int,int,int,bool,bool,bool);//print File with the arguments

void justifysmall(char *, int, int, int);//help function to align text
void justifybig(char *, int, int, int);//help function to align text

int main(int argc, char* args[]) {
    int firstline=1,lastline=EOF,pagewidth=72,margin=0;
    char *filename;
    bool blocksatz=false,nnewline=false,dopplenl=false,help=false,version=false;

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
            nnewline=true;
        }else if (strcmp(args[i],"-j")==0) {
            blocksatz=true;
        }else if (strcmp(args[i],"-m")==0) {
            //GET THE MARGIN
            margin=atoi(args[++i]);
            //fprintf(stdout,"margin=%d\n",margin);
        }else if (strcmp(args[i],"-w")==0) {
            //GET PAGEWIDTH
            pagewidth=atoi(args[++i]);
            //fprintf(stdout,"pagewidth=%d\n",pagewidth);
        }else if (strcmp(args[i],"-e")==0) {
            //GET LASTLINE
            lastline=atoi(args[++i]);
            //fprintf(stdout,"lastline=%d\n",lastline);
        }else if (strcmp(args[i],"-s")==0) {
            //GET FIRSTLINE
            firstline=atoi(args[++i]);
            //fprintf(stdout,"firstline=%d\n",firstline);
        }
    }

    //Common file types could be txt,csv,log,json,xml,html,yaml
    if (validFile(args[argc-1])==1) {
        //fprintf(stdout,"\nFound file %s\n",args[argc-1]);
        FILE *file=fopen(args[argc-1],"r");
        printFile(file,firstline,lastline,pagewidth,margin,nnewline,dopplenl,blocksatz);
    }else {
        printFile(stdin,firstline,lastline,pagewidth,margin,nnewline,dopplenl,blocksatz);
    }

    //Personal Preference, to work with the file and print help/version
    if (version==true||help==true) {
        if (version==true) printf("Version: %d", Version);

        if (help==true) {

            FILE *fptr=NULL;
            char str[100]=" ";
            fptr=fopen("help.txt","r");//Honestly pretty dumb since the file needs to be in the same or a specified folder. Like to work with files
            //might want to make an option if the file wasn't found give a smaller version out or create the file.
            if (fptr==NULL) {
                fprintf(stderr,"No help File. Consider downloading it\n");
                fprintf(stdout,"-s x,ab wann soll die asugabe beginnen\n-e x, wann soll sie beenden\n-w x,wie viele zeichen pro zeile\n-m x, einrueckung\n-j,blocksatz\n-n,keine newlines\n-p, doppelt newline ist eine Absatz\n-h,--help:hilfe\n--version:version");
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

void printFile(FILE* fptr, int start, int end, int maxwidth, int margin, bool nonewline, bool paragraph, bool blocksatz) {
    char *word = malloc(128);  // Start with a small buffer
    char *line = malloc(maxwidth + 50);  // Line buffer
    int countlines = 1, countwidth = 0, wordlen = 0, linepos = 0;
    bool newline = false;
    char ch;

    if (fptr == NULL) {
        fprintf(stderr, "File can't be opened.\n");//Shouldn't happen since files are checked beforehand or stdin is used
        exit(-1);
    }

    //Go to the end of the file or till the specified end is reached
    while ((ch = fgetc(fptr)) != EOF && (countlines <= end || end == EOF)) {
        if (ch=='\n') {
            countlines++;
        }
        if (ch == '\n') {
            if (newline) {  // Double newline detected
                if (paragraph && countlines >= start) {
                    if (countwidth > 0) {  // Print the current line before marking a paragraph
                        if (blocksatz) {
                            if (maxwidth <=10) {
                                justifysmall(line, countwidth, maxwidth, margin);
                            }justifybig(line, countwidth, maxwidth, margin);
                        }
                        else {
                            printMargin(line, margin);
                            printf("\n");
                        }
                        //countlines++;
                        countwidth = 0;
                        linepos = 0;
                        line[0] = '\0';
                    }
                    printf("¶\n");
                }
                newline = false;
                continue;//delete new line and continue loop
            }
            newline = true;
        } else {
            newline = false;
        }

        // Resize word buffer if necessary
        if (wordlen >= 127) {
            word = realloc(word, wordlen + 50);
            if (!word) {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // Build words
        if (ch != ' ' && ch != '\n') {
            if (countlines >= start) {
                word[wordlen++] = ch;//add character to word
                word[wordlen] = '\0';  // Always terminate string
            }
        } else {
            if (wordlen > 0) {  // Word complete
                // Check if line is full and must be printed before adding new word
                if (countwidth + wordlen >= maxwidth) {

                        if (blocksatz) {
                            if (maxwidth <=10) {
                                justifysmall(line, countwidth, maxwidth, margin);
                            }justifybig(line, countwidth, maxwidth, margin);
                        }
                        else {
                            printMargin(line, margin);
                            printf("\n");
                        }

                    countwidth = 0;//RESET
                    line[0] = '\0';
                    linepos = 0;

                }


                // Add word to the line
                if (countwidth > 0) {
                    line[linepos++] = ' ';
                    countwidth++;
                }

                strcpy(&line[linepos], word);
                linepos += wordlen;
                countwidth += wordlen;
                wordlen = 0;  // Reset for next word
            }
        }
    }

    // Refactor
    if (wordlen > 0 && countlines >= start && (countlines < end || countlines == EOF)) {
        printf("%d",end);
            if (countlines >= start) {
                if (blocksatz) {
                    if (maxwidth <=10) {
                        justifysmall(line, countwidth, maxwidth, margin);
                    }justifybig(line, countwidth, maxwidth, margin);
                }
                else {
                    printMargin(line, margin);
                    printf("\n");
                }
            }
            countlines++;
            countwidth = 0;
            linepos = 0;
            line[0] = '\0';


        strcpy(&line[linepos], word);
        linepos += wordlen;
        countwidth += wordlen;
    }

    // Redundant code that should be simplified at the next refactoring
    if (countwidth > 0&&(countlines<end||end==EOF)) {
        if (blocksatz) {
            if (maxwidth <=10) {
                justifysmall(line, countwidth, maxwidth, margin);
            }justifybig(line, countwidth, maxwidth, margin);
        }else {
            printMargin(line, margin);
            printf("\n");
        }
    }

    free(word);
    free(line);
}


void printMargin(const char *text, int margin) {
    for (int i = 0; i < margin; i++) {
        putchar(' ');
    }
    printf("%s", text);
}


int validFile(const char *filename) {
    const char *valid[] = {".txt", ".csv", ".log", ".json", ".xml", ".html", ".htm",
                                      ".ini", ".md", ".yml", ".yaml", ".c", ".h", ".cpp", ".py",
                                      ".java", ".js", ".css", ".php", ".sh", ".bat"};
    int len = strlen(filename);
    if (len < 3) return 0; // Too short to be a valid file name

    // Find the last occurrence of a dot (.)
    char *dot = strrchr(filename, '.');//Returns a pointer to the last occurrence of character, finds the dot
    if (!dot || dot == filename) return 0; // No dot found or there is no specificater afterwards like help.

    // Compare the input with known valid endings
    for (int i = 0; i < sizeof(valid) / sizeof(valid[0]); i++) {
        if (strcmp(dot, valid[i]) == 0) {
            return 1; // Valid file
        }
    }
    fprintf(stderr, "Wrong File-ending, stdin will be used.\n");
    return 0; // Not a valid file
}

void justifybig(char *line, int lineLen, int maxwidth, int margin) {
    int spacesneeded = maxwidth - lineLen;
    int wordCount = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ' ') wordCount++;
    }
    if (wordCount == 0) {
        return;
    }//Prevent division by zero error

    int spacescount = spacesneeded / wordCount;
    int extraspaces = spacesneeded % wordCount;


    printMargin("", margin);  // Print margin

    for (int i = 0; line[i] != '\0'; i++) {
        putchar(line[i]);
        if (line[i] == ' ') {
            for (int j = 0; j < spacescount; j++) putchar(' ');
            if (extraspaces > 0) {
                putchar(' ');
                extraspaces--;
            }
        }
    }
    printf("\n");
}

void justifysmall(char *line, int lineLen, int maxwidth, int margin) {
    if (maxwidth <= 0) return;  // Prevent infinite loop in case of incorrect width

    int index = 0;

    while (index < lineLen) {
        printMargin("", margin);  // Print margin before each new line

        // Print up to maxwidth characters
        for (int i = 0; i < maxwidth && index < lineLen; i++, index++) {
            putchar(line[index]);
        }

        printf("\n");  // Move to next line
    }
}

