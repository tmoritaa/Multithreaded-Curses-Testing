/*******************************************************
problems:
- Nothing currently. Maybe the fact that it doesn't do anything.
- The way headers are organized kind of sucks.
*******************************************************/

#include "MultiCurses.h"
#include "CursesListWindow.h"

inline void InitCurses() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();	
}

CursesWindow InitializeScreen(int startx, int starty, int width, int height, bool separator) {
	WINDOW* tmpWin;

	tmpWin = newwin(height, width, starty, startx);

	class CursesWindow tmpWindow(tmpWin, startx, starty, width, height);

	if (separator) {
		tmpWindow.drawBottomLine();
	}

	tmpWindow.refresh();

	return tmpWindow;
}

void HandleScreen(int _msqid) {
	int msqid = _msqid;
	int width, height;
	int curx = 0, cury = 0;
	textmsg msg;

	InitCurses();

	pthread_mutex_lock(&count_mutex);
	pthread_cond_signal(&count_threshold_cv);
	pthread_mutex_unlock(&count_mutex);

	getmaxyx(stdscr, height, width);

	class CursesWindow inputWin, tmpWin;

	inputWin.set(InitializeScreen(0, (height/2), width, height/2, false));

	tmpWin.set(InitializeScreen(0, 0, width, height/2, true));
	class CursesListWindow outputWin(tmpWin);


	while(true) {
		if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), TYPE_QUIT, IPC_NOWAIT) == -1) {
			if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), TYPE_MSG, IPC_NOWAIT) == -1) {
				continue;
			}

			if (msg.mwindow == WINDOW_INPUT) {
				if ((msg.mproperties & PROP_CLEAR) == PROP_CLEAR) {
					inputWin.clear();
				}

				if((msg.mproperties & PROP_DEL) == PROP_DEL) {
					inputWin.deleteChar();	
				} else {

					if ((msg.mproperties & (PROP_TOP | PROP_BOTTOM)) == (PROP_TOP | PROP_BOTTOM)) {
						continue;
					} else if ((msg.mproperties & PROP_TOP) == PROP_TOP) {
						inputWin.move(0, 1);	
					} else if ((msg.mproperties & PROP_BOTTOM) == PROP_BOTTOM) {
						inputWin.move(0, inputWin.getHeight() - 1);
					}
		
					inputWin.stdWrite(msg.mtext);			
				}

				getyx(inputWin.getWindow(), cury, curx);

				inputWin.setCursorx(curx);
				inputWin.setCursory(cury);

				inputWin.refresh();

			} else if (msg.mwindow == WINDOW_OUTPUT) {
					
				outputWin.clear();

				outputWin.move(0, 0);

				outputWin.stdWrite(msg.mtext);

				getyx(outputWin.getWindow(), cury, curx);

				outputWin.setCursorx(curx);
				outputWin.setCursory(cury);

				outputWin.drawBottomLine();

				outputWin.refresh();			
			}
			

		} else {
			break;
		}
	}

	// cleaning up
	endwin();
	pthread_exit(NULL);
}

void* outputProc(void* param) {

	key_t key;
	int msqid;

	key = ftok("MultiCurses.cpp", 'B');
	
	if (key == -1) {
		perror("output key creation failed");
		pthread_exit(NULL);
	}

	if ((msqid = msgget(key, 0644)) == -1) {
		perror("output could not get key");
		pthread_exit(NULL);
	}

	HandleScreen(msqid);
}

int InitializeConnections(pthread_t* hOutput, int* msqid) {
	key_t key;

	key = ftok("MultiCurses.cpp", 'B');
	
	if (key == -1) {
		return 1;
	}

	if ((*msqid = msgget(key, 0644)) == -1) {

		if ((*msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
			return 1;	
		}

		pthread_mutex_init(&count_mutex, NULL);
		pthread_cond_init(&count_threshold_cv, NULL);

		pthread_create(hOutput, NULL, outputProc, NULL);
		master = true;
	}

	return 0;
}

int SendMessage(int msqid, textmsg* msg) {
	if (msgsnd(msqid, msg, sizeof(textmsg) - sizeof(long), 0) == -1) {
		perror("Error when sending");
		return 1;
	}

	return 0;
}

void SetTextMsg(textmsg* msg, long type, enum windowtype window, char* text, int properties) {
	memset((*msg).mtext, 0, strlen(text)+1);

	(*msg).mtype = type;
	(*msg).mwindow = window;
	strcpy((*msg).mtext, text);
	(*msg).mproperties = properties;
}

void HandleMasterInput(pthread_t hOutput, int msqid) {

	pthread_t* hAPI;
	int counter = 0;
	char d = 0;
	char inputTextBuffer[256] = {0};
	char outputTextBuffer[256] = {0};
	textmsg msg;

	memset(msg.mtext, 0, 200);

	
	while(true) {
		strcpy(inputTextBuffer, "Please enter Message\n");
		SetTextMsg(&msg, TYPE_MSG, WINDOW_INPUT, inputTextBuffer, PROP_TOP|PROP_CLEAR);
		
		if (SendMessage(msqid, &msg)) {
			break;
		}
		
		// get input using curses 
		while((d = getch()) != '\n') {
			if (d == 'q' && !counter) {
				// 3rd parameter is unnecessary
				SetTextMsg(&msg, TYPE_QUIT, (windowtype)0, "", NULL);

				if (SendMessage(msqid, &msg)) {
					pthread_kill(hOutput, SIGINT);		
				}

				goto OutLoop;

			} else if (d == 'Q') {
				if (strlen(outputTextBuffer) == 0) {
					continue;
				}
				
				SetTextMsg(&msg, TYPE_MSG, WINDOW_INPUT, "", PROP_DEL);

				if (SendMessage(msqid, &msg)) {
					break;
				}
				
				outputTextBuffer[--counter] = '\0';


			} else { 
				memset(inputTextBuffer, 0, strlen(inputTextBuffer));	
				inputTextBuffer[0] = d;
				outputTextBuffer[counter] = d;
				
				if (counter) {
					SetTextMsg(&msg, TYPE_MSG, WINDOW_INPUT, inputTextBuffer, 0); 
				} else {
					SetTextMsg(&msg, TYPE_MSG, WINDOW_INPUT, inputTextBuffer, PROP_BOTTOM);	
				}

				if (SendMessage(msqid, &msg)) {
					break;
				}

				counter++;

			}
		}

		SetTextMsg(&msg, TYPE_MSG, WINDOW_OUTPUT, outputTextBuffer, PROP_CLEAR | PROP_TOP);

		if (SendMessage(msqid, &msg)) {
			break;
		}

		memset(outputTextBuffer, 0, strlen(outputTextBuffer));
		counter = 0;
	}

	OutLoop:

	return;
}

void HandleSlaveInput(int msqid) {
	pthread_t* hAPI;
	char outputTextBuffer[256] = {0};
	textmsg msg;

	memset(msg.mtext, 0, 200);

	
	while(true) {
		printf("Please enter Message\n");
	
		fgets(outputTextBuffer, 255, stdin);

		if (!strcmp(outputTextBuffer, "q\n")) {
			goto OutLoop;
		} else {
			SetTextMsg(&msg, TYPE_MSG, WINDOW_OUTPUT, outputTextBuffer, PROP_CLEAR | PROP_TOP);

			if (SendMessage(msqid, &msg)) {
				break;
			}
		}

		memset(outputTextBuffer, 0, strlen(outputTextBuffer));
	}

	OutLoop:

	return;
}

int main() {
	pthread_t hOutput;
	int msqid;

	if	(InitializeConnections(&hOutput, &msqid)) {
		perror("Error in initialization");
		return 1;
	}

	if (master) {
		pthread_mutex_lock(&count_mutex);
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		pthread_mutex_unlock(&count_mutex);

		HandleMasterInput(hOutput, msqid);
	} else {
		HandleSlaveInput(msqid);
	}


	// cleaning up
	if (master) {
		pthread_join(hOutput, NULL);
		msgctl(msqid, IPC_RMID, NULL);
		pthread_mutex_destroy(&count_mutex);
		pthread_cond_destroy(&count_threshold_cv);
	}
	
	return 0;
}
