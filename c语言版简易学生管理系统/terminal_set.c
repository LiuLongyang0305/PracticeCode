#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "terminal_set.h"
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
void set_terminal_hide()
{
	struct termios ter_buf;
	while(tcgetattr(fileno(stdin),&ter_buf) != 0);
	ter_buf.c_lflag &= ~ECHOFLAGS;
	while(tcsetattr(fileno(stdin),TCSAFLUSH,&ter_buf) != 0);
}
void set_terminal_show()
{
	struct termios ter_buf;
	while(tcgetattr(fileno(stdin),&ter_buf) != 0);
	ter_buf.c_lflag |= ECHOFLAGS;
	while(tcsetattr(fileno(stdin),TCSAFLUSH,&ter_buf) != 0);
}
