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
  bg.h = 210;
  SDL_FillRect(io.getScreen(), &bg, SDL_MapRGBA(io.getScreen()->format, 100,100,100,70));
  io.printMessageCenteredAt(
    gdata->getXmlStr("caption"),
    gdata->getXmlInt("messageY"));
  io.printMessageCenteredAt(
    gdata->getXmlStr("helpText"),
    gdata->getXmlInt("helpY"));
  io.printMessageCenteredAt("Tracking "+objectToTrack->getName(), 30);
  io.printMessageCenteredAt("Press ESC to go back to menu", 70);
  
  io.printMessageAt("Luigi", 475, 110);
  io.printMessageAt("Move: a d", 400, 130);
  io.printMessageAt("Jump: w", 400, 150);
  io.printMessageAt("Pick/Shoot Pokeball: q", 400, 170);
  io.printMessageAt("Sprint: e", 400, 190);

  io.printMessageAt("Mario", 100, 110);
  io.printMessageAt("Move: Left/Right arrow keys", 50, 130);
  io.printMessageAt("Jump: Up arrow key", 50, 150);
  io.printMessageAt("Pick/Shoot Pokeball: /", 50, 170);
  io.printMessageAt("Sprint: RSHIFT", 50, 190);
//  io.printMessageCenteredAt("Luigi:  Move using a d w keys | Pick/Shoot Pokeball using q | Sprint using e", 90);
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
