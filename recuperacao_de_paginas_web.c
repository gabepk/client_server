/*$Id: $*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 80

char email[2014];
char msg[1024];

void connectToSocket () {
  int sockfd;
  struct sockaddr_in server_addr;
  
  if (sockfd = (PF_INET, SOCK_STREAM, 0) == -1) {
    printf("Nao foi possivel obter descritor do socket.\n");
    exit(1);
  }
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("10.12.110.57");
  memset(&(server_addr.sin_zero), '\0', 8);
  
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    printf("Erro ao fazer o bind do socket com a porta %d.\n", PORT);
    exit(1);
  }
  
  /*bind: associa o sockete a uma porta no meu computador*/
  
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
    if (!argv[3] || strncmp(argv[3], "-s", 2) != 0) {
      printf("Arquivo ja existe e nao pode ser sobrescrito.\n");
      printf("Para sobreescreve-lo, adicione nos parametros a flag -s.\n");
      exit(1);
    }
    /*pode sobreescrever*/
  }
  else {
    fclose(fp);
    fp = fopen(argv[2], "a+");
  }
  
  setMsg(argv[1]); /* TODO tirar host*, deixar soh path*/
  printf("\n%s", msg);
  
  
  connectToSocket();
  
  
  fclose(fp);
  return 0;
}