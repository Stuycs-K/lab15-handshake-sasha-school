#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  mkfifo("wkp",0666);
    printf("made wkp\n");
  int wkp = open("wkp", O_RDONLY); //blocks
  char temp[256];
  if (read(wkp, temp, sizeof(temp))){
    close(wkp);
      printf("closed wkp\n");
  }
  from_client = wkp;
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;
  from_client = server_setup();
  char temp[256];
  read(from_client, temp, sizeof(temp));
  *to_client = open(temp, O_RDWR); // unblock
    printf("opened pp\n");
  srand( time(NULL) );
  int random = rand()%10;
  printf("sending random %d\n",random);
  write(*to_client, &random, sizeof(random));
  int r;
  read(from_client, &r, sizeof(r));
  if (r==random+1){
    printf("handshake works\n");
  }
  else{
    printf("%d is not %d +1",r,random);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  char pp_name[256];
  snprintf(pp_name, sizeof(pp_name), "%d", getpid());
  mkfifo(pp_name,0666);
    printf("made pp\n");
  int wkp = open("wkp", O_RDWR); //unblocks
    printf("opened wkp\n");
  write(wkp, pp_name, sizeof(pp_name));
  int pp = open(pp_name, O_RDONLY); //blocks
    printf("opened pp\n");
  from_server = pp;
  int r1;
  if (read(pp, &r1, sizeof(int))){
    close(pp);
      printf("closed pp\n");
  }
  printf("recieved %d\n",r1);
  r1+=1;
  printf("sending rand+1 %d\n",r1);
  write(*to_server, &r1, sizeof(int));

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
