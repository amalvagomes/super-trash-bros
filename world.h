#include "frame.h"
#include "viewport.h"

class World {
public:
  World(Frame* const frame, float sc, float fact = 1.0f) : 
      frame1(*frame), frame2(*frame), 
      frameWidth( frame1.getWidth() ),
      worldWidth( Gamedata::getInstance()->getXmlInt("worldWidth") ),
      factor(fact),
      scale(sc),
      viewX(0.0), viewY(0.0), 
      view(Viewport::getInstance()) 
  {}
  void update();
  void draw() const;
  float getScale() const { return scale; }
private:
  Frame const frame1;
  Frame const frame2;
  unsigned frameWidth;
  unsigned worldWidth;
  float factor;
  float scale;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
