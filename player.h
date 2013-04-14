#include "gamedata.h"
#include "frameFactory.h"
#include "twowayMultisprite.h"

class Player {
public:
  Player();
  virtual ~Player();

  virtual void update(Uint32 ticks);
  virtual void draw() const { sprite->draw(); }
  const Drawable* getSprite() const { return sprite; }

  void stop();
  void right();
  void left();
  void up();
  void down();

private:
  const Gamedata* gdata;
  bool keyPressed;
  int worldWidth;
  int worldHeight;
  int jumpCounter;
  int jumpTicks;
  Vector2f initialVelocity;
  float width;
  float height;
  std::vector<Frame*> frames;
  std::vector<Frame*> framesLeft;
  TwowayMultiframeSprite* sprite;
  void makeSprite();
  Player(const Player&);
  Player& operator=(const Player&);
};


