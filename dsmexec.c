#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common_impl.h"

adfsfs
/* variables globales */

/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL;

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;


void usage(void)
{
  fprintf(stdout,"Usage : dsmexec machine_file executable arg1 arg2 ...\n");
  fflush(stdout);
  exit(EXIT_FAILURE);
}

void sigchld_handler(int sig)
{
  /* on traite les fils qui se terminent */
  /* pour eviter les zombies */
}


int main(int argc, char *argv[])
{
  if (argc < 3){
    usage();
  } else {
    pid_t pid;
    int num_procs = 0;
    int i;

    /* Mise en place d'un traitant pour recuperer les fils zombies*/
    /* XXX.sa_handler = sigchld_handler; */

    /* lecture du fichier de machines */
    char *machine_file = argv[1];
    FILE *machine_file_ptr = fopen(machine_file, "r");

    /* 1- on recupere le nombre de processus a lancer */
    char *machine_file_buffer = malloc(sizeof(char) * 100);
    while (fscanf(machine_file_ptr, "%s", machine_file_buffer) != EOF){
      num_procs++;
    }

    rewind(machine_file_ptr); // remet le pointeur du fichier au debut

    /* 2- on recupere les noms des machines : le nom de */
    /* la machine est un des elements d'identification */
    char **machine_names = malloc(sizeof(char *) * num_procs); // machine_names contient les noms des machines

    for (i = 0; i<num_procs; i++){
      bzero(machine_file_buffer,strlen(machine_file_buffer));
      fscanf(machine_file_ptr, "%s", machine_file_buffer);
      machine_names[i] = malloc(sizeof(char ) * strlen(machine_file_buffer));
      strcpy(machine_names[i],machine_file_buffer);
    }

    /* creation de la socket d'ecoute */
    int *port_num = malloc(sizeof(int));
    int sockhost_fd = creer_socket(0, port_num);

    /* + ecoute effective */
    listen(sockhost_fd, num_procs);

    /* Declaration des tubes */
    //declaration de matrices de taille 2*num_procs
    int **pipe_stdout = malloc(sizeof(*pipe_stdout) * num_procs);
    int **pipe_stderr = malloc(sizeof(*pipe_stderr) * num_procs);
    for(i = 0; i< num_procs; i++){
      pipe_stdout[i] = malloc(sizeof(**pipe_stdout)*2);
      pipe_stderr[i] = malloc(sizeof(**pipe_stdout)*2);
    }

    /* creation des fils */
    for(i = 0; i < num_procs ; i++) {

      /* creation du tube pour rediriger stdout */
      pipe(pipe_stdout[i]);

      /* creation du tube pour rediriger stderr */
      pipe(pipe_stderr[i]);

      pid = fork();
      if(pid == -1) ERROR_EXIT("fork");

      if (pid == 0) { /* fils */
        int j;
        /* redirection stdout */

/*
        close(pipe_stdout[i][0]); // fermeture de la fd de lecture
        close(STDOUT_FILENO);
        int newfd_stdout = dup(pipe_stdout[i][1]); //pipe_stdout[i][1} devient la sortie standard ||| newfd_stdout est la sortie standard initiale dupliquée
        close(pipe_stdout[i][1]); //fermeture de la fd ancienne dupliquée

        /* redirection stderr */

/*
        close(pipe_stderr[i][0]); // meme chose
        close(STDERR_FILENO);
        int newfd_stderr = dup(pipe_stderr[i][1]);
        close(pipe_stderr[i][1]);


        /* Creation du tableau d'arguments pour le ssh */
        char **const newargv = malloc(sizeof(char*)*(argc+4)); //argc car : machine, dsmwrap, truc w les arguments dialo thirno


        newargv[0] = malloc(sizeof(char ) * (strlen("ssh")+1)); //+1 pour prendre en compte "\0" à la fin de la chane de caractere
        strcpy(newargv[0], "ssh");


        newargv[1] = malloc(sizeof(char ) * (strlen(machine_names[i])+1));
        strcpy(newargv[1], machine_names[i]);


        newargv[2] = malloc(sizeof(char ) * (strlen("/net/t/azouheir/PR204/Phase1/bin/dsmwrap")+1));
        strcpy(newargv[2], "/net/t/azouheir/PR204/Phase1/bin/dsmwrap");


        char *char_port_num = malloc(sizeof(char)*20);
        sprintf(char_port_num,"%d", *port_num);
        newargv[3] = malloc(sizeof(char ) * (strlen(char_port_num)+1));
        strcpy(newargv[3], char_port_num);

/*
        char *hostname = malloc(sizeof(char ) * 1024);
        gethostname(hostname, 1024);
        newargv[4] = malloc(sizeof(char ) * (strlen(hostname)+1));
        newargv[4][strlen(hostname)] = '\0';
        strcpy(newargv[4], hostname);
*/

        char hostname[1024];
        hostname[1023] = '\0';
        gethostname(hostname, 1023);
        newargv[4] = malloc(sizeof(char ) * (strlen(hostname)+1));
        strcpy(newargv[4], hostname);

        for(j = 5; j < argc+3 ;j++){
          newargv[j] = malloc(sizeof(char ) * (strlen(argv[j-3])+1));
          strcpy(newargv[j], argv[j-3]);
        }
        // verify this + taille de argv dans malloc
        //newargv[argc+4]  = NULL;

        //////////////// a gerer apres
        //strcpy(newargv[0], "ssh");
        ////////////////

        /* TEST */

/*
        for(j = 0; j<argc+4;j++){
          printf("%d %s\n",i,newargv[j]);
        }
*/


/* 7yed had l9lwa ou non ?
        for(j = 0; j<argc+3;j++){
          bzero(newargv[j],strlen(newargv[j]));
        }
*/

        /* jump to new prog : */

        int err_exec = execvp("/usr/bin/ssh",newargv);
        if (err_exec)
          printf("error on execvp\n");

        /* fermetrure des fd fils ecriture */
        close(pipe_stdout[i][1]);
        close(pipe_stderr[i][1]);
        exit(0) ;
      } else  if(pid > 0) { /* pere */
        /* fermeture des extremites des tubes non utiles */
        close(pipe_stdout[i][1]);
        //wait(NULL);
        //READ

        num_procs_creat++;
      }
    }


    for(i = 0; i < num_procs ; i++){

      /* on accepte les connexions des processus dsm */
      struct sockaddr_in *sin_client = malloc(sizeof(struct sockaddr_in));

      do_accept(sockhost_fd,sin_client);


      //mazal majerbnach accept, 9ad ssh bach tssifet


      /*  On recupere le nom de la machine distante */
      /* 1- d'abord la taille de la chaine */
      /* 2- puis la chaine elle-meme */

      /* On recupere le pid du processus distant  */

      /* On recupere le numero de port de la socket */
      /* d'ecoute des processus distants */
    }

    /* envoi du nombre de processus aux processus dsm*/

    /* envoi des rangs aux processus dsm */

    /* envoi des infos de connexion aux processus */

    /* gestion des E/S : on recupere les caracteres */
    /* sur les tubes de redirection de stdout/stderr */
    /* while(1)
    {
    je recupere les infos sur les tubes de redirection
    jusqu'à ce qu'ils soient inactifs (ie fermes par les
    processus dsm ecrivains de l'autre cote ...)

  };
  */

  /* on attend les processus fils */

  /* on ferme les descripteurs proprement */

  /* on ferme la socket d'ecoute */
}
exit(EXIT_SUCCESS);
}
