#include "header.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{   char *command;
    while (1) {
      printf("$ ");
      command = readline();
      putchar('\n');
      //printf("  %s\n",command);  // only to show the buffer
      excute(command);
  }
    return 0;
  }
