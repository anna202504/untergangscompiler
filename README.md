# Labor Compilerbau

## Anleitungen

### Flex
1. Flex Datei erstellen: 'touch datei.l'
2. Flex festlegen: 'flex datei.l'
3. gcc ausführen: 'gcc lex.yyy.c'
4. Ausführen der Datei: './a.out input_datei.pl1' / './pl1c Input_folder/complex_in.pl1'


Wichtig: Bei den ersten beiden Schritten wird auf die flex-Datei zugegriffen. Beim 4. Schritt muss eine input-Datei in PL1
eingegeben werden. 

Datei von Output erstellen:

pl1c Input_folder/complex_in.pl1 > out.pl1