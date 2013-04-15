#include <iostream>
#include <cmath>
#include "item.h"
#include "gamedata.h"

unsigned Item::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frames[currentFrame]->getSurface()->pixels);
  return pixels[ ( y * frames[currentFrame]->getWidth() ) + x ];
}


void Item::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

Item::Item( const std::string& name, 
                  const std::vector<Frame*> & fms, const float sc) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(0.0,0.0),
           sc
           ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameHeight(fms[0]->getHeight()),
  frameWidth(fms[0]->getWidth()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  animated( false ),
  stopped( true ),
  released( false ),
  frames(fms),
  dt(0),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") )
{ }

Item::Item(const Item& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity(), s.getScale()),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameHeight(s.getFrame()->getHeight()),
  frameWidth(s.getFrame()->getWidth()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  animated( s.animated ),
  stopped( s.stopped ),
  released( s.released ),
  frames(s.frames),
  dt(s.dt),
  frameInterval( s.frameInterval )
  { }

void Item::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void Item::fall() { 
    animated=false;
    stopped=false;
    released=true;
    velocityX(0.0);
    velocityY(200.0);
}

void Item::stop() { 
    stopped=true;
    velocityX(0.0);
    velocityY(0.0);
}

void Item::toggleMovement() { 
    stopped=!stopped;
}

void Item::toggleAnimation() { 
    animated=!animated;
}

void Item::update(Uint32 ticks) { 
  if(animated) {
  advanceFrame(ticks);
  } else {
    currentFrame = 0;
  }

  float xDelta = velocityX();
  float yDelta = velocityY();
  if(stopped) {
    velocityY(0.0);
    velocityX(0.0);
  } else {
    velocityX(velocityX()*(1.0-(ticks*.001)));
    if(Y()<420) {
        float veloc = velocityY();
        velocityY((veloc<200.0)?veloc+(ticks*0.01):200.0);
    } else {
        velocityY(0.0);
    }
  }
  xDelta = velocityX() - xDelta;
  yDelta = velocityY() - yDelta;
  if(xDelta != 0.0 && yDelta != 0.0) {
    std::cout << "item - " << velocityX() << " " << velocityY() << std::endl;
  }

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
