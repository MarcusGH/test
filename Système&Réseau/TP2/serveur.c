#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define BUFFERSIZE 1024
int main(int argc, char *argv[]){ 

  int looptime = 0; /* Numero de la boucle */
  socklen_t ls = sizeof(struct sockaddr_in); /* Taille des adresses */
  /*---- Caracterisation de la socket d'émission ----------*/
  int socket_descriptor_server;                 /* Descripteur  */
  int port_serveur = 6666;    /* Port         */
  struct sockaddr_in adresse_serveur, *p_adresse_serveur = &adresse_serveur; /* Adresse  */
  /*---- Caracterisation de la socket distante ------*/
  struct sockaddr_in adresse_destinataire,*p_adresse_destinataire = &adresse_destinataire;  /* Adresse du destinataire */
  struct hostent *p_ip_client;       /* Adresse IP de la machine distante */
  /*---- Buffers pour Messages -------------------------------*/ 
  char msg_in[BUFFERSIZE] = "0";      /* Message recu de "0" a "99" */
  char msg_out[BUFFERSIZE] = "0";    /* Message a envoyer "0" a "99" */


  //UDP
  if ((socket_descriptor_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
      perror("[SOCK_DGRAM, AF_INET, 0]");
  }
  else{
    printf("socket [SOCK_DGRAM, AF_INET, 0] creee\n");
  }
  /*TCP
    if ((socket_descriptor_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
      perror("[SOCK_STREAM, AF_INET, 0]");
  }
  else{
    printf("socket [SOCK_STREAM, AF_INET, 0] creee\n");
  }*/ //TCP

  adresse_serveur.sin_family      = AF_INET;
  adresse_serveur.sin_addr.s_addr = htonl(INADDR_ANY);  /* Format reseau */
  adresse_serveur.sin_port        = htons(port_serveur);  /* Format reseau */

  if(bind(socket_descriptor_server,(struct sockaddr *)(p_adresse_serveur),ls) == -1) {
    perror("Attachement de la socket impossible");
    close(socket_descriptor_server);  /* Fermeture de la socket               */
    exit(2);       /* Le processus se termine anormalement.*/
  }

  /* d) Recuperation de l'adresse effective d'attachement. */
  getsockname(socket_descriptor_server,(struct sockaddr *)p_adresse_serveur,&ls);
  
  /* a) A partir du nom du destinataire */    
  p_ip_client=gethostbyname(argv[1]); //cote serveur sa sert a rien avant la premiere reception
  
  if(p_ip_client == NULL){ 
    fprintf(stderr,"machine %s inconnue\n",argv[1]); 
    exit(2); 
  }
  else{ // On renseigne la structure destinataire
    memcpy(&adresse_destinataire.sin_addr.s_addr, p_ip_client->h_addr, p_ip_client->h_length);
    adresse_destinataire.sin_family = AF_INET;
    adresse_destinataire.sin_port   = htons(port_serveur); // Meme port que socket_descriptor_server : why not ? 
    fprintf(stdout,"machine %s --> %s \n", p_ip_client->h_name, inet_ntoa(adresse_destinataire.sin_addr)); 
  }

  for(;;) { 
    int i;
    /* b) Reception */
    printf("Attente de reception ... ");
    if (recvfrom(socket_descriptor_server ,msg_in,sizeof(msg_in),0, (struct sockaddr *) p_adresse_destinataire, &ls) == -1)  
      printf("inachevee : %s !\n",msg_in);
    else  {
      printf("terminee : valeur = %s !\n",msg_in);
      /* c) Traitement : La reception est bonne, on fait evoluer i */
      i = atoi(msg_in); 
      i = (i+1)%100; 
      sprintf(msg_out,"%d",i);//on remplis le buffer
    }

    printf("\n\nEnvoi(%d) ... ", looptime);
    
    //envoi du message
    if (sendto(socket_descriptor_server ,msg_out,sizeof(msg_out),0,(struct sockaddr *)p_adresse_destinataire,ls) >0)
      printf("termine : valeur = %s !\n",msg_out);
    else
      printf("inacheve : %s !\n",msg_out);
  
    sleep(1);

    looptime++;
  }
}
