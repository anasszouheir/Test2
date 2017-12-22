#include "common_impl.h"



int init_sock_addr(char *port_num, char *hostname_dsmexec){

  struct sockaddr_in *sin_dsmexec = malloc(sizeof(struct sockaddr_in));

  memset( sin_dsmexec, '\0' , sizeof(*sin_dsmexec));
  sin_dsmexec->sin_family=AF_INET;
  sin_dsmexec->sin_port=htons(atoi(port_num));
  int err_in_at = inet_aton(hostname_dsmexec,& sin_dsmexec->sin_addr);
  if (err_in_at == -1)
    printf("erreur inet_aton\n");

  struct addrinfo *res;
  struct addrinfo hints;

  memset(&hints,0,sizeof(hints));

  hints.ai_family=AF_INET;
  hints.ai_socktype=SOCK_STREAM;
  int s = getaddrinfo(hostname_dsmexec,port_num,&hints,&res);
  if (s != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
      exit(EXIT_FAILURE);
  }

  struct addrinfo *rp;
  int sfd;
  for (rp = res; rp != NULL; rp = rp->ai_next) {
      sfd = socket(rp->ai_family, rp->ai_socktype,
                   rp->ai_protocol);

      if (sfd == -1)
          continue;

      if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){
          break;                  /* Success */
      close(sfd);
    }
  }
  if (rp == NULL) {               /* No address succeeded */
      fprintf(stderr, "Could not connect at %s with %s\n",port_num,hostname_dsmexec);
      exit(EXIT_FAILURE);
  }

  freeaddrinfo(res);

/*
  if (err_get_add_inf != 0)
    printf("err getaddinfo\n");
*/
  return(sfd);
}

int creer_socket(int type, int *port_num)
{
   int fd;

//
   /* fonction de creation et d'attachement */
   /* d'une nouvelle socket */
   /* renvoie le numero de descripteur */
   /* et modifie le parametre port_num */

   /* Initialisation de la structure sock_addr_in */
   struct sockaddr_in sin;
   //clean the serv_add structure
   memset(&sin,0,sizeof(sin));

   //internet family protocol
   sin.sin_family = AF_INET;

   //we bind to ny ip form the host
   sin.sin_addr.s_addr = INADDR_ANY;

   //we bind on the tcp port specified
   sin.sin_port = htons(0); //htons(int)

   socklen_t len = sizeof(sin);

   /* Creation de la socket */
   fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

   /*Bind de la socket */
   int f = bind(fd, (struct sockaddr *) &sin , sizeof(sin));

   getsockname(fd, (struct sockaddr *)&sin, &len);

   /* Modification du port_num */
   *port_num = sin.sin_port;

   return fd;
}

int do_connect(int sock,struct sockaddr_in *sin_dsmexec){
  socklen_t addrlen = sizeof(*sin_dsmexec);

  int ret;
  do{
    ret = connect(sock, (struct sockaddr *)sin_dsmexec, addrlen);
  } while ((-1 == ret) && (errno == EINTR));
  return (ret);

}

int do_accept(int sockhost_fd,struct sockaddr_in *sin_client){
  socklen_t sizeof_sin_client = sizeof(sin_client);
  int a = accept(sockhost_fd, (struct sockaddr *) sin_client ,&sizeof_sin_client);
  if (a == -1)
    printf("error on accepting\n");
}

/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */
