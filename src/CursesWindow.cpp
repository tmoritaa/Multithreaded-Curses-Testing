#include "CursesWindow.h"

void CursesWindow::set(CursesWindow window)
{
     m_window = window.getWindow();
     m_startx = window.getStartx();
     m_starty = window.getStarty();
     m_width = window.getWidth();
     m_height = window.getHeight();
     m_cursorx = window.getCursorx();
     m_cursory = window.getCursory();
}

CursesWindow::CursesWindow()
{
     m_window = NULL;
     m_startx = 0;
     m_starty = 0;
     m_width = 0;
     m_height = 0;
     m_cursorx = 0;
     m_cursory = 0;
}

CursesWindow::CursesWindow(WINDOW* win, int startx, int starty, int width, int height)
{
     m_window = win;
     m_startx = startx;
     m_starty = starty;
     m_width = width;
     m_height = height;
     m_cursorx = 0;
     m_cursory = 0;
}

void CursesWindow::refresh()
{
     wrefresh(m_window);
}

void CursesWindow::clear()
{
     wclear(m_window);
}

void CursesWindow::move(int x, int y)
{
     m_cursorx = x;
     m_cursory = y;
     wmove(m_window, m_cursory, m_cursorx);
}

void CursesWindow::stdWrite(char* str)
{
     waddstr(m_window, str);
}

void CursesWindow::drawBottomLine()
{

     mvwhline(m_window, m_height - 1, 0, '-', m_width-1);
}

void CursesWindow::deleteChar()
{
     mvwaddch(m_window, m_cursory, m_cursorx - 1, ' ');
     wmove(m_window, m_cursory, m_cursorx - 1);
}

