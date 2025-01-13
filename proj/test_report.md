# Test Report

## Introduktion
Under projektet har vi försökt hålla oss till arbetssättet **TDD** (Test driven development) för att minska utrymmet för buggar och från början kunna vara säkra på att våra funktioner fungerar som vi vill. Vi har skrivit tre olika typer av test. Enhetstester, Integrationstester och Regressionstester. 

## Enhetstestning

Vi har skrivit enhetstester för alla viktiga funktioner i projektet i en testfil **test.c**. Vi arbetade enligt TDD vilket har gjort att vi har testat vår kod hela tiden under utväcklingen. Varje modul har testfall som testar dess funktionalitet och dessutom edge cases och felhantering.

## Coverage

Vi har undersökt testernas coverage med hjälp av verktyget **gcov** och här är våra resultat:

#### File 'src/ref.c':
Lines executed: 89.73% of 146

Branches executed: 100.00% of 66



 

    void free_all() {
	    cleanup();
	    lib_linked_list_destroy(get_schedule_linked_list());
	    schedule_linked_list = NULL;
	    lib_hash_table_destroy(get_metadata_ht());
    }	

	void shutdown() {
	    set_cascade_limit(STANDARD_CASCADE_LIMIT);
	    free_all();
    }

Vår coverage uppnår inte 100% eftersom att vi inte kan testa vår shutdown() funktion ovan. Anledningen är att den är svår att anropa i testmiljön eftersom att den endast ska anropas en gång, vilket är när programmet avslutas. Detta eftersom att den förstör implementationens länkade lista och hashtabell, vilket vi endast vill göra vid programmets slut. Men kodgranskning och manuell testning av funktionen har bekräftat att funktionen fungerar som vi vill. Den ger inga minnesläckor och fungerar korrekt. Vi har under utvecklingen av funktionen testat både shutdown() och funktionen free_all() som den kallar på och kan se att de friar för mycket minne om de körs fler än en gång. Genom att räkna antalet frees kan vi se att den fungerar så som vi vill om man bara anropar den en gång.

## Integrationstestning

Vi har gjort integrationstester för att vara säkra på att våra olika moduler fungerar tillsammans som förväntat. Dessa tester kontrollerar att inlupp2 fungerar korrekt tillsammans med ref.c. Vi utökade testerna från inlupp2 i filen **backend_tests.c** för att säkerställa att ref.c integrerats rätt.

## Regressionstestning

Vi har kört våra tester efter varje ändring i koden för att säkerställa att ändringar som gjordes i koden inte introducerade nya fel. Våra tester byggdes för tidigare implementerade funktioner och inkluderade tidigare funna buggar och edgecases. Att alltid säkerställa detta innan vi fortsatte med ett nytt steg minskade felen vi stötte på.

## Automatisering

Vi har försökt att automatisera våra tester med GitHub Actions så att de körs vid varje pull request och varje commit till main. Men bristen på tid gjorde att vi tyvärr inte hann få detta att fungera som vi ville då det var något nytt för oss alla i gruppen. Fördelarna vi hade kunnat ta del av om vi fått det att funka är dels att våra testresultat loggas direkt i GitHub, men även alla i gruppen får ett main med en sammanfattning av resultaten. 

## De sex otäckaste buggarna

Här är de sex allvarligaste buggarna som vi har stött på under projektets gång, samt länk till deras github-issues. Vi började använda oss av github-issues sent under projektet och därmed finns det inte länkar till alla buggar:

#### Bug 1 - Free_scheduled_tasks kallades rekursivt 
https://github.com/IOOPM-UU/Oboy/issues/43


#### Bug 2 - Problem med Cascade Limit - Stack overflow
https://github.com/IOOPM-UU/Oboy/issues/42

#### Bug 3  - Gammal bugg från inlupp 2
När vi ändrade namn på en vara och därefter försökte lägga till fler av den, ändrades inte lagersaldot.

#### Bug 4 - Free_scheduled_tasks frigjorde för mycket
När free_scheduled_list() låg i funkunen allocate() blev det minnesläckor på grund av att länkar frigjordes för tidigt.

#### Bug 5 - Tester som läcker 
https://github.com/IOOPM-UU/Oboy/issues/46 

#### Bug 6 - Tester i main som skrevs 'fel' från början
https://github.com/IOOPM-UU/Oboy/issues/19

## Slutsats

Samanfattningsvis har vi skapat enhetstester, integrationstester och regressionstester som tillsammans testar vår implementation noggrant och säkerställer att våra funktioner fungerar korrekt både själva och med varandra. 