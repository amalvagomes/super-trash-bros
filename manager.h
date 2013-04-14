#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "gamedata.h"
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
//#include "playerDeath.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const Gamedata* gdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;

  SDL_Surface * const backSurface;
  Frame * const backFrame;
  World backWorld;
  SDL_Surface * const midSurface;
  Frame * const midFrame;
  World midWorld;
  SDL_Surface * const midfrontSurface;
  Frame * const midfrontFrame;
  World midfrontWorld;
  SDL_Surface * const foreSurface;
  Frame * const foreFrame;
  World foreWorld;
  Viewport& viewport;

  Player player;
  Player player2;

  int currentStar;
//  std::vector<Frame*> deathFrames;
  bool displayHelpText;

  std::vector<Drawable*> stars;
  const unsigned int TICK_INTERVAL;
  Uint32 nextTime;

  void draw() const;
  Uint32 timeLeft();
  Manager(const Manager&);
  Manager& operator=(const Manager&);

  void makeShells();
//  void killPlayer();

  class DrawableComparator{
    public:
      bool operator() (Drawable * left, Drawable * right) const {
        return left->getScale() < right->getScale();
      }
  };
};
