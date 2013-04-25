#include <iostream>
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include <limits>
using std::cout; using std::endl;

Sprite::Sprite(const std::string& name, const Frame* fm, const float sc) :
  Drawable(name,
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(
                    (rand()%2?1:-1)*
                      getRand(10, Gamedata::getInstance()->getXmlInt(name+"SpeedX")),
                    (rand()%2?1:-1)*
                      getRand(10, Gamedata::getInstance()->getXmlInt(name+"SpeedY"))
           ),
           sc
  ), 
  frameWidth(fm->getWidth()),
  frameHeight(fm->getHeight()),
  frame(fm),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity(), s.getScale()), 
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  frame(s.frame),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  frame = rhs.frame;
  worldWidth = Gamedata::getInstance()->getXmlInt("worldWidth");
  worldHeight = Gamedata::getInstance()->getXmlInt("worldHeight");
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

unsigned Sprite::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frame->getSurface()->pixels);
  return pixels[ ( y * frame->getWidth() ) + x ];
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
  
}
