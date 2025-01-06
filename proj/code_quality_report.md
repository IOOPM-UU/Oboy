# Code Quality Report

## Introduktion
Under projektets gång har gruppen strävat efter att skriva kod av hög kvalitet genom att fokusera på flera viktiga punkter för en bra kodutveckling. Denna rapport beskriver hur vi har uppnått god kodkvalitet med fokus på läsbarhet, underhållsbarhet, prestanda, korrekthet och testbarhet.


## Läsbarhet
För att öka kodens läsbarhet har vi tänkt på följande punkter:
- **Konsekvent namngivning**: Filer, funktioner och variabler följer tydliga och konsekventa namngivningskonventioner.
- **Kommentarer och dokumentation**: Delar av koden som behöver förtydligas är försedda med kommentarer som förklarar syfte och funktionalitet.
- **Modularisering**: Koden är uppdelad i små, hanterbara funktioner med tydliga ansvarsområden.
- **Formattering och kodstruktur**: Vi har använt kodformattering enligt standarder för att säkerställa att koden är lättläst.

Att vi bestämde hur vi skulle namnge samt formattera koden i början av projektet, har underlättat samarbetet i gruppen och gjort det enkelt för alla medlemmar att förstå och bidra till projektet, utan att vi behövt döpa om eller omformattera koden efterhand. 

## Underhållsbarhet
För att förbättra kodens underhållsbarhet har vi:
- **Separerat hantering av logik och data**: Under projektets gång har vi alltid varit medvetna om hur vi delar upp logik och data. Eftersom att vi endast har en struct "metadata_t" och dess tillhörande funktioner som har hand om datan har vi valt att ha data och logik i samma fil. Om det hade varit mer kod till datan hade vi dock valt att dela upp koden i separata filer. Att ha koll på vad som är data och vad som är logik underlättar felsökning och uppdateringar. 
- **Återanvändbarhet**: Funktioner har designats för att kunna återanvändas i olika delar av systemet.
- **Version control**: Vi har använt oss av GitHub för versionshantering och kodgranskning, vilket har gjort att vi kunnat ha bra kontroll över ändringar och enkelt kunnat gå tillbaka till äldre versioner av koden vid behov.


## Prestanda
För att uppnå hög prestanda har vi:
- **Effektiva algoritmer**: Optimerade datastrukturer och algoritmer för att minimera tidskomplexitet.
- **Minneshantering**: Implementerat dynamisk minneshantering och städningsfunktioner för att undvika minnesläckor.
- **Profilering och mätningar**: Genomfört prestandatester för att identifiera och åtgärda flaskhalsar.


## Korrekthet
Vi har säkerställt kodens korrekthet med hkälp av:
- **Enhetstester**: Skrivit omfattande tester för varje funktion för att kontrollera funktionaliteten.
- **Felhantering**: Implementerat robust felhantering för att upptäcka och rapportera oväntade tillstånd.
- **Kodgranskning**: Diskutterat koden inom gruppen vid varje möte för att kontrollera att alla i gruppen förstår samt höra flera olika perspektiv och därmed förhoppningsvis kunna identifiera potentiella fel tidigt.


## Testbarhet
Testbarheten har förbättrats genom:
- **Modulära funktioner**: Designade för att vara enkla att isolera och testa.
- **Mock-objekt**: Använt mock-objekt för att simulera beroenden och förenkla tester.


## Slutsats
Sammanfattningsvis har vårt fokus på läsbarhet, underhållsbarhet, prestanda, korrekthet och testbarhet resulterat i ett system som är lätt att förstå, enkelt att underhålla och robust vid körning. Vi är övertygade om att dessa principer har bidragit till att skapa kod av hög kvalitet som uppfyller projektets krav och mål.
