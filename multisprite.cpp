#include <iostream>
#include <cmath>
#include "multisprite.h"
#include "gamedata.h"

void MultiframeSprite::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

MultiframeSprite::MultiframeSprite( const std::string& name, 
                  const std::vector<Frame*> & fms, const float sc) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY")),
           sc
           ),
  frames(fms),
  frameWidth(fms[0]->getWidth()),
  frameHeight(fms[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") )
{ }

MultiframeSprite::MultiframeSprite(const MultiframeSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity(), s.getScale()),
  frames(s.frames),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval )
  { }

void MultiframeSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiframeSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

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
