#include <sstream>
#include "ioManager.h"
#include "viewport.h"
#include "clock.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  viewWidth(gdata->getXmlInt("viewWidth")), 
  viewHeight(gdata->getXmlInt("viewHeight")),
  worldWidth(gdata->getXmlInt("worldWidth")),
  worldHeight(gdata->getXmlInt("worldHeight")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL) 
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getFrame()->getWidth();
  objHeight = objectToTrack->getFrame()->getHeight();
}

void Viewport::draw() const {
  IOManager& io = IOManager::getInstance();
  SDL_Rect bg = SDL_Rect();
  bg.x = 10;
  bg.y = 5;
  bg.w = 620;
  bg.h = 110;
  SDL_FillRect(io.getScreen(), &bg, SDL_MapRGBA(io.getScreen()->format, 100,100,100,70));
  io.printMessageCenteredAt(
    gdata->getXmlStr("caption"),
    gdata->getXmlInt("messageY"));
  io.printMessageCenteredAt(
    gdata->getXmlStr("helpText"),
    gdata->getXmlInt("helpY"));
  io.printMessageCenteredAt("Tracking "+objectToTrack->getName(), 30);
//  io.printMessageCenteredAt("Move Mario using arrow keys", 70);
  io.printMessageCenteredAt("Mario:  Move using arrow keys | Pick/Shoot Pokeball using / | Sprint using RSHIFT", 70);
//  io.printMessageCenteredAt("Move Luigi using a s d w keys", 110);
  io.printMessageCenteredAt("Luigi:  Move using a s d w keys | Pick/Shoot Pokeball using q | Sprint using e", 90);

  Clock& clock = Clock::getInstance();
  io.printMessageValueAt("FPS: ",clock.getFps(), 15, 7);
}

void Viewport::update() {
  const float x = objectToTrack->X();
  const float y = objectToTrack->Y();
  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
