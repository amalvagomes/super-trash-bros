#include <string>
#include <iostream>

#include "multisprite.h"

class PlayerDeath : public MultiframeSprite {
public:
  PlayerDeath(const std::string&, const std::vector<Frame*>&);
  PlayerDeath(const PlayerDeath& s);
  virtual ~PlayerDeath() { } 
  PlayerDeath& operator=(const PlayerDeath&);
  virtual void update(Uint32 ticks);
  bool isDead() const { return dead; }

private:
  bool dead;
  float timeSinceLastFrame;
  int deathInterval;
  void advanceFrame(Uint32 ticks);
};
