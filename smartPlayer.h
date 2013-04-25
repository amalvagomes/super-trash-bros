#include "player.h"
#include "ioManager.h"

class SmartPlayer : public Player {
public:
  SmartPlayer(const string&, const Player&, float);
  SmartPlayer(const SmartPlayer& rhs);

  SmartPlayer& operator=(const SmartPlayer&rhs);
  void incrSafeDistance() {++safeDistance; }
  void decrSafeDistance() { --safeDistance; }
  float getSafeDistance() { return safeDistance; }

  void draw() const;
  void update(Uint32 ticks);
  enum MODE {NORMAL, EVADE};
private:
  IOManager* io;	
  const Player& enemy;
  float safeDistance;
  MODE currentMode;
  int timeToChase;
};
