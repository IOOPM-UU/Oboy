# Projekt rapport

# 1 Grupp
Vår grupp heter O’boy. Nedan följer information om gruppens medlemmar:

| Namn                    | E-mail                                   | Aktiva Datum           |
|-------------------------|------------------------------------------|------------------------|
| Alexandra Barbu         | alexandra.barbu.0105@student.uu.se       | 4/12/2024 - 15/01/2025 |
| Albin Carlsson          | albin.carlsson.9644@student.uu.se        | 4/12/2024 - 15/01/2025 |
| Einar Eriksson Wahlin   | einar.eriksson-wahlin.5775@student.uu.se | 4/12/2024 - 15/01/2025 |
| Felix Fagerstedt        | felix.fagerstedt.0046@student.uu.se      | 4/12/2024 - 15/01/2025 |
| David Julin             | david.julin.1279@student.uu.se           | 4/12/2024 - 15/01/2025 |
| Erika Wallin            | erika.wallin.6208@student.uu.se          | 4/12/2024 - 15/01/2025 |


# 2 Kvantifering
Vi började projektet 4 december, då vi inledde vår första gruppmöte. Vi kom då fram till att vi behåller kursens struktur 
för deadlines, och bestämde att ha en soft- och en hard-deadline.
<br>   **Soft-deadline**: (ursprungligt) 25/12/2024, ändrat till 1/1/2025
<br>   **Hard-deadline**: (ursprungligt) 1/1/2025, ändrat till 5/1/2025

I kommande möten vi hade, delade vi upp arbetet i fyra olika sprints, nedan följer start- och slutdatum för dem 
(med ändrade datum också):

Sprint   | Startdatum (ursprungligt) | Startdatum (ändrat) | Slutdatum (ursprungligt) |  Slutdatum (ändrat) 
|--------|---------------------------|---------------------|--------------------------|--------------------------
   1        | 4/12/2024                  | ändrades ej         | 10/12/2024               | 17/12/2024
2           | 10/12/2024                 | 17/12/2024          | 16/12/2024               | 24/12/2024
3           | 16/12/2024                 | 24/12/2024          | 19/12/2024               | 1/1/2025
4        |  21/12/2024                   | 29/12/2024          | 1/1/2025                 | 5/1/2025


Vi skrev totalt:
*  242  rader C kod
*  539  rader testkod


Som ett grupp jobbade vi totalt 260 timmar.

Vi hade totalt: 
* 183  git commits
* 34  Pull Requests
* 25  GitHub issues


# 3 Process
## 3.1 Inception
Processen som valdes för det här projektet är Scrum eftersom vi arbetade med den under kursens gång och alla medlemmar 
var vana med den. Det innebär att dela upp projektet i olika sprintar (delar) som ska klaras under en viss tid för 
att följa utvecklingen. Till skillnad från kursens sprintar där allt var redan planerad och uppdelad så fick vi indela 
arbetsuppgifterna själva, vilket representerade en stor del av planeringen. 

## 3.2 Implementation
Vi implementerade denna process med hjälp av en Trello-board, där vi
kategoriserade olika upggifter till respektive sprint och person. För att säkerställa att vi är på rätt spår så hade
vi vecko-möten där vi diskuterade indelningen av uppgifter samt justerade våra prioriteringar. Vi tycker att denna
process gjorde det lättare för oss att följa utvecklingen av vårt projekt, eftersom med hjälp av Trello kunde vi
visuellt se våra framsteg och snabbt justera arbetsbelastningen.

Under projektets gång märkte vi att processen hade både styrkor och svagheter. En av styrkorna var att vi lyckades
fördela arbetsbelastningen jämnt i sprintar samt mellan gruppmedlemmarna. Detta gjorde att alla kunde jobba parallellt,
vilket ökade effektiviteten. Däremot underskattade vi tiden vissa uppgifter kommer att ta, vilket ledde till förseningar 
som påverkade hela planeringen. Detta representerar en tydlig svaghet och som grupp tog vi beslutet att ändra planeringen.

Om vi skulle börja om, så skulle vi göra mer detaljerade tidsplaner och ägna mer tid åt oväntade hinder, såsom buggar. 
Detta representerar en smart lösning eftersom det kommer att minska stressen medlemmarna känner när man ligger efter i 
planeringen och är ett mer realistisk tidsuppskattning.

Alla beslut angående projektet togs i grupp på våra vecko-möten, där vi mest diskuterade läget och bestämde våra 
prioriteringar för respektive vecka. Vi planerade att arbeta även under Jul-lovet (med vissa undantag) för att undvika
större förseningar. Detta följdes av alla medlemmar och visade sig vara framgångsrik, då vi lyckades hålla projektet på 
rätt spår trots tidigare problem.



# 4 Användning av verktyg 
Under projektet använde vi flera verktyg för att underlätta arbetsprocessen. Vi har använt Discord som huvudsaklig
kommunikationsmetod eftersom den har en mängd användbara funktioner (röstsamtal, skärmdelning, text 
meddelanden) som underlättar samarbetet. Planeringen har gjorts med hjälp av en Trello-board, som främjar Scrum processen.
Alla medlemmar hade tillgång till denna board och kunde editera och visuellt se framsteg under arbetsprocessen. 

Vi valde att koda i VS Code eftersom detta är en IDE som stödjer par-/grupparbete med sin LiveShare funktion och alla
medlemmar var vana med den sedan innan. Kodning 
gjordes mestadels i par och LiveShare gynnar medlemmarna eftersom man kan koda tillsammans utan konflikter. För att lösa
buggar (segfaults eller stackoverflows) har vi använt oss av GDB, vilket är en debugger som gav oss möjligheten att 
köra programmet steg för steg, inspektera variabler och hitta exakt vart ett fel inträffar. 

Det är normalt att få minnesläckor eller andra minnesrelaterade problem när man utvecklar ett program i C-språket. För 
att hitta och åtgärda dem, har vi använt oss av Valgrind, som identifierar minnesläckor eller ogiltiga 
minnesåtkomster. För att säkerställa kodkvaliteten har vi tester som körs med hjälp av LCunit och GCOV. Genom att använda
LCunit har vi tester som vi kan köra automatiskt och verifiera att vår kod fungerar som förväntat. GCOV används för
att mäta kodtäckning, vilket gör det lätt för oss att se vilka områden som inte testas. Detta hjälper oss att identifiera 
kod som inte täcks av testerna eller död kod och gör det möjligt för oss att fokusera på vilka områden som ska förbättras.

För versionshantering och teknisk dokumentation använder vi GitHub med sina funktionaliteter: Issues och Pull Requests.
Vi uppdelade arbetsuppgifterna i olika moduler och branches vilket främjade parallell kodning och gjorde att vi undvek 
konflikter när vi senare skulle slå ihop alla delar av koden. När ett fel upptäcktes, skapade vi Issues, som
är synliga för alla medlemmar men kan tilldelas till en viss person som behöver åtgärda dem. Detta underlättade 
arbetsprocessen eftersom man kan enkelt hålla reda på vad som behöver göras. Vi har använt Pull Requests för att integrera
en ny del av kod i huvudbranchen. Vi hade alltid en opartisk medlem som kodgranskar koden innan den inkorporerades , för
att säkerställa att den uppfyllde kodstandarden och inte introducerade några buggar.

Vi upplever inte att vi har saknat några verktyg under projektets gång eftersom de verktyg vi har använt
bemöt våra behov när det gäller kodhantering, planering samt kommunikation. Vi har använt dessa verktyg tidigare och
kommer att fortsätta använda dem eftersom de uppfyller våra krav.


# 5 Kommunikation, Kooperation och Koordination

Vi kommunicerade med varandra (och med vår coach) via vår Discord kanal som skapades i början av projektet.
Innan "Jul-lovet" hade vi våra möten på plats eftersom vi tyckte det var det bästa kommunikationssättet, men
under lovet håll vi våra möte på Discord. Detta underlättade arbetsprocessen eftersom man kan snabbt "hoppa in" i någons
kanal och fråga något man undrar. 

Vi började att skriva kod i helgrupp, vilket snabbt upptäcktes vara ineffektivt, eftersom det är både svårt och krångligt
att hålla på med samma funktionalitet samtidigt, eftersom olika medlemmar har olika arbetsstilar. Detta ledde till några
förseningar tills vi delade upp arbetsbelastningen i par (och GitHub branches), vilket gjorde att bara en enskild par ska 
fokusera på en specifik del av projektet. Genom att dela upp ansvarsområden kunde vi jobba parallellt samt förtsätta ha 
koll på andras arbete när vi integrerade alla branches i main. 

Vi kan garantera att ingen av våra medlemmar har haft en svår period under projektets gång, vilket vi är glada över. Detta
kan bero på att alla var inkluderade i utvecklingen av den här processen. Vi hade möte när de passade alla medlemmar samt
anpassade arbetsbelastningen så att ingen känner sig överväldigad. Alla medlemmar var flexibla med sina arbetstider och 
kommunicerade öppet, vilket gjorde att vi undvek onödiga stressmoment. 

Som det har nämnts innan, hade vi inga fysiska möten under "Jul-lovet". På varje möte diskuterade vi hur olika par ligger 
till, om någon behöver hjälp eller likt, uppdaterade Trello så alla har koll på hur projektet utvecklas. Efter det fick
paren bestämma hur de vill lägga upp arbetet, dock så har de mesta förtsätt med sina uppgifter under dagen. Vi undvek 
att jobba under röda dagar, eftersom alla behövde ledig tid för att vila samt fira Jul/Nyår.

Under utvecklingsprocessen har vi lärt oss vikten av att överskatta tidsåtgången för en uppgift snarare än att 
underskatta den tiden, och sedan hamna efter i planeringen. I början underskattade vi tiden för vissa uppgifter, 
vilket skapade press när vi insåg att vi ligger efter vårt schema. Vi har också lärt oss att en stor grupp kan inte
jobba på samma uppgift tillsammans, eftersom detta resulterar i att bara några medlemmar kan bidra till uppgiften 
och gruppen förlorar tid. Att arbeta i par eller mindre grupper har visat sig vara en mer effektiv arbetsmetod.
På så sätt kan varje medlem få sitt eget ansvarsområde, arbeta parallellt och i sin egen takt med respektive uppgift. 
Notera dock att detta krävde öppen kommunikation, tillit, och disciplin för att fungera effektivt.

# 6 Work Breakdown Structure


Vi delade upp projektet i fyra sprintar:

## Sprint 1
Denna sprint innefattade fastställandet av desginkrav samt planering av programstrukturen på en hög nivå.
Detta gjordes dock översiktligt i början eftersom vi endast hade diskuterat det och ännu inte skissat en konkret 
struktur för programmet. Sprinten påbörjades den 4 december med ett planerat slutdatum den 10 december. Efter det första 
mötet med coachen insåg vi dock att den planerade implementationen inte överensstämde med projektets avsikt. Flest 
medlemmar var upptagna med inlämningsuppgift 4 under denna period så några förseningar inträffade här, när vi skulle 
omvärdera programmets struktur. Vi fick ändra slutdatumet till 17 december och tänka om vår planering för att ha en bra 
grund inför kommande sprintar. Sprint 1 gjordes i helgrupp för att säkerställa att alla medlemmar kan bidra till 
projektets struktur och få en djupare förståelse för målet.

## Sprint 2
Den andra sprinten hade som mål implementationen av basfunktionerna för skräpsamlaren, samt utveckla tillhörande 
enhetstester. Arbetet påbörjades den 10 december men inleddes med en mindre stabil grund. Avsaknaden av en tydlig struktur 
orsakade en missuppfattning kring hur implementationen skulle utformas. Vi jobbade i helgrupp under en kort stund, men
eftersom detta visade sig vara ineffektivt delade vi istället upp oss i tre par. Varje par tilldelades anvar för 
specifika funktioner och tillhörande tester att utveckla. Under processen insåg vi inte att den tänkta strukturen var 
bristfällig, men det första mötet med coachen gjorde oss medvetna om detta. Vi bestämde oss att gå tillbaka till sprint
1 och tänka om programmet och sedan återvända till den andra sprinten. Att göra detta anser vi vara det bästa valet
som vi har gjort, eftersom det lade en stabil grund för det fortsatta arbetet samt minskade risken för framtida problem,
även om det orsakade några förseningar. Hela planerigenen ändrades,  vi började om den 17 december och satte en deadline
den 24 december istället för den 16.


## Sprint 3
Denna sprinten fokuserar på att skapa olika destruktorer och integrations-/regressionstester. De olika destruktorer med
tillhörande tester tilldelades olika par, men eftersom alla medlemmar behövde ha samma grund för arbetet eller kunde inte
arbeta precis då (under Jul perioden), valde vi att minska antalet grupper eller jobba enskilt när de passade. Detta
resulterade i ett mer fokuserat och anpassad samarbete som alla kunde bidra till. Trots alla förseningar, verkade 
arbetsbelastningen vara rättvist fördelad vilket gjorde att alla medlemmar kunde arbeta effektivt utan större problem.
Precis som alla andra sprints, fick vi ändra slutdatumet från den 19 december till den 1 januari.

## Sprint 4
Den sista sprinten innefattar de sista stegen: att implementera skräpsamlaren i samband med den andra inlämningsuppgiften,
utveckla de befintliga testerna, genomföra små justeringar för att säkerställa programmets funktionalitet,
samt skriva klart de olika dokument som behöver lämnas in (rapporter och presentation). Eftersom implementationen verkade
vara relativt enkel så tilldelades ett par denna uppgift, medan de andra olika medlemmar fokuserade på olika dokument och
genomföra de små ändringar/tester som behövdes. Dock så hittade vi en stor bugg som ledde till en omstrukturering av
programmet. Eftersom den totala arbetsbelastningen var inte jämnt fördelat i slutet, valde vi att tilldela större 
uppdrag till medlemmarna som har arbetat mindre antal timmar, medan de andra fick fokusera på mindre delar av programmet.

## Arbetsuppdelning

I slutet har vi undersökt vad gruppen spenderade sin tid på under projektets gång med hjälp av en "pie chart". Här är
resultaten vi fick:

![Pie Chart](pie.png)

Som förväntat visade det sig att största delen av tiden gick åt kodskrivning främst på grund av att vi behövde 
åtgärda fel som gjordes i grundimplementationen. Möten representerar också en betydande del av våra arbetsuppgifter
då många viktiga beslut angående programmets implementation fattades här. Minst tid spenderades på dokumentering eftersom
detta är inte lika tidskrävande och gjordes bara en gång i slutet av projektet. 

# 7 Reflection 
Detta projekt representerar för alla medlemmar den första möjligheten att arbeta i en stor grupp. Även om olika ansvarsområden
tilldelades till olika personer så har vi alla försökt delta i varje viktig moment av projektets utveckling och stödja varandra.
På en skala från 1 till 7 bedömer vi vår tillfredsställelse med processen till en 6:a. Vi började alltid våra
möten genom att hur alla ligger till, visa hänsyn till de som behöver hjälp och planera för de kommande dagarna. Vi höll
alltid en organiserad struktur på projektet även när problem inträffade. Vi kan inte ge oss själva den högsta betyget eftersom
vår största misslyckande i projektet var att ignorera planeringen och börja koda utan att ha en fullständig struktur på
hur programmet ska se ut. Detta ledde till massa förseningar och vi var tvungna att ändra planeringen och börja om.
Kombinationen av frustration och av att ligga efter i planeringen hjälpte inte oss, men vi tog detta som ett lärdom.
Vi ser detta som vår största framgång – att kunna fortsätta och hitta lösningar trots de utmaningar vi stötte på.

Därför bedömer vi vår tillfredsställelse med den levererade produkten och kvalitetsgaranti till en 6. Genom att omstrukturera
koden från början, säkerställde vi en stabil grund för vidare implementationer. Detta siffra återspeglas i vår
testers kodtäckning : 89% lines executed och 100% branch executed. Vi har lagt stort fokus på att göra noggranna tester 
och kodgranskningar så att vi hamnar inte i samma situation som innan och säkerställer kodkvaliteten. Detta arbete har
lett till en pålitlig produkt som vi kan vara stolta över.

Sammanfattningsvis är vi nöjda med den levererade produkten. Vi har lärt oss vikten av att ha en detaljerad planering
och struktur på programmet samt hur viktigt kommunikation och arbetsuppdelning är när man arbetar i en stor grupp. 



