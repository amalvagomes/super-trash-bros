#include "gamedata.h"
#include "frameFactory.h"
#include "twowayMultisprite.h"
#include "collisionStrategy.h"

class Player {
public:
  Player(const string&);
  virtual ~Player();

  virtual void update(Uint32 ticks);
  virtual void draw() const { sprite->draw(); }
  const Drawable* getSprite() const { return sprite; }
  float getDamage() const { return damage; }

  void collideWith(const Drawable* d) const;
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
  Vector2f initialVelocity;
  float width;
  float height;
  float damage;
  CollisionStrategy* cStrat;
  std::vector<Frame*> frames;
  std::vector<Frame*> framesLeft;
  TwowayMultiframeSprite* sprite;
  std::string name;
  void makeSprite();
  Player(const Player&);
  Player& operator=(const Player&);
};


