#include <iostream>
#include <fstream>
#include <vector>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include "map.hpp"
#include "ArgParseStandalone.h"


using namespace std;
using namespace game;


void quit()
{
  endwin();
}

#define MSG_BOX_STYLE 1
void msgbox(string msg) {
  WINDOW* w = newwin(3,msg.length()+2,5,10);
  PANEL* p = new_panel(w);
  wattron(w, COLOR_PAIR(MSG_BOX_STYLE));
  box(w,0,0);
  mvwaddstr(w,1,1,msg.c_str());
  wattroff(w, COLOR_PAIR(MSG_BOX_STYLE));
  refresh();
  update_panels();
  doupdate();
  int c = 'c';
  do {
    c = getch();
  } while((c != ' ') && (c != '\n') && (c != 'e') && (c != 'E'));
  del_panel(p);
  delwin(w);
}

int main(int argc, char** argv) {
  ArgParse::ArgParser parser("Bulldozer");
  
  bool create_new_level = false;
  string new_level_editor = "nano";
  int start_level = 1;
  
  parser.AddArgument(
                      "-n/--new-level",
                      "Creates a new level file and opens it in an editor.\n\t\tIt openes the level by default using the nano editor.",
                      &create_new_level,
                      ArgParse::Argument::Optional
                     );
  parser.AddArgument(
                      "-e/--editor",
                      "Changes the default level editor.",
                      &new_level_editor,
                      ArgParse::Argument::Optional
                     );
  parser.AddArgument(
                      "-l/--level",
                      "Starts the game at a given Level.",
                      &start_level,
                      ArgParse::Argument::Optional
                     );

  if(parser.ParseArgs(argc, argv)) {
    cout << "Argument Error" << endl;
    return -1;
  }
  
  if(parser.HelpPrinted()) {
    return 0;
  }
  
  if (create_new_level) {
    cout << "neu level kommt noch..." << endl;
    int newLevelNr = 1;
    string path = "./level/" + to_string(newLevelNr) + ".txt";
    for(bool run = true; run;) {
      path = "./level/" + to_string(newLevelNr) + ".txt";
      ifstream f(path);
      run = f.good();
      if (run) newLevelNr++;
    }
    ofstream nf(path);
    nf << 31 << endl << 15 << endl;
    for(int i = 0; i < 15; i++)
      nf << "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" << endl;
    system(((new string(new_level_editor + " " + path))->c_str()));
    return 0;
  }
  
  atexit(quit);
  
  try {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_game();
    init_pair(MSG_BOX_STYLE, COLOR_WHITE, COLOR_BLUE);
    
    int currentLevel = start_level;
    
    msgbox("Enter to start...");
    
    Map* m = new Map();
    if(!m->load("./level/" + to_string(currentLevel) + ".txt")) {
      msgbox("No levels found :(");
      return 0;
    }
    m->init();
  
    bool run = true;
    do {
      mvaddstr(0,1,("Level: " + to_string(currentLevel)).c_str());
      m->update();
      refresh();
      update_panels();
      doupdate();
      keypad(stdscr, true);
      
      if(m->checkComplete()) {
        msgbox("Level complete! [Enter] to continue ...");
        currentLevel++;
        delete m;
        m = new Map();
        if(!m->load("./level/" + to_string(currentLevel) + ".txt")) {
          msgbox("All levels finished :D");
          return 0;
        }
        m->init();
        
      } else {  
        int input = getch();
        
        switch(input) {
          case 'w':
          case 'W':
          case KEY_UP:
              m->moveEntities(0,-1);
            break;
          case 'a':
          case 'A':
          case KEY_LEFT:
              m->moveEntities(-1,0);
            break;
          case 's':
          case 'S':
          case KEY_DOWN:
              m->moveEntities(0,1);
            break;
          case 'd':
          case 'D':
          case KEY_RIGHT:
             m->moveEntities(1,0);
            break;
          case 'q':
          case 'Q':
              run = false;
            break;
          case 'r':
          case 'R':
          case 127: //Backspace
              delete m;
              m = new Map();
              m->load("./level/" + to_string(currentLevel) + ".txt");
              m->init();
            break;
        }
      }
    } while(run);
    
  } catch (int ex) {
    endwin();
    throw ex;
  }
  
  return 0;
}
