#include "player.h"
#include "ioManager.h"

Player::~Player() {
  for (unsigned i = 0; i < frames.size(); ++i) {
    delete frames[i];
    delete framesLeft[i];
  }
  delete sprite;
  delete cStrat;
}

Player::Player(const string& n) :
  gdata( Gamedata::getInstance() ),
  keyPressed(false),
  worldWidth( gdata->getXmlInt("worldWidth") ), 
  worldHeight( gdata->getXmlInt("worldHeight") ), 
  initialVelocity( Vector2f(gdata->getXmlInt(n+"SpeedX"), 
                               gdata->getXmlInt(n+"SpeedY") )
  ), 
  width( gdata->getXmlInt(n+"Width") ), 
  height( gdata->getXmlInt(n+"Height") ),
  damage( 0.0 ),
  cStrat( new PerPixelCollisionStrategy() ),
  frames(),
  framesLeft(),
  sprite( NULL ),
  name(n)
{ 
  makeSprite();
}

bool Player::collideWith(const Drawable* d) {
  if(cStrat->execute(*(static_cast<Drawable*>(sprite)), *d)) {
    //double signx = (sprite->velocityX()>0)?-1.0:1.0;
    //sprite->velocityX(sprite->velocityX()*-1.0 + (damage*signx));
    //sprite->X(sprite->X()+(signx*20.0));
    return true;
  }
  return false;
}

void Player::makeSprite() {
  FrameFactory& frameFact = FrameFactory::getInstance();
  float scale;
  std::vector<Frame*> right = frameFact.getFrameVector(name, &scale);
  std::vector<Frame*> left = frameFact.getFrameVector(name+"Left", &scale);
  sprite = new TwowayMultiframeSprite(name, left, right, scale);
}

void Player::update(Uint32 ticks) { 
  //if (!keyPressed) stop();
  keyPressed = false;
  if(sprite->velocityY() < 0.0 || sprite->velocityY() > 0.0) {
    sprite->velocityY(sprite->velocityY()*0.95);
  }
  if(sprite->velocityX() > 0.0 || sprite->velocityX() < 0.0) {
    sprite->velocityX(sprite->velocityX()*0.9);
  }
  sprite->update(ticks); 
}

void Player::stop() { 
  sprite->velocityX(0);  
  sprite->velocityY(0);  
}

void Player::right() { 
  if ( sprite->X() < worldWidth-width && sprite->velocityX()<200.0) {
    sprite->velocityX(sprite->velocityX()+initialVelocity[0]);
  }
  keyPressed = true;
} 
void Player::left()  { 
  if ( sprite->X() > 0 && sprite->velocityX()>-200.0) {
    sprite->velocityX(sprite->velocityX()-initialVelocity[0]);
    //sprite->velocityX(-initialVelocity[0]);
  }
  keyPressed = true;
} 
void Player::up() { 
  keyPressed = true;
  if ( sprite->Y()+sprite->getFrame()->getHeight()-20 < Gamedata::getInstance()->getXmlInt(name+"MinY") ) {
    sprite->velocityY( initialVelocity[1] );
  }
  else {
    sprite->velocityY(-initialVelocity[0]);  
  }
} 
void Player::down()  { 
  keyPressed = true;
  if ( sprite->Y() < worldHeight-height) {
    sprite->velocityY(initialVelocity[1]);
  }
} 


void Player::leftaccelerate() {
  keyPressed = true;
  if ( sprite->X() > 0) {
    sprite->velocityX(-initialVelocity[0]-500);
  }

  else if ( sprite->X() < worldWidth-width) {
      sprite->velocityX(initialVelocity[0]-500);
  }

}

void Player::rightaccelerate() {
  keyPressed = true;
  if ( sprite->X() > 0) {
    sprite->velocityX(-initialVelocity[0]+1000);
  }

  else if ( sprite->X() < worldWidth-width) {
      sprite->velocityX(initialVelocity[0]+1000);
  }

}

void Player::jump() {
  keyPressed = true;
  if(!sprite->velocityY())
    sprite->velocityY(-400);
}
