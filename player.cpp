#include "player.h"
#include "ioManager.h"

Player::~Player() {
  for (unsigned i = 0; i < frames.size(); ++i) {
    delete frames[i];
    delete framesLeft[i];
  }
  delete sprite;
}

Player::Player() :
  gdata( Gamedata::getInstance() ),
  keyPressed(false),
  worldWidth( gdata->getXmlInt("worldWidth") ), 
  worldHeight( gdata->getXmlInt("worldHeight") ), 
  jumpCounter( 0 ),
  jumpTicks( gdata->getXmlInt("marioJumpTicks") ), 
  initialVelocity( Vector2f(gdata->getXmlInt("marioSpeedX"), 
                               gdata->getXmlInt("marioSpeedY") )
  ), 
  width( gdata->getXmlInt("marioWidth") ), 
  height( gdata->getXmlInt("marioHeight") ),
  frames(),
  framesLeft(),
  sprite( NULL )
{ 
  makeSprite();
}

void Player::makeSprite() {
  FrameFactory& frameFact = FrameFactory::getInstance();
  float scale;
  std::vector<Frame*> right = frameFact.getFrameVector("mario", &scale);
  std::vector<Frame*> left = frameFact.getFrameVector("marioLeft", &scale);
  sprite = new TwowayMultiframeSprite("mario", left, right, scale);
}

void Player::update(Uint32 ticks) { 
  if (!keyPressed) stop();
  keyPressed = false;
  sprite->update(ticks); 
}

void Player::stop() { 
  sprite->velocityX(0);  
  sprite->velocityY(0);  
}

void Player::right() { 
  keyPressed = true;
  if ( sprite->X() < worldWidth-width) {
    sprite->velocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  keyPressed = true;
  if ( sprite->X() > 0) {
    sprite->velocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  keyPressed = true;
  if ( sprite->Y() < Gamedata::getInstance()->getXmlInt("marioMinY") ) {
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

