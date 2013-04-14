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
/*
protected:
  unsigned currentFrame;
  unsigned numberOfFrames;
  int frameHeight;
  int frameWidth;
  int worldWidth;
  int worldHeight;

private:
  const std::vector<Frame *> frames;
  float dt;
  unsigned frameInterval;
  void advanceFrame(Uint32 ticks);
*/
MultiframeSprite::MultiframeSprite( const std::string& name, 
                  const std::vector<Frame*> & fms, const float sc) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY")),
           sc
           ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameHeight(fms[0]->getHeight()),
  frameWidth(fms[0]->getWidth()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  frames(fms),
  dt(0),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") )
{ }

MultiframeSprite::MultiframeSprite(const MultiframeSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity(), s.getScale()),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameHeight(s.getFrame()->getHeight()),
  frameWidth(s.getFrame()->getWidth()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  frames(s.frames),
  dt(s.dt),
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
