#ifndef MULTI_CURSES
#define MULTI_CURSES

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <ncurses.h>
#include "defs.h"
#include "CursesWindow.h"

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

// message type
enum {
     TYPE_MSG = 1,
     TYPE_QUIT
};


// window specifiers
enum windowtype {
     WINDOW_INPUT = 1,
     WINDOW_OUTPUT
};

// properties
DWORD PROP_CLEAR = 0x0001;
DWORD PROP_TOP = 0x0002;
DWORD PROP_BOTTOM = 0x0004;
DWORD PROP_DEL = 0x0008;

bool master = false;

typedef struct _textmsg {
     long mtype;
     enum windowtype mwindow;
     char mtext[200];
     DWORD mproperties;
} textmsg;

void InitCurses();
CursesWindow InitializeScreen(int startx, int starty, int width, int height, bool separator);
void HandleScreen(int _msqid);
void* outputProc(void* param);
int InitializeConnections(pthread_t* hOutput, int* msqid);
int SendMessage(int msqid, textmsg* msg);
void SetTextMsg(textmsg* msg, long type, enum windowtype window, char* text, int properties);
void HandleMasterInput(pthread_t hOutput, int msqid);
void HandleSlaveInput(int msqid);

#endif
