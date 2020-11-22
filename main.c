#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ "

int ctoi(char c)
{
  //char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  char find = c;
  char* ptr = strchr(CHARS, find);
  int index = ptr - CHARS;
  return index;
}

char itoc(int i)
{
  char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  return(chars[i]);
}

int main(void)
{
  int myctoi = ctoi('X');
  char myitoc = itoc(2);
  printf("myctoi: %d\nmyitoc: %c\n", myctoi, myitoc);
  return 0;
}