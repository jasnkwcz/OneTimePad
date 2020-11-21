#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define BUFFSIZE 1024
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
  //initialize variables for socket addresses and message memory
  struct sockaddr_in server_addr, client_addr;
  int server_socket;
  int client_socket;
  socklen_t client_size;
  int plaintext_size;
  int key_size;
  int pid;
  char* buffer;
  char* plaintext;
  char* key;
  char* ciphertext;

  //check for valid command, error and exit if no port is spec'd
  if (argc < 2)
  {
    error("Error: enc_server expects at least 2 arguments.\n");
    exit(EXIT_FAILURE);
  }

  //create a listening socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    error("Error creating server listening socket.\n");
  }
  setupAddressStruct(&server_addr, atoi(argv[1]));


  //bind the listening socket to the port specified in argv[1]
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    error("ERROR on binding.\n");
  }

  //listen forever using that socket
  listen(server_socket, 5);

  //while listening (forever):
  while(1)
  {
    //wait for a connection request using accept()
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)sizeof(client_addr));
    if (client_socket < 0)
    {
      error("Error: could not accept connection to client socket.\n")
    }

    //when a connection is recieved, create a new process with fork()
    pid = fork();
    if (pid < 0)
    {
      error("Error in creating child process to serve client.\n")
    }

    //in the child process:
    if (pid == 0)
    {
      //buffer to receive data from client is stored here, then concat into full message
      buffer = (char*)calloc(BUFFSIZE, sizeof(char));
      //recieve first msg from enc_client, containing size of plaintext. allocate a block of memory to hold the plaintext
      recv(client_socket, buffer, BUFFSIZE, 0);
      plaintext_size = atoi(buffer);

      //validate the plaintext
      if(plaintext_size == 0)
      {
        error("Did not recieve valid header from client.\n")
      }

      plaintext = (char*)calloc(plaintext_size, sizeof(char));
      memset(buffer, '\0', BUFFSIZE);

      //recieve plaintext msg from enc_client until the newline character is reached
      while (strstr(buffer, "\n") == NULL)
      {
        recv(client_socket, buffer, BUFFSIZE - 1, 0);
        //check that all data recived is valid
        for(int i = 0; i < strlen(buffer); i++)
        {
          char curr = buffer[i];
          if (((int)curr != 32) && (((int)curr - 65) > 25))
          {
            error("Recieved invalid plaintext input from client.\n");
          }
        }
        //concatenate the contents of the buffer to the plaintext
        strcat(plaintext, buffer);
        memset(buffer, '\0', BUFFSIZE);
      }
      char* remove = strstr(buffer)
      //recieve size of key from enc_client


      //recieve key from enc_client


      //verify that key is at least as large as plaintext. if not, error and terminate connection


      //encode the data using the OTP algorithm


      //send processsend encoded data as response to client


      //close client connection

      
      //terminate the child process
    }

  }
  //close the socket

  exit(EXIT_SUCCESS);
}