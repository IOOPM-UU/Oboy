# Code Review Report

## Introduktion
Under projektets gång har vi genomfört kodgranskning med jämna mellanrum, för att säkerställa kodkvalite, korrekthet och underhållbarhet. Vi har använt oss av **Pull Requests** med tydliga beskrivningar av vad som har ändrats i koden. Denna rapport beskriver hur vi använde oss av kodgranskning och vad vi lärt oss inför framtiden och kommande projekt.

## Syfte och konsekvenser av kodgranskningar

### Fördelar vi observerat:
1. **Felhantering:**
   - Logiska fel och **edge cases** upptäcktes tidigt under kodgranskningarna vilket minskade eventuella buggar i framtiden.

2. **Kodkvalitet:**
   - Läsbarheten och formatteringen kontrollerades och vi säkerställde att kodkvaliteten följde våra riktlinjer. 

3. **Öppnade dörrar för disskusion om koden:**
   - Eftersom många i gruppen arbetade på olika delar av koden så gav kodgranskningarna oss tillfällen att diskutera och förstå helheten av koden, vilket gjorde att alla i gruppen höll sig uppdaterade och arbetade mot samma vision. 


### Utmaningar:
- Kodgranskningarna öppnade för diskussioner om hur vi skulle gå tillväga, där vi ibland tyckte olika. Detta kunde ta tid att diskutera men i slutändan gav det mer förståelse för uppgiften och vi upptäckte ofta **edge cases** och andra eventuella problem som kunde uppstå.


### Procedur för granskningar:
1. **Användning av checklista:**
   - Vi skapade en checklista som vi följde när vi granskade koden. Detta gjorde det lättare att granska koden då det blev mer strukturerat och tydligt vad vi skulle göra. 

2. **Granskning av Pull Requests:**
    - Vid varje Pull Request krävdes en granskares godkännande innan vi mergade den nya koden till main. 

3. **GitHub Diffs:**
    - De färgkodade ändringarna i GitHub hjälpte oss att spåra ändringar som gjorts och minska riskerna att stöta på merge-konflikter.
    - De som granskade lämnade kommentarer direkt i koden, vilket förenklade implementering av förbättringar.

### Statistik för Pull Requests

**Antal Pull Requests:** Cirka 25 st Pull Requests skapades under projektet.

**Push-backs innan merging:**

- Vi hade inga Push-backs av Pull Requests under projektet. Detta eftersom att vi alltid jobbade på egna brancher och fick allt att fungera innan vi skickade en Pull Request.

**Pull Request-livslängd:**

- Nästan alla Pull Requests mergades inom 1 dag.

**Konsistens:**

- Alla gruppmedlemmar använde Pull Requests på samma sätt, vilket säkerställde att alla förstod vad varje Pull Request skulle ändra i koden och därmed inte råkade godkänna något som inte skulle godkännas. 

### Lärdomar

**Positiva erfarenheter:**

- Tidiga granskningar sparar tid: Att identifiera problem tidigt sparade tid för felsökning och testning senare.

- Ökat samarbete: Granskningsprocessen förbättrade kommunikationen och hjälpte samarbetet.

**Förbättringsområden:**

- Tidsplanering:

    - Bättre planering av deadlines för granskningar kunde ha undvikit flaskhalsar nära milstolpar.

**Rekommendationer för framtida projekt**

- Håll Pull Request-storleken hanterbar:

    - Dela upp ändringar i mindre, fokuserade PRs för att förenkla granskningar.


### Slutsats

Kodgranskningsprocessen vi använde i projekt förbättrade kodkvaliteten, underhållsbarheten och samarbetet i gruppen. Även om det ibland kändes som tidskrävande extra arbete, var fördelarna övervägande och minskade fel och ökade vår förståelse. 