#include <ncurses.h>
#include <panel.h>
#include <vector>
#include <fstream>
#include "map.hpp"

using namespace std;
using namespace game;

#define WALL 'W', false, 3
#define STREET ' ', true, 2
#define BOLDER 4
#define STORAGE 5
#define TRUCK 6
void game::init_game() {
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_WHITE);
  init_pair(BOLDER, COLOR_WHITE, COLOR_BLACK);
  init_pair(STORAGE, COLOR_RED, COLOR_BLACK);
  init_pair(TRUCK, COLOR_GREEN, COLOR_BLACK);
}

Map::Map() {
  this->w = 0;
  this->h = 0;
  
  this->win = newwin(0, 0, 1, 1);
  this->pan = new_panel(this->win);
};

Map::Map(int w, int h) : Map::Map() {
  this->resize(w, h);
}

Map::~Map() {
  for(int x = 0; x < this->w; x++) {
    for(int y = 0; y < this->h; y++) {
      free(this->fields.at(x)->at(y));
    }
    free(this->fields.at(x));
  }
  for(vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++) {
    free(*it);
  }
}

void Map::init() {
  for(int x = 0; x < this->w; x++) {
    for(int y = 0; y < this->h; y++) {
      wattron(this->win,COLOR_PAIR(this->fields.at(x)->at(y)->style));
      mvwaddch(this->win, y, x, this->fields.at(x)->at(y)->icon);
      wattroff(this->win,COLOR_PAIR(this->fields.at(x)->at(y)->style));
    }
  }
  
  for (vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++) {
    (*it)->init();
  }
}

void Map::resize(int w, int h) {
  //Delete old Fields
  for(int x = 0; x < this->w; x++) {
    for(int y = 0; y < this->h; y++) {
      delete this->fields.at(x)->at(y);
    }
    this->fields.at(x)->clear();
    delete this->fields.at(x);
  }
  this->fields.clear();
  

  //resize Window
  this->w = w;
  this->h = h;
  wresize(this->win, h, w);
  
  //create new Fields
  for(int x = 0; x < w; x++) {
    vector<Field*>* nV = new vector<Field*>;
    for(int y = 0; y < h; y++) {
      nV->push_back(new Field('#', false));
    }
    this->fields.push_back(nV);
  }
}

Point Map::size() {
  Point result = {this->w, this->h};
  
  return result;
}

bool Map::load(string path) {
  ifstream file(path);
  if(!file.good()) return false;
  
  int w, h;
  file >> w;
  file >> h;
  this->resize(w,h);
  
  vector<Point> moveables;
  vector<char> moveableTypes;
  
  string line;
  for(int y = 0; file >> line; y++) {
    for(int x = 0; x < line.length(); x++) {
      switch(line[x]) {
        case 'W':
            this->fields.at(x)->at(y)->setData(WALL);
          break;
        case 'S':
            this->fields.at(x)->at(y)->setData(STREET);
            this->entities.push_back(new Storage(this,{x,y}));
          break;
        case 'B':
        case 'P':
            this->fields.at(x)->at(y)->setData(STREET);
            moveables.push_back({x,y});
            moveableTypes.push_back(line[x]);
          break;
        case '.':
            this->fields.at(x)->at(y)->setData(STREET);
          break;
        default:
            this->fields.at(x)->at(y)->setData(WALL);
      }
    }
  }
  
  for(int i = 0; i < moveables.size(); i++) {
    char Type = moveableTypes.at(i);
    Point pos = moveables.at(i);
    switch(Type) {
      case 'B':
          this->entities.push_back(new Bolder(this, pos));
        break;
      case 'P':
          this->entities.push_back(new Truck(this, pos));
        break;
    }
  }
  return true;
}

void Map::update() {
  redrawwin(this->win);
  for (vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++) {
    (*it)->update();
  }
}

void Map::moveEntities(int x, int y) {
  for(vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++) {
    Entity* e = *it;
    if (e->canMove) {
      e->move(x,y);
    }
  }
}

bool Map::checkComplete() {
  for(vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++) {
    Entity* e = *it;
    if(e->isWalkable) {
      Field* f = this->fields.at(e->pos.x)->at(e->pos.y);
      if(f->entity == 0)
        return false;
      if (f->entity == e)
        return false;
      else if (!(f->entity->canBePushed))
        return false;
    }
  }
  return true;
}

Field::Field(char icon, bool walkable) {
  this->icon = icon;
  this->walkable = walkable;
  this->entity = 0;
}

void Field::setData(char icon, bool walkable, int style) {
  this->icon = icon;
  this->walkable = walkable;
  this->style = style;
}

Entity::Entity(Map* parent) {
  this->parent = parent;
  this->pos = {0,0};
  this->style = 0;
  this->canMove = false;
  this->canBePushed = false;
  this->isWalkable = false;
  
  this->win = newwin(1, 1, this->pos.y, this->pos.x);
  this->pan = new_panel(this->win);
}

void Entity::init() {
  wattron(this->win, COLOR_PAIR(this->style));
  waddch(this->win, this->icon);
  wattroff(this->win, COLOR_PAIR(this->style));
  mvwin(this->win, this->pos.y + 1, this->pos.x + 1);
  Field* f = this->parent->fields.at(this->pos.x)->at(this->pos.y);
  f->walkable = this->isWalkable;
  if(!this->isWalkable) f->entity = this;
}

void Entity::update() {
  mvwin(this->win, this->pos.y + 1, this->pos.x + 1);
}

void Entity::move(int x, int y) {
  Field* newPos = this->parent->fields.at(this->pos.x + x)->at(this->pos.y + y);
  Field* curPos = this->parent->fields.at(this->pos.x)->at(this->pos.y);
  if(!newPos->walkable) {
    if(newPos->entity == 0) {
      return;
    } else if(newPos->entity->canBePushed && !this->canBePushed) {
      newPos->entity->move(x,y);
      if(!newPos->walkable) {
        return;
      }
    } else {
      return;
    }
  }
  curPos->walkable = true;
  curPos->entity = 0;
  this->pos.x += x;
  this->pos.y += y;
  newPos->walkable = this->isWalkable;
  newPos->entity = this;
}

Bolder::Bolder(Map* parent, Point position) : Entity::Entity(parent) {
  this->icon = 'O';
  this->style = BOLDER;
  this->pos = position;
  this->canBePushed = true;
}

Storage::Storage(Map* parent, Point position) : Entity::Entity(parent) {
  this->icon = '+';
  this->style = STORAGE;
  this->pos = position;
  this->isWalkable = true;
}

Truck::Truck(Map* parent, Point position) : Entity::Entity(parent) {
  this->icon = '#';
  this->style = TRUCK;
  this->pos = position;
  this->canMove = true;
}


