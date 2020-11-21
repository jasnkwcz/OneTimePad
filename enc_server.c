#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//function for outputting an error message using perror
void error(char* msg)
{
  fprintf(stderr, "Error: %s\n", msg);
  exit(EXIT_FAILURE);
}

//function to set up an address struct for the server socket


//encrypt a message using plaintext and key sent by the enc_client program, return the encrypted message
char* encrypt(char* plaintext, char* key)
{
  char* ciphertext;
  return ciphertext;
}


//build the address struct for the server listening socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}



void main(int argc, char* argv[])
{
  //initialize variables for socket addresses
  struct sockaddr_in server_addr, client_addr;
  int server_socket;

  //check for valid command, error and exit if no port is spec'd
  if (argc < 2)
  {
    error("Error: enc_server expects at least 2 arguments\n");
    exit(EXIT_FAILURE);
  }

  //create a listening socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    error("Error creating server listening socket\n");
  }
  setupAddressStruct(&server_addr, atoi(argv[1]));


  //bind the listening socket to the port specified in argv[1]
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    error("ERROR on binding");
  }

  //listen forever using that socket
  listen(server_socket, 5);

  //while listening (forever):
  while(1)
  {


    //wait for a connection request using accept()


    //when a connection is recieved, create a new process with fork()


    //in the child process:
    
    
      //recieve first msg from enc_client, containing size of plaintext


      //recieve plaintext msg from enc_client


      //recieve size of key from enc_client


      //recieve key from enc_client


      //verify that key is at least as large as plaintext. if not, error and terminate connection


      //encode the data using the OTP algorithm


      //send processsend encoded data as response to client


      //close client connection

      
      //terminate the child process

  }
  //close the socket

  exit(EXIT_SUCCESS);
}