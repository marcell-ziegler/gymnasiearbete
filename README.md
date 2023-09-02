# gymnasiearbete
Tåg e gött


# Att ladda ned
- [MiKTeX](https://miktex.org/download)
Ladda ned som "Installer" och installera *inte* "for alla users" så behöver ni nog inte admin. Annars ladda ner "Portable Edition" så fixar jag när vi börjar
-  [Strawberry Perl](https://strawberryperl.com/)
Samma tanke som ovan: 64-bit och för endast er om det går utan admin annars portable så fixar vi sen.
- [Python](https://www.python.org/downloads/)
3.11.x i godtyckligt format. Finns också i software center om ni inte kan fixa från hemsidan. Om 3.11 inte finns där funkar alla versioner >3.9
- [Visual Studio Code](https://code.visualstudio.com/)
Finns i software center. Kommer behövas en del extensions, men det tar vi när vi kommer till det.
- [Git](https://git-scm.com/download/win)
64-bit setup. Om det inte går, kör detta i terminalen:
```shell
winget install --source winget Git.Git
```
- Gör ett [GitHub](https://github.com/) konto
	- Hitta min repo: [GitHub - marcell-ziegler/gymnasiearbete: Tåg e gött](https://github.com/marcell-ziegler/gymnasiearbete)
		- Stjärnmarkera den eller spara den på liknande vis för framtida användning. Detta kommer att bli där vi skriver dokumentet.
		- Klona gärna den till godtycklig plats om ni vet hur man gör, låt bli annars.
	- Frivillig bonus: ladda ned [GitHub Desktop](https://desktop.github.com/) om ni vill ha ett bra grafiskt interface för Git, dock finns ett liknande i VS Code så upp till personlig smak. Sedan är det oklart om ni ens kan ladda ner den.
- Frivillig: [Windows Terminal](https://github.com/microsoft/terminal) eller liknande terminalapp som är bättre än default. Just denna kan laddas ned med följande kommando i terminalen ^215e05
```shell
winget install --id Microsoft.WindowsTerminal -e
```

# Att testa
För att se om allt funkar, kör följande bitar kod i terminalen och se om ni får godtyckligt resultat. Använd gärna PowerShell till detta då vanliga `cmd.exe` är sämre. Använd gärna [[#^215e05|Windows Terminal]] till detta och ställ in PowerShell som default. För frågor skriv till mig.
### Python
```shell
python -V
```
borde ge er någonting i stil med (kan vara olika versioner)
```
Python 3.11.0
```

Kör gärna följande för att spara lite tid med att ladda ner paket senare. (kommer nog inte behövas för några månader med sparar tid)
```shell
pip install numpy scipy matplotlib 
```

### LaTeX
```shell
latexmk --version
```
Borde ge er en del text med en rad likt (exakt version och datum irrelevant)
```
Latexmk, John Collins, 4 Apr. 2023. Version 4.80
```

### Git
```shell
git --version
```
borde ge er någonting likt
```
git version 2.33.0.windows.2
```
