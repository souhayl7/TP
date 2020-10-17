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

  char buf[256];
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

  /* Creation de la socket UDP */
  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Probleme lors de la creation de socket\n");
    exit(1);
  }

  /* Bind sur la socket */
  if(bind(sd, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
    printf("Probleme avec le bind\n");
    exit(1);
  }


  printf("Serveur en ecoute...\n");

  i = 0;
  while(1) {
 
  int addrlen = sizeof(sa);
  n = recvfrom(sd,buf,sizeof(buf),0,(struct sockaddr*)&sa,&addrlen);
  sendto(sd,buf,n,0,(struct sockaddr*)&sa,addrlen);
    }


  /* Fermeture du serveur. Never reached */

  printf("fermeture du serveur \n");
  close(sd);
  exit(0);
}
