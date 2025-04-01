# Betriebssysteme
 betriebssysteme Aufgabe 01


Aufruf der Applikation:
just [OPTION]... [DATEINAME]
Wenn ein DATEINAME angegeben wird, dann wird die angegebene Datei gelesen. Wenn
kein Dateiname oder alternativ als Platzhalter ein Zeichen “-“ angegeben wird, dann
wird der Eingabestrom aus stdin gelesen.
-s firstline Beginnt die Ausgabe mit der firstline gelesenen Zeile;
default ist 1 für Ausgabe beginnt ab der 1. Zeile
-e lastline Beendet die Ausgabe mit der lastline ausgegebenen Zeile;
wenn keine Angabe, dann wird bis zur letzten Zeile ausgegeben
-w pagewidth Gibt die Breite der Seite in Zeichen an; default ist 72
-m margin Einrückung jeder Zeile um margin Leerzeichen; margin ist nicht
Teil der Seitenbreite; default ist 0 für keine Einrückung
-j Die Ausgabe erfolgt in Blocksatz; alle Zeilen sind gleich lang,
Leerräume werden hierfür nötigenfalls mit Leerzeichen aufgefüllt
-n Entfernt etwaige Newline (bei Windows CR+LF oder Linux LF) aus
dem Texteingabestrom
-p Interpretiert ein doppeltes NewLine als Absatzzeichen
-h, --help Gibt eine erweiterte Hilfeinformation aus und terminiert
--version Gibt eine Versionsinformation aus und terminiert
Achtung: Etwaige Fehler dürfen grundsätzlich nur auf stderr ausgegeben werden!


Verwendung:

Das Programm kann entweder über eine IDE (z. B. CLion, Code::Blocks) oder direkt über die Kommandozeile ausgeführt werden. Dazu wird ein C-Compiler benötigt.

    Windows: Installation über einen Paketmanager wie MinGW.

    Unix/Linux: Installation über das Terminal mit:

    sudo apt install gcc

    (Hinweis: Die gewünschte GCC-Version kann hier direkt angegeben werden.)

Weitere Informationen zur Installation von GCC unter Ubuntu:
🔗 Install GCC on Ubuntu: https://phoenixnap.com/kb/install-gcc-ubuntu

Für eine Unix-Umgebung unter Windows kann das Windows Subsystem for Linux (WSL) verwendet werden:
🔗 WSL-Installation: https://learn.microsoft.com/de-de/windows/wsl/install

Empfohlene Entwicklungsumgebung:

    Code::Blocks (Download & Infos): https://www.codeblocks.org/

Optionen & Argumente:

Das Programm bietet verschiedene Optionen zur Textmanipulation. Diese können in beliebiger Reihenfolge angegeben werden. Falls eine Datei verarbeitet werden soll, sollte sie am Ende der Befehlszeile stehen.
Andernfalls wird die standard Eingabe verwendet.