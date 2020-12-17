echo off
Rem Es werden ab dem aktuellen Verzeichnis alle Dateien geloescht  
Rem die wieder automatisch durch Visual Studio erzeugt werden können,

Rem ACHTUNG: Sie muessen sich sicher sein, dass Sie wirklich die Dateien loeschen wollen

Rem
Rem Aenderungen:
Rem HHe: 02.03.03:  auch bmt, cod, lot und lof-Dateien werden gelöscht
Rem HHe: 04.03.03:  pdf-Dateien der Unterverzeichnisse dürfen nicht gelöscht werden
Rem HHe: 26.09.03:  htm, suo wird gelöscht
Rem HHe: 29.11.03:  processidx.exe wird nicht mehr geloescht
Rem HHe: 02.01.04:  *.suo-Dateien werden nun auch geloescht, ging bisher nicht, weil Dateien versteckt sind
Rem HHe: 29.11.05:  Datei mit Endung class werden gelöscht
Rem HHe: 23.02.06:  Datei mit Endung .java~ und cpp~ und c~ werden gelöscht
Rem HHe: 05.06.06:  Datei mit Endung .sbr ergänzt
Rem HHe: 11.08.06:  Löschen aller Dateien wg. Umstellung nach Visual Studio 8.0
Rem HHe: 10.12.06:  processidx.exe wird zuvor gerettet, bevor alle exe gelöscht werden
Rem HHe: 27.01.07:  addHeadLineToIndex.exe wird zuvor gerettet, bevor alle exe gelöscht werden
Rem HHe: 07.04.07:  exe in auflage werden gerettet
REM HHe: 07.04.07: *fl1x* wird gelöscht
REM HHe: 15.10.20:  LaTeX wird nicht mehr beachtet, nur noch VS 2017 und VS 2019


echo ACHTUNG alle von Visual Studio automatisch erzeugbaren Datei werden geloescht
echo Bei Druecken einer Taste beginnt das Loeschen!!!!
pause

del/s *.exe
del/s *.obj
del/s *.ilk
del/s *.pch
del/s *.pdb
del/s *.ncb
del/s *.opt
del/s *.plg
del/s *.idb
del/s *.bak


Rem Loeschen wegen dotNet
del/s *.htm
del/s/a:h *.suo
del/s *.sbr


Rem Von Visual 8.0 erzeugt
del/s *.bsc
del/s UpgradeLog.XML
del/s *.vcproj.H*.*.*
del/s *.vcproj.DLR.f*.*
del/s *.*.user

del/s mt.dep

del/s *.sdf
del/s *.*.old
del/s *.old


del/s *.*.user
del/s *.lib

Rem Von Visual 10.0 erzeugt
del/s *.ipch
del/s *.tlog
del/s *.*.tlog
del/s *.lastbuildstate
del/s *manifest.rc


Rem Von Visual Studio 2015 erzeugt
del/s *.db
del/s *.vs


