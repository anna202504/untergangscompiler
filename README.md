# Labor Compilerbau

## Anleitungen

### Flex
1. Flex Datei erstellen: `touch datei.l`
2. Flex festlegen: `flex datei.l`
3. gcc ausführen: `gcc lex.yy.c`
4. Ausführen der Datei: `./a.out verzeichnis/input_datei.pl1`
   
Wichtig: Bei den ersten beiden Schritten wird auf die flex-Datei zugegriffen. Beim 4. Schritt muss eine input-Datei in PL1
eingegeben werden. 
