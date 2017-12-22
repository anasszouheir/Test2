#include "common_impl.h"


int main(int argc, char **argv)
{
  int i;

  /* processus intermediaire pour "nettoyer" */
  /* la liste des arguments qu'on va passer */
  /* a la commande a executer vraiment */

  int port_num = atoi(argv[1]);

  char *hostname_dsmexec = malloc(sizeof(char) * strlen(argv[2]));
  strcpy(hostname_dsmexec, argv[2]);

  //printf("%s\n", hostname_dsmexec);

  char **const newargv = malloc(sizeof(char*)*argc-3);

  for (i =0; i < argc-3; i++){
    newargv[i] = malloc(sizeof(char ) * strlen(argv[i+3]));
    strcpy(newargv[i], argv[i+3]);
  }


  /* creation d'une socket pour se connecter au */
  /* au lanceur et envoyer/recevoir les infos */
  /* necessaires pour la phase dsm_init */



  int sfd = init_sock_addr(argv[1], hostname_dsmexec);
  printf("yo");

/*
  char * str = malloc(sizeof(char*));
  sprintf(str, "%d", a);


  int fp = open("testPR.txt",O_RDWR);

  write(fp,str,strlen(str));
*/


  //int sock = socket(AF_INET,SOCK_STREAM,0); !!!!!!!!!!!!!!!!!


  /* Test avec fichier */

//int fp = open("testPR.txt",O_RDWR);

  //write(fp,hostname_dsmexec,strlen(hostname_dsmexec));





  //do_connect(sock,sin_dsmexec); !!!!!!!!!!!!!!





  //int c = connect(sock, (struct sockaddr *)sin_dsmexec);
  //printf("%d\n", c);


  /* Envoi du nom de machine au lanceur */

  /* Envoi du pid au lanceur */

  /* Creation de la socket d'ecoute pour les */
  /* connexions avec les autres processus dsm */

  /* Envoi du numero de port au lanceur */
  /* pour qu'il le propage à tous les autres */
  /* processus dsm */

  /* on execute la bonne commande */
  return 0;
}
