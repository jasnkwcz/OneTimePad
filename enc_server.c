#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define BUFFSIZE 1024
#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
//function for outputting an error message using perror
void error(char* msg)
{
  fprintf(stderr, "Error: %s\n", msg);
  exit(EXIT_FAILURE);
}

//convert a character c into its corresponding integer index value in CHARS
int ctoi(char c)
{
  //char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  char find = c;
  char* ptr = strchr(CHARS, find);
  int index = ptr - CHARS;
  return index;
}

//convert an integer into its corresponding character value in CHARS
char itoc(int i)
{
  char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  return(chars[i]);
}


//encrypt a message using plaintext p and key k sent by the enc_client program, modify ciphertext c to be encoded message
void encrypt(char* p, char* k, char** c)
{
  int pch;  //plaintext character converted to int
  int kch;  //key character converted to int
  int cch;  //cipher character converted to int
  int clen = strlen(p); //the length of the plaintext limits the size of the ciphertext

  //iterate over the plaintext, putting encrypted characters in the ciphertext array one at a time
  for (int curr = 0; curr < clen; curr++)
  {
    cch = (ctoi(pch) + ctoi(kch) % 27));
  }
  return;
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
      error("Error: could not accept connection to client socket.\n");
    }

    //when a connection is recieved, create a new process with fork()
    pid = fork();
    if (pid < 0)
    {
      error("Error in creating child process to serve client.\n");
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
        error("Did not recieve valid header from client.\n");
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
      //strip off the trailing newline at the end of the plaintext
      char* remove = strstr(plaintext, "\n");
      strcpy(remove, "\0");


      //reset the buffer for the next recieve
      memset(buffer, '\0', BUFFSIZE);
      //recieve size of key from enc_client
      key_size = atoi(buffer);

      if(plaintext_size == 0)
      {
        error("Did not recieve valid header from client.\n")
      }

      //recieve key from enc_client
      key = (char*)calloc(key_size, sizeof(char));
      memset(buffer, '\0', BUFFSIZE);

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
        strcat(key, buffer);
        memset(buffer, '\0', BUFFSIZE);
      }
      remove = strstr(key, "\n");
      strcpy(remove, "\0");

      //verify that key is at least as large as plaintext. if not, error and terminate connection
      if (key_size < plaintext_size)
      {
        error("Key is not large enough to encrypt plaintext.\n")
      }

      //encode the data using the OTP algorithm
      ciphertext = (char*)calloc(plaintext_size, sizeof(char));
      encrypt(plaintext, key, &ciphertext);

      //send processsend encoded data as response to client


      //close client connection

      
      //free memory and terminate the child process
      free(ciphertext);
      free(key);
      free(plaintext);
      free(buffer);
    }

  }
  //close the socket

  return;
}