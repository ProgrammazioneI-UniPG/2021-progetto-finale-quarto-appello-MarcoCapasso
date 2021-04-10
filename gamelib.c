#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"

static unsigned short int numeroQuest;
static time_t t;
static int temp;
static int casuale;
static int astronautiDeceduti;
static int numeroGiocatori;
static struct Giocatore* giocatori = NULL;
static int numeroImpostori;
static int randomImpostori;
static int astronautiVivi;
static int impostoriTrovati;
static struct Stanza* primaStanza = NULL;
static int tipoStanzaAssegnato;
static struct Stanza* stanzaSuccessiva  = NULL;
static struct Stanza* elencoStanze  = NULL;
static struct Stanza* stanzaInizioGioco = NULL;
static int turnoRandom;
static int stanzeTotali;
static int stanzeBotola;
static bool movimentoImpostore;
static int stanzeBotolaRandom;
static unsigned short int questCompletate;
static void tipoStanza(int Tipo);
static void inizio_gioco();
static void stanzaCreata(struct Stanza* stanza_creata, struct Stanza* stanza_precedente);
static void giocatori_stanza();
static void stanzaPrincipale();
static void elencoGiocatori();
static void statoGiocatori(int Stato);
static void nomeGiocatori(int Nome);
static int stampaTipoStanza();
static void clear();
static void chiamata_emergenza();
static void uccidi_astronauta();
static void usa_botola();
static void avanza();
static void esegui_quest();
static void sabotaggio();

//Funzione per impostare la partita
void Imposta_Gioco()
{

 srand((unsigned)time(&t));
 printf(" Inserire il numero dei giocatori per questa partita: \n"); //Inserimento numero giocatori
 scanf("%d", &numeroGiocatori);
 while (getchar()!='\n'); // Svuoto il buffer
 if(numeroGiocatori < 4 || numeroGiocatori > 10)
 {
  while(numeroGiocatori < 4 || numeroGiocatori > 10)
  {
   printf("Errore giocatori\n");
   scanf("%d", &numeroGiocatori);
  }
 }

 printf("Il numero di giocatori è valido\n");
 if (numeroGiocatori<6) //Imposta casualmente il numero di impostori in base al numero di impostori
  {
    numeroImpostori=1;
  }
  else if(numeroGiocatori>5 && numeroGiocatori<9)
  {
     randomImpostori=rand()%5;
     if (randomImpostori>=0 && randomImpostori<3)
     {
       numeroImpostori=2;
     }
     else
     {
       numeroImpostori=1;
    }
  }
  else if(numeroGiocatori>=9)
  {
     randomImpostori=rand()%5;
     if (randomImpostori>=0 && randomImpostori<3)
     {
       numeroImpostori=3;
     }
     else
     {
       numeroImpostori=2;
     }
  }
 giocatori=(struct Giocatore *) calloc(numeroGiocatori, sizeof(struct Giocatore)); //Array di giocatori
 stanzaPrincipale();
 elencoGiocatori();
 printf("Inserire il numero di quest: \n"); //Inserimento numero di quest, verificando che sia valido per l'inizio della partita
 scanf ("%hu", &numeroQuest);
 if (numeroQuest < numeroGiocatori || numeroQuest > 20)
 {
   while (numeroQuest < numeroGiocatori || numeroQuest > 20)
   {
     printf ("Il numero di quest inserite non è adatto, inserire di nuovo: \n");
     scanf ("%hu", &numeroQuest);
   }
 }
 printf ("Il numero di quest inserito è corretto:%hu\n", numeroQuest);
 printf ("Premere 1 per giocare o 2 per impostare di nuovo il gioco: \n"); //L'utente può scegliere se impostare nuovamente la partita o iniziare a giocare
 static int impostazione_gioco_corretta;
 scanf ("%d", &impostazione_gioco_corretta);
 switch (impostazione_gioco_corretta)
 {
   case 1:
     clear();
     inizio_gioco();
     break;
   case 2:
     free (giocatori);
     giocatori=NULL;
     free (primaStanza);
     primaStanza=NULL;
     clear();
     break;
   default:
     printf ("La scelta non è valida!\n");
     break;
 }
}

//Inizio partita
static void inizio_gioco () {
  conferma_impostazione=true;
}

//Creazione della prima stanza
static void stanzaPrincipale()
{
 primaStanza=(struct Stanza*)malloc(sizeof(struct Stanza));
 primaStanza->Avanti=NULL;
 primaStanza->Destra=NULL;
 primaStanza->Sinistra=NULL;
 primaStanza->StanzaPrecedente=NULL;
 primaStanza->Next=NULL;
 primaStanza->Tipo=stampaTipoStanza();
 primaStanza->chiamata_emergenza=false;
 elencoStanze=primaStanza;
 stanzaInizioGioco=primaStanza;
}

//Funzione necessaria per creare una nuova stanza
static void stanzaCreata(struct Stanza* stanza_creata, struct Stanza* stanza_precedente)
{
  stanza_creata->Avanti=NULL;
  stanza_creata->Destra=NULL;
  stanza_creata->Sinistra=NULL;
  stanza_creata->Next=NULL;
  stanza_creata->StanzaPrecedente=stanza_precedente;
  stanza_creata->Tipo=stampaTipoStanza();
  stanza_creata->chiamata_emergenza=false;
}

//Stampa il tipo della stanza
static void tipoStanza(int Tipo)
{
 if (Tipo==3)
 {
   printf("Botola\n");
 }
 else if(Tipo==2)
 {
   printf("Quest complicata\n");
 }
 else if(Tipo==1)
 {
   printf("Quest semplice\n");
 }
 else
 {
   printf("Vuota\n");
 }
}

//Creazione tipo stanza casualmente
static int stampaTipoStanza()
{
 tipoStanzaAssegnato=rand()%101;
 if (tipoStanzaAssegnato>=0 && tipoStanzaAssegnato<=25)
 {
   tipoStanzaAssegnato=3;
 }
 else if (tipoStanzaAssegnato>=26 && tipoStanzaAssegnato<=40)
 {
   tipoStanzaAssegnato=2;
 }
 else if (tipoStanzaAssegnato>=41 && tipoStanzaAssegnato<=70)
 {
   tipoStanzaAssegnato=1;
 }
 else if (tipoStanzaAssegnato>70)
 {
   tipoStanzaAssegnato=0;
 }
 return tipoStanzaAssegnato;
}

//Stampa lo stato del giocatore
static void statoGiocatori(int Stato)
{
  switch(Stato)
  {
  case 0:
   printf("Astronauta\n");
   break;
  case 1:
   printf("Impostore\n");
   break;
  case 2:
   printf("Assassinato\n");
   break;
  case 3:
   printf("Defenestrato\n");
   break;
  }
}

//Stampa il nome del giocatore
static void nomeGiocatori(int Nome)
{
  switch(Nome)
  {
  case 0:
   printf("Bianco\n");
   break;
  case 1:
   printf("Giallo\n");
   break;
  case 2:
   printf("Nero\n");
   break;
  case 3:
   printf("Rosso\n");
   break;
  case 4:
   printf("Verde\n");
   break;
  case 5:
   printf("Marrone\n");
   break;
  case 6:
   printf("Ciano\n");
   break;
  case 7:
   printf("Rosa\n");
   break;
  case 8:
   printf("Arancione\n");
   break;
  case 9:
   printf("Blu\n");
   break;
  }
}

//Stampa i giocatori in ordine casuale
static void elencoGiocatori()
{
 for (int i = 0; i < numeroGiocatori; i++)
 {
   giocatori[i].Posizione=primaStanza;
   if (numeroImpostori!=0)
   {
     giocatori[i].Stato=rand()%2;
     if (giocatori[i].Stato==1)
     {
       numeroImpostori--;
     }
   }
   else
   {
     giocatori[i].Stato=0;
   }
   giocatori[i].Nome=i;
 }
 for (int i = 0; i < numeroGiocatori; ++i)
 {
   temp=giocatori[i].Nome;
   casuale=rand()%numeroGiocatori;
   giocatori[i].Nome=giocatori[casuale].Nome;
   giocatori[casuale].Nome=temp;
 }
 for (int i = 0; i < numeroGiocatori; ++i)
 {
   printf("\n%d° Giocatore \n", i+1);
   printf("\nTipo di Stanza: ");
   tipoStanza(giocatori[i].Posizione->Tipo);
   printf("\nStato: ");
   statoGiocatori(giocatori[i].Stato);
   printf("\nColore: ");
   nomeGiocatori(giocatori[i].Nome);
   printf("\n\n");
 }
}

//Funzione per giocare
void Gioca()
{
  static int astronauti;
  static int impostori;
  static int conta_turni;
  static int stop_turno;
  static int scelta_astronauta;
  static int scelta_impostore;
  static int scelta_movimento;
  stanzaSuccessiva = primaStanza;
  int turno[numeroGiocatori];
  for (int i=0;i < numeroGiocatori; i++)
  {
    if (giocatori[i].Stato==0)
    {
      astronauti++;
    }
    else if (giocatori[i].Stato==1)
    {
      impostori++;
    }
  }
  while (numeroQuest > questCompletate && impostori < astronauti-astronautiDeceduti && impostori > impostoriTrovati) //condizioni necessarie per terminare la partita
  {
    if (conta_turni < numeroGiocatori)
    {
      turnoRandom=rand()%numeroGiocatori; //stampa per ogni turno in ordine casuale l'elenco dei giocatori
      if (turno[turnoRandom]!=1 && turno[turnoRandom]!=2 && turno[turnoRandom]!=3) //verifica se il giocatore può giocare o deve attendere 1 o 2 turni
      {
        if (giocatori[turnoRandom].Stato==0)
        {
          printf("\nSei il giocatore: ");
          nomeGiocatori(giocatori[turnoRandom].Nome); //stampo il nome del giocatore
          printf("\nTi trovi nella stanza: ");
          stanzaSuccessiva = giocatori[turnoRandom].Posizione;
          tipoStanza(stanzaSuccessiva->Tipo); //stampo il tipo della stanza dove il giocatore di trova
          giocatori_stanza();
          printf ("\nCosa vuoi fare?\n1) Avanzare \n2)Eseguire quest\n3) Effettuare chiamata d'emergenza\n"); //in base alla scelta del giocatore si verificherà una funzione
          scanf ("%d", &scelta_astronauta);
          switch (scelta_astronauta)
          {
            case 1:
              printf ("Dove vuoi andare?\n1) Avanti\n2) Destra\n3) Sinistra\n4) Rimani fermo\n");
              scanf ("%d", &scelta_movimento);
              if (scelta_movimento<1 || scelta_movimento>4) //chiede al giocatore fino a quando non verrà inserito un valore valido
              {
                 while (scelta_movimento<1 || scelta_movimento>4)
                 {
                   printf ("Dove vuoi andare?\n1) Avanti\n2) Destra\n3) Sinistra\n4) Rimani fermo\n");
                   scanf ("%d", &scelta_movimento);
                 }
              }
              avanza(scelta_movimento);
              turno[turnoRandom]=1;
              break;
            case 2:
              esegui_quest(turno, turnoRandom);
              break;
            case 3:
              chiamata_emergenza();
              turno[turnoRandom]=1;
              break;
            default:
              printf("Scelta non valida!\n");
              turno[turnoRandom]=1;
              break;
          }
          conta_turni++;
        }
        else if (giocatori[turnoRandom].Stato==1)
        {
          printf("\nSei il giocatore: ");
          nomeGiocatori(giocatori[turnoRandom].Nome);
          printf("\nTi trovi nella stanza: ");
          stanzaSuccessiva = giocatori[turnoRandom].Posizione;
          tipoStanza(stanzaSuccessiva->Tipo);
          giocatori_stanza();
          printf ("\nCosa vuoi fare?\n1) Avanzare\n2)Uccidere astronauta\n3) Usare la botola\n4) Effettuare sabotaggio della stanza\n5)Effettuare chiamata d'emergenza\n");
          scanf ("%d", &scelta_impostore);
          switch (scelta_impostore)
          {
            case 1:
              printf ("Dove vuoi andare?\n1) Avanti\n2) Destra\n3) Sinistra\n4) Rimani fermo\n");
              scanf ("%d", &scelta_movimento);
              if (scelta_movimento<1 || scelta_movimento>4)
              {
                 while (scelta_movimento<1 || scelta_movimento>4)
                 {
                 printf ("Dove vuoi andare?\n1) Avanti\n2) Destra\n3) Sinistra\n4) Rimani fermo\n");
                 scanf ("%d", &scelta_movimento);
                 }
              }
              avanza(scelta_movimento);
              turno[turnoRandom]=1;
              break;
            case 2:
              uccidi_astronauta();
              turno[turnoRandom]=1;
              break;
            case 3:
              usa_botola();
              turno[turnoRandom]=1;
              break;
            case 4:
              sabotaggio();
              turno[turnoRandom]=2;
              break;
            case 5:
              chiamata_emergenza();
              turno[turnoRandom]=1;
              break;
            default:
              printf("Scelta non valida!\n");
              turno[turnoRandom]=1;
              break;
          }
          conta_turni++;
        }
        else if (giocatori[turnoRandom].Stato==2)
        {
            printf("\nSta giocando il giocatore : ");
            nomeGiocatori(giocatori[turnoRandom].Nome);
            printf ("\nSei stato ucciso!\nDovrai aspettare la fine della partita\n");
            turno[turnoRandom]=1;
            conta_turni++;
        }
        else if (giocatori[turnoRandom].Stato==3)
        {
            printf("\nSta giocando il giocatore : ");
            nomeGiocatori(giocatori[turnoRandom].Nome);
            printf ("\nSei stato defenestrato!\nDovrai aspettare la fine della partita\n");
            turno[turnoRandom]=1;
            conta_turni++;
        }
        if (numeroQuest-questCompletate<=0) //tutte le quest sono state completato
        {
          printf("Tutte le quest sono state completate e hanno vinto gli ASTRONAUTI!\n");
          fine_gioco=true;
        }
        clear();
      }
    }
    else
    {
      stop_turno=0;
      for (int i=0; i<numeroGiocatori; i++)
      {
        turno[i]--; //se il giocatore non ha compiuto azioni che prevedono lo stare fermo 1 o 2 turni allora potrà giocare subito
      }
      for (int i=0; i<numeroGiocatori; i++)
      {
        if (turno[i]>=1)
        {
          stop_turno++;
        }
      }
      conta_turni=0+stop_turno;
      }
    }
    if (impostori==impostoriTrovati) //tutti gli impostori sono stati scoperti
    {
     printf("Tutti gli impostori sono stati scoperti. Hanno vinto gli ASTRONAUTI!\n");
     fine_gioco=true;
    }
    else if (astronauti-astronautiDeceduti==impostori)
    {
     printf("Tutti gli astronauti sono stati eliminati. Hanno vinto gli IMPOSTORI!\n");
     fine_gioco=true;
    }
}

//Funzione per terminare il gioco, dealloca la memoria usata
 void Termina_Gioco()
 {
   if (giocatori!=NULL)
   {
     free(giocatori);
     giocatori=NULL;
   }
   else if (primaStanza!=NULL)
   {
     free(primaStanza);
     primaStanza=NULL;
   }
   else if (stanzaSuccessiva!=NULL)
   {
    free(stanzaSuccessiva);
     stanzaSuccessiva=NULL;
   }
 }

//Funzione che controlla lo stato e il numero di giocatori presenti in una stanza
static void giocatori_stanza()
{
  astronautiVivi=0; //variabile che controlla il numero di astronauti utile poi per la funzione uccidi_astronauta
  for (int i = 0; i <numeroGiocatori; i++)
  {
    if (i!= turnoRandom)
    {
      if (giocatori[turnoRandom].Posizione==giocatori[i].Posizione)
      {
        if (giocatori[i].Stato==2)
        {
          printf("\nNella stanza c'è un giocatore assassinato."); //i giocatori nella stanza possono la chiamata d'emergenza
        }
        if (giocatori[i].Stato!=3)
        {
          if (giocatori[i].Stato==1)
          {
            printf("\nIn questa stanza c'è il giocatore: ");
            nomeGiocatori(giocatori[i].Nome);
          }
          else if (giocatori[i].Stato==0)
          {
            printf("\nIn questa stanza c'è il giocatore: ");
            nomeGiocatori(giocatori[i].Nome);
            astronautiVivi++;
          }
        }
      }
    }
  }
}

//Funzione per far ricominciare il giro di turni
static void clear()
{
  static int reset;
  printf("\nInserire un numero per pulire la schermata: ");
  scanf("%d", &reset);
  switch(reset)
  {
    default:
      system("clear");
      break;
  }
}

//Funzione che permette ai giocatori di muoversi nella direzione scelta o di rimanere fermi
static void avanza(int movimento)
{
    switch (movimento)
    {
      case 1:
        stanzaSuccessiva=giocatori[turnoRandom].Posizione;
        if (stanzaSuccessiva->Avanti!=NULL) //verifica se la stanza sia già stata creata
        {
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Avanti;
          printf ("\nSei arrivato nella stanza: ");
          tipoStanza(stanzaSuccessiva->Avanti->Tipo);
        }
        else
        {
          stanzaSuccessiva->Avanti=(struct Stanza*)malloc(sizeof(struct Stanza)); //creo la stanza dato che non è stata raggiunta ancora da nessuno
          stanzaCreata(stanzaSuccessiva->Avanti, stanzaSuccessiva);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipoStanza(stanzaSuccessiva->Avanti->Tipo);
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Avanti;
          elencoStanze->Next=giocatori[turnoRandom].Posizione;
          elencoStanze=giocatori[turnoRandom].Posizione;
        }
        giocatori_stanza();
        break;
      case 2:
        stanzaSuccessiva=giocatori[turnoRandom].Posizione;
        if (stanzaSuccessiva->Destra!=NULL)
        {
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Destra;
          printf ("\nSei arrivato nella stanza: ");
          tipoStanza(stanzaSuccessiva->Destra->Tipo);
        }
        else
        {
          stanzaSuccessiva->Destra=(struct Stanza*)malloc(sizeof(struct Stanza));
          stanzaCreata(stanzaSuccessiva->Destra, stanzaSuccessiva);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipoStanza(stanzaSuccessiva->Destra->Tipo);
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Destra;
          elencoStanze->Next=giocatori[turnoRandom].Posizione;
          elencoStanze=giocatori[turnoRandom].Posizione;
        }
        giocatori_stanza();
        break;
      case 3:
        stanzaSuccessiva=giocatori[turnoRandom].Posizione;
        if (stanzaSuccessiva->Sinistra!=NULL)
        {
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Sinistra;
          printf ("\nSei arrivato nella stanza: ");
          tipoStanza(stanzaSuccessiva->Sinistra->Tipo);
        }
        else
        {
          stanzaSuccessiva->Sinistra=(struct Stanza*)malloc(sizeof(struct Stanza));
          stanzaCreata(stanzaSuccessiva->Sinistra, stanzaSuccessiva);
          printf ("\nSei arrivanto in una nuova stanza: ");
          tipoStanza(stanzaSuccessiva->Sinistra->Tipo);
          giocatori[turnoRandom].Posizione=stanzaSuccessiva->Sinistra;
          elencoStanze->Next=giocatori[turnoRandom].Posizione;
          elencoStanze=giocatori[turnoRandom].Posizione;
        }
        giocatori_stanza();
        break;
      case 4:
        stanzaSuccessiva=giocatori[turnoRandom].Posizione;
        printf ("\nSei rimasto nella stessa stanza dove ti trovavi\n");
        giocatori_stanza();
        break;
      default:
        printf("\nLa scelta non è valida!\n");
        break;
    }
}

//Funzione che permette all'utente di completare una quest
static void esegui_quest(int turno_giocatore[], int giocatore_che_gioca)
{
  if (giocatori[turnoRandom].Posizione->Tipo==1)
  {
    giocatori[turnoRandom].Posizione->Tipo=0;
    questCompletate++;
    printf("Rimangono %hu quest da completare\n", numeroQuest-questCompletate);
    printf("La stanza adesso è vuota\n");
    turno_giocatore[giocatore_che_gioca]=2; //con una quest semplice il giocatore starà fermo un turno
  }
  else if (giocatori[turnoRandom].Posizione->Tipo==2)
  {
    giocatori[turnoRandom].Posizione->Tipo=0;
    questCompletate+=2;
    printf("Rimangono %hu quest da completare\n", numeroQuest-questCompletate);
    printf("La stanza adesso è vuota\n");
    turno_giocatore[giocatore_che_gioca]=3; //con una quest complicata il giocatore starà fermo 2 turni
  }
  else if (giocatori[turnoRandom].Posizione->Tipo==0 || giocatori[turnoRandom].Posizione->Tipo==3)
  {
    printf ("Non ci sono quest presenti nella stanza in cui ti trovi\n");
    turno_giocatore[giocatore_che_gioca]=1;
  }
}

//Funzione usata dai giocatori nella stanza dove è presente un giocatore assassinato
static void chiamata_emergenza()
{
  static int decisione_chiamata;
  static int astronauti_morti;
  static int impostore_defenestrato;
  static int astronauta_defenestrato;
  static int defenestrazione_casuale;
  static int minimo_giocatori_chiamata;
  static bool defenestrazione_effettuata;
  if(giocatori[turnoRandom].Posizione->chiamata_emergenza==false) //Controlla che la chiamata d'emergenza non sia stata già effettuata
  {
    astronauti_morti=0;
    for(int i=0; i<numeroGiocatori; i++) //verifico la presenza di giocatori assassinati
    {
      if(giocatori[i].Stato==2 && giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
      {
        astronauti_morti++;
      }
    }
    if(astronauti_morti>0) //se vi sono astronauti assassinati
    {
      printf("Nella stanza vi sono astronauti assassinati, vuoi fare la chiamata d'emergenza?\n1) Sì\n2) No\n");
      scanf("%d", &decisione_chiamata);
      switch(decisione_chiamata)
      {
        case 1:
          impostore_defenestrato=30;
          astronauta_defenestrato=30;
          for (int i=0; i<numeroGiocatori; i++){
            if (giocatori[turnoRandom].Posizione==giocatori[i].Posizione)
            {
              if (giocatori[i].Stato==0)
              {
                for (int j=0; j<numeroGiocatori; j++) //controllo se vi sono altri giocatori presenti
                {
                  if (i!=j)
                  {
                    if( giocatori[j].Posizione==giocatori[turnoRandom].Posizione)
                    {
                      if (giocatori[j].Stato==1)
                      {
                        astronauta_defenestrato+=20;
                      }
                      else if(giocatori[j].Stato==0)
                      {
                        astronauta_defenestrato-=30;
                      }
                    }
                  }
                }
              }
              else if(giocatori[i].Stato==1)
              {
                for (int j=0; j<numeroGiocatori; j++)
                {
                  if (i!=j)
                  {
                    if (giocatori[j].Posizione==giocatori[turnoRandom].Posizione)
                    {
                      if(giocatori[j].Stato==0)
                      {
                        impostore_defenestrato+=20;
                      }
                      else if (giocatori[j].Stato==1)
                      {
                        impostore_defenestrato-=30;
                      }
                    }
                  }
                }
              }
            }
          }
          minimo_giocatori_chiamata=0; //verifico se oltre all'assassinato vi sono altri giocatri nella stessa stanza
          defenestrazione_effettuata=false;
          for (int i=0; i<numeroGiocatori; i++)
          {
            if (giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
            {
              if (giocatori[i].Stato==0 || giocatori[i].Stato==1)
              {
                minimo_giocatori_chiamata++;
              }
            }
          }
          if(minimo_giocatori_chiamata>=2)
          {
            if (impostore_defenestrato>astronauta_defenestrato) // l'impostore ha probabilità maggiori di essere defenestrato
            {
              while (!defenestrazione_effettuata)
              {
                for (int i=0; i<numeroGiocatori; i++)
                {
                  if(giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
                  {
                    if(giocatori[i].Stato==1)
                    {
                      printf("Il giocatore che è stato defenestrato : ");
                      nomeGiocatori(giocatori[i].Nome);
                      printf("\nera un impostore\n");
                      impostoriTrovati++;
                      giocatori[i].Stato=3;
                      defenestrazione_effettuata=true;
                    }
                  }
                }
              }
            }
            else if (astronauta_defenestrato>impostore_defenestrato) //l'astronauta ha probabilità maggiori di essere defenestrato
            {
              while(!defenestrazione_effettuata)
              {
              for(int i=0; i<numeroGiocatori; i++)
              {
                if(giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
                {
                  if(giocatori[i].Stato==0) {
                    printf("Il giocatore che è stato defenestrato : ");
                    nomeGiocatori(giocatori[i].Nome);
                    printf("\nnon era un impostore\n");
                    astronautiDeceduti++;
                    giocatori[i].Stato=3;
                    defenestrazione_effettuata=true;
                  }
                }
              }
            }
          }
          else if(astronauta_defenestrato==impostore_defenestrato) //random in cui le probabilità siano uguali
          {
            defenestrazione_casuale=rand()%2; //la defenestrazione del giocatore viene effettuata randomicamente
            switch(defenestrazione_casuale)
            {
              case 0: //random in cui esca l'impostore
                while (!defenestrazione_effettuata)
                {
                  for (int i=0;i<numeroGiocatori;i++)
                  {
                    if (giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
                    {
                      if (giocatori[i].Stato==1)
                      {
                        printf("Il giocatore che è stato defenestrato : ");
                        nomeGiocatori(giocatori[i].Nome);
                        printf("\nera un impostore!\n");
                        impostoriTrovati++;
                        giocatori[i].Stato=3;
                        defenestrazione_effettuata=true;
                      }
                    }
                  }
                }
                break;
              case 1: //random in cui esca l'astronauta
                while (!defenestrazione_effettuata)
                {
                  for (int i=0;i<numeroGiocatori;i++)
                  {
                    if (giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
                    {
                      if (giocatori[i].Stato==0)
                      {
                        printf("Il giocatore che è stato defenestrato : ");
                        nomeGiocatori(giocatori[i].Nome);
                        printf("\nnon era un impostore!\n");
                        astronautiDeceduti++;
                        giocatori[i].Stato=3;
                        defenestrazione_effettuata=true;
                      }
                    }
                  }
                }
                break;
            }
          }
        }
        else
        {
          printf ("Non vi è un numero sufficiente di giocatori nella stanza per effettuare una defenestrazione\n");
        }
          giocatori[turnoRandom].Posizione->chiamata_emergenza=true; //le chiamate d'emergenza in questa stanza sono finite
          break;
        case 2:
          printf("Hai deciso di non usare la chiamata d'emergenza\n");
          break;
        default:
          printf("La scelta non è valida!\n");
      }
    }
    else
    {
      printf("Non vi sono presenti astronauti assassinati in questa stanza\n");
    }
  }
  else
  {
    printf("La chiamata d'emergenza è già stata attivata in questa stanza\n");
  }
}

//Funzione che permette all'impostore di uccidere un astronauta nellastanza
static void uccidi_astronauta()
{
  static bool assassinio_avvenuto;
  static int conta_giocatori;
  static int vittima_scelta;
  static int astronauta_eliminato;
  static int probabilita;
  static int probabilita_parziale;
  if (astronautiVivi!=0)
  {
    assassinio_avvenuto=false;
    conta_giocatori=0;
    while (!assassinio_avvenuto && conta_giocatori<numeroGiocatori)
    {
      if (conta_giocatori!=turnoRandom)
      {
        if (giocatori[turnoRandom].Posizione==giocatori[conta_giocatori].Posizione && giocatori[conta_giocatori].Stato==0)
        {
          nomeGiocatori(giocatori[conta_giocatori].Nome);
          printf("\nSe vuoi uccidere questo giocatore premi 1 \n premi 2 per vedere il prossimo\n");
          scanf("%d", &vittima_scelta);
          switch (vittima_scelta) {
            case 1:
              printf("Hai ucciso il giocatore: ");
              nomeGiocatori(giocatori[conta_giocatori].Nome);
              astronauta_eliminato=conta_giocatori;
              astronautiDeceduti++;
              giocatori[conta_giocatori].Stato=2;
              assassinio_avvenuto=true;
              break;
            case 2:
              printf("Hai deciso di non uccidere il giocatore: ");
              nomeGiocatori(giocatori[conta_giocatori].Nome);
              break;
            default:
              printf("La scelta non è valida!\n");
              break;
          }
        }
      }
      conta_giocatori++;
    }
  }
  else
  {
    printf ("Non ci sono astronauti da assassinare\n");
  }
  if (assassinio_avvenuto)
  {
    probabilita_parziale=0;
    for (int i=0; i<numeroGiocatori; i++)
    {
      if (i!=turnoRandom && i!=astronauta_eliminato && giocatori[i].Stato==0)
      {
        if (giocatori[i].Posizione==giocatori[turnoRandom].Posizione)
        {
          probabilita_parziale+=50;
        }
        else if (giocatori[i].Posizione==giocatori[turnoRandom].Posizione->StanzaPrecedente)
        {
          probabilita_parziale+=20;
        }
      }
    }
  }
  probabilita=rand()%101;
  if (probabilita>0 && probabilita<=probabilita_parziale)
  {
    nomeGiocatori(giocatori[turnoRandom].Nome);
    printf(" era un impostore, ora verrà defenestrato\n");
    giocatori[turnoRandom].Stato=3;
    impostoriTrovati++;
  }
  else
  {
    printf ("Sei salvo, non verrai defenestrato!\n");
  }
}

//Funzione usata dagli impostori per spostarsi in un'altra stanza con una botola all'altra
static void usa_botola()
{
  stanzeTotali=0;
  stanzeBotola=0;
  movimentoImpostore=false;
  primaStanza=stanzaInizioGioco;
  if (giocatori[turnoRandom].Posizione->Tipo==3) //verifico se il giocatore si trova in una stanza con una botola
  {
    printf("Hai deciso di usare la botola nella stanza\n");
    while(primaStanza!=NULL)
    {
      if(primaStanza->Tipo==3 && primaStanza!=giocatori[turnoRandom].Posizione)
      {
        stanzeBotola++; //aumento il numero delle stanze con botola
      }
      stanzeTotali++; //aumento il numero di stanze prive di botola
      primaStanza=primaStanza->Next;
    }
    primaStanza=stanzaInizioGioco;
    printf("Ci sono %d stanze totali\n", stanzeTotali);
    int conta_botole[stanzeTotali];
    for (int i=0; i<stanzeTotali; i++)
    {
      if (primaStanza->Tipo==3 && primaStanza!=giocatori[turnoRandom].Posizione)
      {
        conta_botole[i]=1;
      }
      else
      {
        conta_botole[i]=0;
      }
      primaStanza=primaStanza->Next;
    }
    primaStanza=stanzaInizioGioco;
    if(stanzeBotola>0)
    {
      printf("%d hanno una botola, ora verrai spostato in un'altra stanza con una botola\n", stanzeBotola);
      stanzeBotolaRandom=rand()%stanzeTotali;
      while(!movimentoImpostore)
      {
        if (conta_botole[stanzeBotolaRandom]==0)
        {
          stanzeBotolaRandom=rand()%stanzeTotali;
        }
        else
        {
          while (stanzeBotolaRandom!=0)
          {
            primaStanza=primaStanza->Next;
            stanzeBotolaRandom--;
          }
          movimentoImpostore=true;
        }
      }
      giocatori[turnoRandom].Posizione=primaStanza; //aggiorno la posizione del giocatore
      printf("Ti sei spostato nella stanza: ");
      tipoStanza(giocatori[turnoRandom].Posizione->Tipo);
      giocatori_stanza();
    }
    else
    {
      printf("Non vi sono ancora stanze con una botola, verrai spostato in una stanza casuale qualsiasi\n");
      stanzeBotolaRandom=rand()%stanzeTotali;
      while(stanzeBotolaRandom!=0)
      {
        primaStanza=primaStanza->Next;
        stanzeBotolaRandom--;
      }
      giocatori[turnoRandom].Posizione=primaStanza;
      printf("Sei stato portato nella stanza: ");
      tipoStanza(giocatori[turnoRandom].Posizione->Tipo);
      giocatori_stanza();
    }
  }
  else
  {
   printf("Nella stanza non vi è nessuna botola\n");
  }
}

static void sabotaggio()
{
  if (giocatori[turnoRandom].Posizione->Tipo==1)
  {
    giocatori[turnoRandom].Posizione->Tipo=0; //la stanza diventa vuota
    printf("La stanza adesso è vuota\n");
  }
  else if (giocatori[turnoRandom].Posizione->Tipo==2)
  {
    giocatori[turnoRandom].Posizione->Tipo=0; //la stanza diventa vuota
    printf("La stanza adesso è vuota\n");
  }
  else
  {
    printf("Nella stanza non sono presenti quest da sabotare\n");
  }
}
