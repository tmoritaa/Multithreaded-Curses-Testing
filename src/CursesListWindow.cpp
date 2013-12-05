#include "CursesListWindow.h"

CursesListWindow::CursesListWindow()
{
     m_maxlist = 0;
     m_strlist = NULL;
     m_listsize = 0;
}

CursesListWindow::CursesListWindow(CursesWindow window)
     : CursesWindow(window.getWindow(), window.getStartx(), window.getStarty(),
                    window.getWidth(), window.getHeight())
{
     m_maxlist = m_height-1;
     m_strlist = (char**) malloc(sizeof(char**)*m_maxlist);
     m_listsize = 0;

     for (int i = 0; i < m_maxlist; i++) {
          m_strlist[i] = (char*)malloc(sizeof(char*)*STRBUFFERSIZE);
          memset(m_strlist[i], 0, sizeof(m_strlist[i]));
     }
}

CursesListWindow::~CursesListWindow()
{
     for (int i = 0; i < m_maxlist; i++) {
          free(m_strlist[i]);
     }

     free(m_strlist);
}

void CursesListWindow::stdWrite(char* str)
{
     // separate to different function
     if (m_listsize < m_maxlist) {
          strcpy(m_strlist[m_listsize], str);
          m_listsize++;
     } else {
          for(int i = 1; i < m_maxlist; i++) {
               memset(m_strlist[i-1], 0, strlen(m_strlist[i-1]));
               strcpy(m_strlist[i-1], m_strlist[i]);
          }

          memset(m_strlist[m_maxlist-1], 0, strlen(m_strlist[m_maxlist-1]));
          strcpy(m_strlist[m_maxlist-1], str);

          m_listsize = m_maxlist;
     }

     m_cursorx = 0;
     m_cursory = 0;

     for (int i = 0; i < m_listsize; i++) {
          CursesWindow::move(m_cursorx, m_cursory);
          CursesWindow::stdWrite(m_strlist[i]);
          m_cursory++;
     }

}
