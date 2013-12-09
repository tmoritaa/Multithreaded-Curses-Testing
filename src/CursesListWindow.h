#ifndef CURSES_LIST_WINDOW
#define CURSES_LIST_WINDOW

#include <string.h>
#include <stdlib.h>
#include "CursesWindow.h"

class CursesListWindow: public CursesWindow {
private:
     int m_maxlist;
     char** m_strlist;
     int m_listsize;
public:
     int getMaxList() {
          return m_maxlist;
     }
     char** getStrList() {
          return m_strlist;
     }
     int getListSize() {
          return m_listsize;
     }
     void setStrList(char** strlist) {
          m_strlist = strlist;
     }
     void setListSize(int size) {
          m_listsize = size;
     }
     void stdWrite(char*);
     CursesListWindow();
     CursesListWindow(CursesWindow window);
     ~CursesListWindow();
};

#endif
