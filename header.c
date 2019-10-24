#include "header.h"

/*headers (standard C)*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>



/*define keys */
#define KEY_ENTER   0x000a
#define KEY_ESCAPE  0x001b

/*Maximum size of the buffer*/
#define buffer_max_size 1024

static struct termios term, oterm;

/* function handle putchar with termios */
static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);


static int getch(void)
{
    int c = 0;
    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;
    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;
    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;
    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}
char *readline(void){
  int c;
  int buffer_index =0;
  int buffer_size = buffer_max_size;
  char *buffer = malloc(buffer_size);
  while (1) {
      c = kbget();
      if(c == 4){
          exit(EXIT_FAILURE);
      }else if(c == KEY_ENTER){
          buffer[buffer_index] = '\0';
          return buffer;
      }else if (c == KEY_ESCAPE) {
          exit(EXIT_FAILURE);
      }else{
            buffer[buffer_index] = c;
            buffer_index++;
          }
          putchar(c);

      /* reallocate the buffer size to avoid buffer  overflows */
      if(buffer_index>=buffer_size){
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
        }
      }
      return 0;
    }