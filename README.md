Game-Of-Life-Ncurses
====================

Piccola implementazione del famoso automa cellulare di Conway, sfruttando le librerie grafiche da terminale ncurses.

Sono disponibili alcuni comandi utilizzabili durante l'eseguzione:

  -[SPAZIO] -> genera una nuova griglia casuale di partenza
  
  -'i'      -> mostra una piccola finestrella con delle informazioni sulla sessione corrente.
  
  -'u'      -> nasconde la finestrella di info
  
  -'q'      -> termina ed esce dal programma
  
  -'c'      -> cambia il colore delle cellule
  
  -'b'      -> cambia il colore dello sfondo
  
  -'p'      -> attiva la modalità passo-passo: per continuare, premere 'p'
  
  -'o'      -> attiva la modalità continua
  
  -'e'      -> aumenta il delay tra le iterazioni
  
  -'w'      -> diminuisce il tempo tra le iterazioni
  
  
E' possibile modificare la dimensione della griglia o il carattere delle cellule vive/morte dai 'define' a inizio programma.
