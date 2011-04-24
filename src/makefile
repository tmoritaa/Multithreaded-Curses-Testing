all: MultiCurses

MultiCurses: CursesWindow.o CursesListWindow.o MultiCurses.o
	g++ CursesWindow.o CursesListWindow.o MultiCurses.o -o MultiCurses -lncurses -pthread

CursesWindow.o: CursesWindow.cpp
	g++ -c CursesWindow.cpp

CursesListWindow.o: CursesWindow.o CursesListWindow.cpp
	g++ -c CursesListWindow.cpp

MultiCurses.o: CursesWindow.o CursesListWindow.o MultiCurses.cpp
	g++ -c MultiCurses.cpp 

clean:
	rm -rf *.o MultiCurses
