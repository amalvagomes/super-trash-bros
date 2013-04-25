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
Player::Player(const Player& rhs) :
  sprite( rhs.sprite ),
  gdata( Gamedata::getInstance() ),
  keyPressed( rhs.keyPressed ),
  worldWidth( rhs.worldWidth ), 
  worldHeight( rhs.worldHeight ), 
  initialVelocity( rhs.initialVelocity ), 
  width( rhs.width ), 
  height( rhs.height ),
  damage( rhs.damage ),
  cStrat( rhs.cStrat ),
  item( rhs.item ),
  frames( rhs.frames ),
  framesLeft(rhs.framesLeft),
  name(rhs.name)
{ }

Player::Player(const string& n) :
  sprite( NULL ),
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
  item( NULL ),
  frames(),
  framesLeft(),
  name(n)
{ 
  makeSprite();
}

bool Player::collideWith(const Drawable* d) {
  if(cStrat->execute(*(static_cast<Drawable*>(sprite)), *d)) {
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
  keyPressed = false;
  if ( sprite->Y() < worldHeight-(sprite->getFrame()->getHeight()))
	sprite->velocityY( sprite->velocityY() + (10 * static_cast<float>(ticks) * .1) );
  
  Vector2f incr = sprite->getVelocity() * static_cast<float>(ticks) * 0.001;
  sprite->setPosition(sprite->getPosition() + incr);

  if ( sprite->Y() >= worldHeight-(sprite->getFrame()->getHeight())) {
      sprite->velocityY(0.0);
      sprite->Y(worldHeight - (sprite->getFrame()->getHeight()));
  }

  if(sprite->velocityX() > 0.0 || sprite->velocityX() < 0.0) {
    sprite->velocityX(sprite->velocityX()*0.9);
  }

  sprite->update(ticks); 
  if(hasItem()) {
    item->stop();
    item->X(sprite->X()+sprite->getFrame()->getWidth()/2);
    item->Y(sprite->Y()+sprite->getFrame()->getWidth()/2);
  }
}

void Player::draw() const { 
  sprite->draw(); 
  if(hasItem()) {
    item->draw();
  }
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
