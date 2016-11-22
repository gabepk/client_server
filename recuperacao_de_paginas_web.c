/*$Id: $*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 80

FILE *fp;
char msg[1024];
char host[1024];

void connect_to_socket ()
{
  int sockfd, is_connected;
  struct sockaddr_in server_addr;
  struct hostent *server;


  /* Conexao com servidor */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if ( sockfd < 0)
  {
    printf("Erro: Nao foi possivel obter descritor do socket.\n");
    exit(1);
  }
  
  server = gethostbyname(host);
  if (!server) {
    printf("Erro: Host %s nao existe.\n", host);
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) 
      < 0) {
    printf("Erro: Nao foi possivel se conectar no Host %s, na porta %d.\n", 
           host, PORT);
  }


  /* Envio e recebimento de mensagens */

  int bytes_sent, bytes_recv;
  int offset = 0;
  int i, j=0, total;
  char content_length[4];
  char buffer[200];
 
  bytes_sent = send(sockfd, msg, strlen(msg), 0);
  printf("Bytes sent: %d\n", bytes_sent);
  printf("Message sent: \n%s", msg);
  
  printf(".\nMessage received: \n");
  memset(buffer, 0, sizeof(buffer));
 
  bytes_recv = recv(sockfd, buffer, sizeof(buffer), 0);
  printf("%s <\n\n\n", buffer); 

  bytes_recv = recv(sockfd, buffer+100, sizeof(buffer), 0);
  printf("%s <", buffer+100); 

  do {
    bytes_recv = recv(sockfd, buffer, sizeof(buffer), 0);
    printf("$s <", buffer);
    if (bytes_recv < 0) {
      printf("Erro ao ler mensagem do socket.\n");
    }
    else if (bytes_recv == 0) {
      printf("\n\nFim da mensagem recebida.\n");
      break;
    }
    
    offset += bytes_recv;
    fprintf(fp, "> %s\n\n", buffer);
  } while (bytes_recv < strlen(buffer));

  close(sockfd);
  return;
}

void set_msg (char *path) {
  char length[4];

  snprintf(msg, 18+strlen(path), "GET %s HTTP/1.0\r\n\r\n", path);
  //snprintf(msg, 40, "GET %s HTTP/1.0\r\n\r\n", path);
  return;
}


int main (int argc, char *argv[]) {
  
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
  
  //set_msg(argv[1]);
  strncat(host, "server.aker.com.br", 20);
  set_msg("http://server.aker.com.br/intranet_aker/");
  connect_to_socket();
  

  fclose(fp);
  return 0;
}
