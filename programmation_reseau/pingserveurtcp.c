#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

main(argc, argv)
int argc;
char *argv[];
{
  int sd;
  struct sockaddr_in sa;	/* Structure Internet sockaddr_in */
  struct hostent *hptr; 	/* Infos sur le serveur */
  int port;        		/* Numero de port du serveur */

  int newsd;			/* Id de la socket entrante */
  struct sockaddr_in newsa;	/* sockaddr_in de la connection entrante */
  int newsalength;
  char msg[256];
  int i,n;


  /* Verification du nombre d'arguments de la ligne de commande */
  if (argc != 2) {
    printf("pingserveurTCP. Erreur d'arguments\n");
    printf("Syntaxe : %% pingserveurTCP numero_port\n");
    exit(1);
  }

  /* Recuperation numero port passe en argument */
  port = atoi(argv[1]);

  /* Initialisation la structure sockaddr sa */
  /* Famille d'adresse : AF_INET = PF_INET */
  sa.sin_family = AF_INET;

  /* Initialisation du numero du port */
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = INADDR_ANY;

  /* Creation de la socket TCP */
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Probleme lors de la creation de socket\n");
    exit(1);
  }

  /* Bind sur la socket */
  if(bind(sd, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
    printf("Probleme avec le bind\n");
    exit(1);
  }

  /* Initialisation de la queue d'ecoute des requetes (5 max) */
  listen(sd, 5); 

  printf("Serveur en ecoute...\n");

  i = 0;
  while(1) {

    /* newsalength contient la taille theorique de la structure newsa */
    newsalength = sizeof(newsa) ;
    if((newsd = accept(sd, ( struct sockaddr * ) &newsa, &newsalength)) < 0 ) {
      printf("Erreur sur accept\n");
      exit(1);
    }

    /* Compteur nombre connexions */
    i++;
    printf("Connection No %d sur le port %d...\n", i, ntohs(newsa.sin_port));

    /* Tempo pour pouvoir faire un netstat... */
   /* sleep(10); */
   
   strcpy(msg,"enter");
   while(strncmp(msg,"quit",4) != 0) { 
    char chaine[256] ="attente d'un message ... \n";
    write(1,chaine,strlen(chaine));
    n = read(newsd,msg,sizeof(msg));
    if (strncmp(msg,"quit",4) != 0) {
      write(newsd, msg, n);
      }
    
    }
     close(newsd);
  } 

  /* Fermeture du serveur. Never reached */
  write(1,"fermeture du serveur \n",strlen("fermeture du serveur \n"));
  close(sd);

  printf("Fin du serveur. Bye...\n");
  exit(0);
}
