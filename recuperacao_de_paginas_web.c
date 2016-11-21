/*$Id: $*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /*gethostbyname()*/

#define PORT 80

char email[1024];
char msg[1024];
char host[1024];

void connectToSocket ()
{
  int sockfd, is_connected;
  struct sockaddr_in server_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if ( sockfd < 0)
  {
    printf("Erro: Nao foi possivel obter descritor do socket.\n");
    exit(1);
  }
  
  server = gethostbyname(host);
  if (!server) {
    printf("Erro: Host %s nao existe", host);
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("Erro: Nao foi possivel se conectar no Host %s, na porta %d", host, PORT);
  }

  int bytes_sent = send(sockfd, msg, strlen(msg), 0);
  printf("bytes_sent: %d\n", bytes_sent);
  printf("msg_sent: %s\n", msg);
  
  char resp[4096];
  int bytes_recv = recv(sockfd, resp, sizeof(resp), 0);
  printf("\n\nmsg_recv: %s\n", resp);

  return;
}

void setMsg (char *path) {
  strncpy(email, "gabriella@aker.com", 18);
  strncpy(msg, "GET ", 4);
  strncat(msg, path, strlen(path));
  strncat(msg, " HTTP/1.0\nFrom: ", 16);
  strncat(msg, email, strlen(email));
  strncat(msg, "\nUser-Agent: HTTPTool/1.0\n\n", 27);
}


int main (int argc, char *argv[]) {
  FILE *fp;
  

  /* TODO Verificar entrada "./r 1 -s"*/
  if (argc < 3) { 
    printf("Informe <URL> <nome do arquivo> <-s para sobrescrever arquivo>\n");
    exit(1);
  }
  
  fp = fopen(argv[2], "r+");
  /*Arquivo ja existe, e flag de sobrescrita foi informada*/
  if (fp) {
    if (argv[3] && strncmp(argv[3], "-s", 2) != 0) {
      printf("Arquivo ja existe e nao pode ser sobrescrito.\n");
      printf("Para sobreescreve-lo, adicione nos parametros a flag -s.\n");
      exit(1);
    }
    /*pode sobreescrever*/
  }
  else {
    fp = fopen(argv[2], "a+");
  }
  
  setMsg(argv[1]); /* TODO tirar host*, deixar soh path*/
  
  //strcpy(host, argv[1]);
  
  strncpy(host, "www.google.com", 16);

  connectToSocket();
  
  
  fclose(fp);
  return 0;
}
