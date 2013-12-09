#ifndef CURSES_WINDOW
#define CURSES_WINDOW

#include <ncurses.h>
#include "defs.h"

class CursesWindow {
protected:
     WINDOW* m_window;
     int m_startx;
     int m_starty;
     int m_width;
     int m_height;
     int m_cursorx;
     int m_cursory;
public:
     WINDOW* getWindow() {
          return m_window;
     }
     int getStartx() {
          return m_startx;
     }
     int getStarty() {
          return m_starty;
     }
     int getWidth() {
          return m_width;
     }
     int getHeight() {
          return m_height;
     }
     int getCursorx() {
          return m_cursorx;
     }
     int getCursory() {
          return m_cursory;
     }
     void setWindow(WINDOW* window) {
          m_window = window;
     }
     void setStartx(int x) {
          m_startx = x;
     }
     void setStarty(int y) {
          m_starty = y;
     }
     void setWidth(int width) {
          m_width = width;
     }
     void setHeight(int height) {
          m_height = height;
     }
     void setCursorx(int curx) {
          m_cursorx = curx;
     }
     void setCursory(int cury) {
          m_cursory = cury;
     }
     void refresh();
     void clear();
     void move(int, int);
     void stdWrite(char*);
     void set(CursesWindow);
     void drawBottomLine();
     void deleteChar();
     CursesWindow(WINDOW*, int, int, int, int);
     CursesWindow();
};

#endif
