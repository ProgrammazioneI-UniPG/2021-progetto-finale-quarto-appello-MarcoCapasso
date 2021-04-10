#include <stdbool.h>
#include <stdio.h>
bool conferma_impostazione;
int scelta;
bool fine_gioco;

enum Nome_Giocatore {Bianco, Giallo, Nero, Rosso, Verde, Marrone, Ciano, Rosa, Arancione, Blu};
enum Stato_Giocatore {Astronauta, Impostore, Assassinato, Defenestrato};
enum Tipo_Stanza {Vuota, Quest_Semplice, Quest_Complicata, Botola};

struct Giocatore
{
  struct Stanza *Posizione;
	enum Stato_Giocatore Stato;
	enum Nome_Giocatore Nome;
};

struct Stanza
{
	struct Stanza *Avanti;
	struct Stanza *Destra;
	struct Stanza *Sinistra;
	struct Stanza *StanzaPrecedente;
  struct Stanza *Next;
  enum Tipo_Stanza Tipo;
  bool chiamata_emergenza;
};

void Imposta_Gioco();
void Gioca();
void Termina_Gioco();
