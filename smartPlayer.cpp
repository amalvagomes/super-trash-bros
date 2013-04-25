#include "smartPlayer.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartPlayer::SmartPlayer(const string& n, const Player& p, float dist) :
  Player(n),
  io(&IOManager::getInstance()), 
  enemy(p), 
  safeDistance(dist),
  currentMode(NORMAL),
  timeToChase(0) { }

SmartPlayer::SmartPlayer(const SmartPlayer& rhs) :
  Player(rhs),
  io(rhs.io),
  enemy(rhs.enemy),
  safeDistance(rhs.safeDistance),
  currentMode(rhs.currentMode),
  timeToChase(rhs.timeToChase) { }

SmartPlayer& SmartPlayer::operator=(const SmartPlayer& rhs) {
  if ( this == &rhs ) return *this;
  sprite->X( rhs.sprite->X() );
  sprite->Y( rhs.sprite->Y() );
  sprite->velocityX( rhs.sprite->velocityX() );
  sprite->velocityY( rhs.sprite->velocityY() );
  io = &IOManager::getInstance(); 
  currentMode = NORMAL;
  return *this;
}

void SmartPlayer::draw() const {
  Player::draw();
}

void SmartPlayer::update(Uint32 ticks) {
  Player::update(ticks);
  float x= sprite->X()+sprite->getFrame()->getWidth()/2;
  float y= sprite->Y()+sprite->getFrame()->getHeight()/2;
  float ex= enemy.getSprite()->X()+enemy.getSprite()->getFrame()->getWidth()/2;
  float ey= enemy.getSprite()->Y()+enemy.getSprite()->getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) {
      currentMode = EVADE;
      timeToChase = 1000;
    }
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance || timeToChase<0) {
      currentMode=NORMAL;
      sprite->velocityX((-1.0)*sprite->velocityX());
      sprite->velocityY((-1.0)*sprite->velocityY());
    } else {
      timeToChase -= ticks;
      if ( x < ex ) this->right();
      if ( x > ex ) this->left();
      if ( y < ey ) this->down();
      if ( y > ey ) this->up();
    }
  }
}
