# Test Report

## Introduktion
Under projektet har vi försökt hålla oss till TDD(Test driven development) för att minska utrymmet för buggar och från början kunna vara säkra på att våra funktioner fungerar som vi vill. Vi har skrivit tre olika typer av test. Enhetstester, Integrationstester och Regressionstester. 

## Enhetstestning

Vi har skrivit enhetstester för alla viktiga funktioner i projektet i en testfil **test.c**. Vi arbetade enligt TDD vilket har gjort att vi har testat vår kod hela tiden under utväcklingen. Varje modul har testfall som testar dess funktionalitet och dessutom edge cases och felhantering.

## Coverage

Vi har undersökt våra testers coverage med hjälp av **gcov** och här är våra resultat:

??

Vår coverage uppnår inte 100% eftersom att vi inte kan testa vår shutdown() funktion. Anledningen är att den är svår att simulera i testmiljön eftersom att den endast ska anropas en gång, vilket är när programmet avslutas. Men kodgranskning och manuell testning har visat att funktionen är korrekt implementerad. Den ger inga minnesläckor och därmed fungerar den som vi vill. Vi har under utvecklingen av funktionen testat både den och dess inre funktioner och kan se att de friar för mycket minne, vilket är det som ska hända om den körs fler gånger.



## Integrationstestning

Vi har skapat integrationstester för att verifiera att olika moduler fungerar tillsammans som förväntat. Dessa tester kontrollerar bland annat:

Kommunikation mellan backend och databasen.

API-anrop och svar.

Flödet mellan användargränssnitt och backend.

Integrationstester körs automatiskt vid varje incheckning till GitHub via GitHub Actions.

## Regressionstestning

Regressionstester har implementerats för att säkerställa att ändringar i koden inte introducerar nya fel. Dessa tester:

Valdes baserat på tidigare funna buggar och kritiska funktioner.

Visar att koden är stabil efter uppdateringar.

Val av tester

Testerna valdes genom analys av:

Kritiska användarflöden.

Funktioner som tidigare haft buggar.

Komponenter med hög förändringsfrekvens.

## Automatisering

Vi har automatiserat våra tester med GitHub Actions. Detta gör att de körs vid varje pull request och varje commit till main. Fördelar med detta är att våra testresultat loggas direkt i GitHub och alla i gruppen får ett main med en sammanfattning av resultaten. 

## De sex otäckaste buggarna

Här är de sex allvarligaste buggarna som vi har stött på under projektets gång, samt länk till deras github-issues:

#### Bug 1 - free_scheduled_tasks kallas rekursivt: https://github.com/IOOPM-UU/Oboy/issues/43 

#### Bug 2 - Stack overflow

#### Bug 3 - https://github.com/IOOPM-UU/Oboy/issues/42 

#### Bug 4 - ?

#### Bug 5 - ?

#### Bug 6 - Tester i main https://github.com/IOOPM-UU/Oboy/issues/19

## Slutsats

Blabla