#include <vector>
#include <string>
#include <ncurses.h>
#include <panel.h>

using namespace std;

namespace game {

class Map;
class Field;
class Entity;
class Bolder;
class Storage;
class Truck;

void init_game();

struct Point {
  int x, y;
};

class Map {
private:
  int w, h;
  WINDOW* win;
  PANEL* pan;
  void resize(int w, int h);

public:
  vector<vector<Field*>*> fields;
  vector<Entity*> entities;
  
  Map();
  Map(int w, int h);
  ~Map();
  
  void init();

  Point size();
  
  bool load(string path);
  
  void update();
  
  void moveEntities(int x, int y);
  
  bool checkComplete();
};

class Field {
public:
  char icon;
  bool walkable;
  int style;
  Entity* entity;
  
  Field(char icon, bool walkable);
  
  void setData(char icon, bool walkable, int style);
};

class Entity {
private:
  Map* parent;
  WINDOW* win;
  PANEL* pan;
  
public:
  char icon;
  Point pos;
  int style;
  bool canMove;
  bool canBePushed;
  bool isWalkable;
  
  Entity(Map* parent);
  
  void init();
  
  void update();
  
  void move(int x, int y);
};

class Bolder: public Entity {
public:
  Bolder(Map* parent, Point position);
};

class Storage: public Entity {
public:
  Storage(Map* parent, Point position);
};

class Truck: public Entity {
public:
  Truck(Map* parent, Point position);
};

}