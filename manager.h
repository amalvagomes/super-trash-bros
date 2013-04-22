#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "gamedata.h"
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "world.h"
#include "viewport.h"
#include "player.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void setNumberOfItems(int);

private:
  const bool env;
  const Gamedata* gdata;
  IOManager& io;
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
  SDL_Surface * const midbackSurface;
  Frame * const midbackFrame;
  World midbackWorld;

  SDL_Surface * const foreSurface;
  Frame * const foreFrame;
  World foreWorld;
  Viewport& viewport;
  SDL_Surface * const pokemonSurface;


  Player player;
  Player player2;
  bool playerPickup;
  bool player2Pickup;

  int currentStar;
  int itemTimer;
  int itemTime;
  bool displayHelpText;

  std::list<Drawable*> sprites;
  std::vector<Frame*> pokemonFrames;
  const unsigned int TICK_INTERVAL;
  Uint32 nextTime;

  void draw() const;
  Uint32 timeLeft();
  Manager(const Manager&);
  Manager& operator=(const Manager&);

  void makeItems();
  void makePokemon();
  void killPokeball();
  void makeNewItem();

  float getRand(float min, float max) {
    return min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min);
  }

  class DrawableComparator{
    public:
      bool operator() (Drawable * left, Drawable * right) const {
        return left->getScale() < right->getScale();
      }
  };
};
