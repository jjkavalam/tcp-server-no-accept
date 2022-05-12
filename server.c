/*
 * A TCP server that only listens; i.e. it does not accept any connections
 * Adapted from: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpserver.c
 *
 * It expects a port to listen on and a time to keep running for.
 *
 * This is useful to test connection timeout behavior of HTTP clients.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {

  int sleep_seconds; /* number of seconds to sleep before exiting */

  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */

  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  int optval; /* flag value for setsockopt */

  /*
   * check command line arguments
   */
  if (argc != 3) {
    fprintf(stderr, "usage: %s <port> <sleep_seconds>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);
  sleep_seconds = atoi(argv[2]);


  /*
   * socket: create the parent socket
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr,
	   sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /*
   * listen: make this socket ready to accept connection requests
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
    error("ERROR on listen");

  fprintf(stderr, "Listening on port %d ... Now going to sleep for %d seconds\n", portno, sleep_seconds);
  sleep(sleep_seconds);

  printf("Bye\n");

}
