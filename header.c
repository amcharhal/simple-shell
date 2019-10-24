#include "header.h"

/*headers (standard C)*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/*define macros to move cursor and delete*/
#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define deletecursor(x) printf("\033[%dK", (x));

/*define keys */
#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define HOME        0x0113
#define END         0x0111
#define BACKSPACE   127
#define DELETE      0x0092
#define TILDE       126
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define MAX_SIZE   200

/*Maximum size of the buffer*/
#define buffer_max_size 1024
static struct termios term, oterm;

/* function handle putchar with termios */
static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

/* for history commands storing */
static char *history_feature[MAX_SIZE];
static int length_buffer[MAX_SIZE];
static int position =0;

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
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'H':
                c = HOME;
                break;
            case 'F':
                c = END;
                break;
            case 127:
                c = BACKSPACE;
                break;
            case 51:
              c = DELETE;
              break;
            case 126:
              c = TILDE;
              break;
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
  int cursorbackward_index = 0;
  int command_position =position;
  while (1) {
      c = kbget();
      if(c == 4){
          exit(EXIT_FAILURE);
      }else if(c == KEY_ENTER){
        buffer[buffer_index] = '\0';
        cursorbackward_index = 0;
        history_feature[position] = buffer;
        length_buffer[position] = buffer_index;
        position++;
        return buffer;
      }else if (c == KEY_ESCAPE) {
          exit(EXIT_FAILURE);
      }else if( c == HOME){
            if(cursorbackward_index<buffer_index && cursorbackward_index>=0){
              cursorbackward(buffer_index-cursorbackward_index);
              cursorbackward_index = buffer_index;
              }
      }else if( c == END){
          if(cursorbackward_index<=buffer_index && cursorbackward_index>0){
            cursorforward(cursorbackward_index);
            cursorbackward_index =0;
          }
      }else if (c == KEY_RIGHT) {
          if(cursorbackward_index<buffer_index  && cursorbackward_index>=0){
            cursorbackward_index++;
            cursorbackward(1);
          }
      }else if (c == KEY_LEFT) {
            if(cursorbackward_index>0){
              cursorforward(1);
              cursorbackward_index--;
            }
      }else if (c == BACKSPACE ) {
        if(buffer_index!=0){
          if(cursorbackward_index!=0){
            if( buffer_index>cursorbackward_index){
              cursorbackward(1);
              deletecursor(0);
              for(int j=buffer_index-cursorbackward_index;j<buffer_index;j++)
                putchar(buffer[j]);
              for(int j=buffer_index-cursorbackward_index;j<buffer_index;j++)
                buffer[j-1]=buffer[j];
              cursorbackward(cursorbackward_index);
              buffer_index--;
            }
          }else{
            cursorbackward(1);
            deletecursor(0);
            buffer_index--;
          }
        }
      }else if (c == DELETE ) {
        c= getch();
        if(c == TILDE)
        if(buffer_index!=0){
          if(cursorbackward_index!=0){
            if( buffer_index>=cursorbackward_index){
              deletecursor(0);
              for(int j=buffer_index-cursorbackward_index+1;j<buffer_index;j++)
                putchar(buffer[j]);
              for(int j=buffer_index-cursorbackward_index+1;j<buffer_index;j++)
                buffer[j-1]=buffer[j];
              if(cursorbackward_index>1)
                cursorbackward(cursorbackward_index-1);
              cursorbackward_index--;
              buffer_index--;
            }
          }
        }
      }else if( c == KEY_UP) {
        if(position!=0 && command_position>0){
          deletecursor(1);
          cursorbackward(buffer_max_size);
          putchar('$');
          putchar(' ');
          char *temp = history_feature[command_position-1];
          for(int k=0; k<length_buffer[command_position-1];k++){
            putchar(temp[k]);
          buffer[k]=temp[k];
          }
          buffer_index=length_buffer[command_position-1];
          command_position--;
        }
      }else if( c == KEY_DOWN) {
        if(position!=0 && command_position<position){
          deletecursor(1);
          cursorbackward(buffer_max_size);
          putchar('$');
          putchar(' ');
          char *temp = history_feature[command_position+1];
          for(int k=0; k<length_buffer[command_position+1];k++){
            putchar(temp[k]);
            buffer[k]=temp[k];
            }
          buffer_index=length_buffer[command_position+1];
          command_position++;
          }
      }else {
          if(cursorbackward_index == 0){
            buffer[buffer_index] = c;
            buffer_index++;
          }else{
            buffer[buffer_index-cursorbackward_index] = c;
            cursorbackward_index--;
          }
          putchar(c);
      }
      /* reallocate the buffer size to avoid buffer  overflows */
      if(buffer_index>=buffer_size){
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
      }

  }
  return 0;
}
void excute(char *command){
  if (!strcmp(command, "exit")){
    exit(EXIT_FAILURE);
  }else if(!strcmp(command, "help")){
    printf("Mohamed amcharhal. simple shell prompt to return the buffer and handle keyboard keys\n");
  }else{
    printf("Sorry This command line is not yet implemented \n");
    }
}
