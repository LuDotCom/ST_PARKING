# ST PARKING
ST PARKING è uno dei tre progetti vincitori del Neapolis Innovation Summer Campus 2023 ideato, progettato e sviluppato da un Team composto da:
- Luca Comentale - Università Degli Studi di Napoli Federico II
- Nicolas Percuoco - Università Degli Studi di Napoli Federico II
- Giovanni D'Ambrosio - Università Degli Studi Luigi Vanvitelli
- Salvatore Cangiano - Università Degli Studi di Napoli Federico II
- Fabrizio Mazzeo - Università Degli Studi di Napoli Federico II
- Giuseppe Amore - Università Degli Studi di Napoli Federico II
- Luigi Cocozza - Università Degli Studi di Napoli Federico II
- Francesco Camillo - Università Degli Studi di Napoli Federico II



# Descrizione
Il progetto "ST PARKING" è un sistema innovativo di gestione del parcheggio urbano che utilizza diverse tecnologie per ridurre le emissioni e i consumi energetici. Per raggiungere questo obiettivo, ST PARKING è equipaggiato con un sensore ambientale che regola l'illuminazione del parcheggio in base alle condizioni di luce naturale circostanti. Quando le condizioni di luce sono scarse il sensore ambientale attiva una luce bassa che garantisce la visibilità necessaria per gli utenti. In particolare, l'elemento innovativo sta nel fatto che quando una macchina si avvicina al parcheggio, i sensori rilevano il movimento e aumentano progressivamente l'intensità delle luci, assicurando che il conducente abbia sempre la migliore visibilità possibile. Questo non solo migliora la sicurezza, ma riduce ulteriormente il consumo energetico, illuminando solo l'area stabilita al momento giusto. Infine, ST PARKING è dotato di uno schermo OLED che mostra una schermata di benvenuto e i posti disponibili all'interno del parcheggio. 

# Caratteristiche principali
- Utilizzo di sensori infrarossi [IR FC-51] per il monitoraggio dell'ingresso e dell'uscita dei veicoli.
- Regolazione automatica dell'illuminazione del parcheggio in base alle condizioni di luce naturale attraverso un sensore di luce ambientale [TEMT6000].
- Aumento dell'intensità luminosa dei Led quando un veicolo si avvicina in ingresso al parcheggio, migliorando la visibilità e la sicurezza.
- Contributo diretto alla sostenibilità ambientale riducendo le emissioni di CO2 e il consumo energetico.

# Utilizzo
__L'intera demo viene eseguita su un microcontrollore della ST Microelectronics STM32-Nucleo64-G474RE utilizzando il sistema operativo ChibiOS/RT e la toolchain ChibiStudio.__
Questo repository contiene il codice sorgente commentato riga per riga per garantire una maggiore leggibilità. Per maggiori informazioni, consultare il file readme.txt presente all'interno della cartella del sorgente. 

*N.B. Per l'utilizzo delle immagini sullo schermo Oled è possibile utilizzare uno script Matlab per la conversione.*

# Collegamenti
All'interno del repository è presente il file [Collegamenti.pdf](./collegamenti.pdf) che permette di replicare i collegamenti utilizzati per il funzionamento del ST PARKING. Visto che il numero di Led per l'illuminazione del parcheggio può essere facoltativa è stata omessa la descrizione del suo collegamento. In ogni caso nel file [myLib.h](./ST_PARKING/myLib.h) è possibile trovare la definizione dei PIN a cui vanno collegati i vari sensori.

