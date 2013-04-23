#include <vector>
#include <string>
#include "frame.h"
#include "ioManager.h"
#include "vector2f.h"
#include "viewport.h"

class Menu {
public:
  Menu();
  ~Menu();
  void draw() const;
  void lightOn() { click = 1; }
  void lightOff() { click = 0; }
  void increment() { nextIcon = (nextIcon + 1) % words.size(); }
  void decrement() { 
    nextIcon = (nextIcon + words.size()-1) % words.size(); 
  }
  const string& getIconClicked() const;
private:
  const IOManager& io;
  std::vector<std::string> words; 
  Vector2f position;
  Vector2f space;

  ParseXML parser;
  SDL_Surface* surfaceOff;
  SDL_Surface* surfaceOn;
  Viewport& viewport;
  std::vector<Frame> clicks;
  unsigned nextIcon;
  unsigned click;

  Menu(const Menu&);
  Menu& operator=(const Menu&);
};
