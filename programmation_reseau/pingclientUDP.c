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

  char *serveur;        	/* Nom du serveur distant */
  char buf[256]; 		/* Buffer */
  int n, i;
  char msg[256];
  int FTPPORT=atoi(argv[2]); 	/* Numero du port du serveur UDP */


  
  

    /* Verification du nombre d'arguments de la ligne de commande */
  if (argc != 3) {
    printf("myftp. Erreur d'arguments\n");
    printf("Syntaxe : %% myftp nom_serveur_ftp\n");
    exit(1);
  }

  /* Recuperation nom du serveur */
  serveur = argv[1];

  /* Recuperation des infos sur le serveur dans /etc/hosts ou par DNS */
  if((hptr = gethostbyname(serveur)) == NULL) {
    printf("Probleme de recuperation d'infos sur le serveur\n");
    exit(1);
  }

  /* Initialisation la structure sockaddr sa */
  /* bcopy(void *source, void *destination, size_t taille); 		*/
  bcopy((char *)hptr->h_addr, (char*)&sa.sin_addr, hptr->h_length);

  /* Famille d'adresse : AF_INET = PF_INET */
  sa.sin_family = AF_INET;

  /* Initialisation du numero du port */
  sa.sin_port = htons(FTPPORT);

  /* Creation de la socket UDP */
  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Probleme lors de la creation de socket\n");
    exit(1);
  }
  
  n=read(0,msg,sizeof(msg));

  int addrlen = sizeof(sa);
  sendto(sd,msg,n,0,(struct sockaddr*)&sa,addrlen);

  n = recvfrom(sd,buf,sizeof(buf),0,(struct sockaddr*)&sa,&addrlen);
  write(1,buf,n);
  /* Fermeture de la socket */
  close(sd);

  exit(0);
}
