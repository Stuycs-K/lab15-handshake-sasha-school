#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  printf("making server\n");
  from_client = server_handshake( &to_client );
}
