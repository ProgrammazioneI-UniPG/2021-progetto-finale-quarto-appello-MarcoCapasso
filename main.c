#include <stdio.h>
#include "gamelib.h"

int main()
{
  do
  {
      if (!conferma_impostazione)
      {
        printf("Benvenuti a trannoi\n");
        printf("Degli astronauti sono intrappolati nello spazio profondo a bordo della loro navicella.\n");
        printf("Tra loro ci sono degli impostori, riusciranno gli astronauti a sopravvivere?\n");
        printf("\n1) Imposta gioco\n2) Gioca\n3) Termina gioco\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch(scelta)
        {
          case 1:
            Imposta_Gioco();
            break;
          case 2:
            printf ("Non hai ancora impostato la partita. Devi farlo prima di iniziare a giocare!\n");
            break;
          case 3:
            Termina_Gioco();
            break;
          default:
            printf("La scelta non è valida!\n");
            break;
        }
      }
      else if (conferma_impostazione && !fine_gioco)
      {
        printf("Hai impostato la partita, ora puoi:\n2) Giocare\n3) Terminare gioco\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch(scelta) {
          case 2:
            Gioca();
            break;
          case 3:
            Termina_Gioco();
            break;
          default:
            printf("La scelta non è valida!\n");
            break;
        }
      }
      else if (fine_gioco)
      {
        printf ("La partita è terminata, premere 3 per uscire\n");
        scanf("%d", &scelta);
        while (getchar()!='\n');
        switch (scelta)
        {
          case 3:
            Termina_Gioco();
            break;
          default:
            printf ("La scelta non è valida!\n");
            break;
        }
      }
  }
  while(scelta !=3);
}
