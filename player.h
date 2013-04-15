#include "gamedata.h"
#include "frameFactory.h"
#include "twowayMultisprite.h"
#include "collisionStrategy.h"
#include "item.h"

class Player {
public:
  Player(const string&);
  virtual ~Player();

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  const Drawable* getSprite() const { return sprite; }
  float getDamage() const { return damage; }
  void damageIncr(float amount = 1.0) { damage += amount; }

  bool hasItem() const { return item!=NULL; }
  void setItem(Item* it) { item=it; }
  Item* getItem() const { return item; }
  bool collideWith(const Drawable* d);
  void stop();
  void right();
  void left();
  void up();
  void down();
  void leftaccelerate();
  void rightaccelerate();
  void jump();

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
  Item* item;
  std::vector<Frame*> frames;
  std::vector<Frame*> framesLeft;
  TwowayMultiframeSprite* sprite;
  std::string name;
  void makeSprite();
  Player(const Player&);
  Player& operator=(const Player&);
};


