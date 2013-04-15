#include <iostream>
#include <cmath>
#include "playerDeath.h"
#include "gamedata.h"
#include <limits>
using std::cout; using std::endl;

void PlayerDeath::advanceFrame(Uint32 ticks) {
	// Frame animation specialized for a dying crow (rip):
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > deathInterval) {
		if (currentFrame < numberOfFrames-1) {
		  ++currentFrame;
		}
		timeSinceLastFrame = 0;
	}
}

PlayerDeath::
PlayerDeath(const std::string& name, const std::vector<Frame*>& fm) :
  MultiframeSprite(name, fm, 1.0),
  dead(false),
  timeSinceLastFrame( 0 ),
  deathInterval(Gamedata::getInstance()->getXmlInt(name+"FrameInterval"))
{ }

PlayerDeath::PlayerDeath(const PlayerDeath& s) :
  MultiframeSprite(s),
  dead(s.dead),
  timeSinceLastFrame( 0 ),
  deathInterval(s.deathInterval)
{ }

PlayerDeath& PlayerDeath::operator=(const PlayerDeath& rhs) {
  //MultiframeSprite::operator=(rhs);
  dead = rhs.dead;
  std::cout << "FIX THIS" << std::endl;
  return *this;
}

void PlayerDeath::update(Uint32 ticks) { 
  if ( Y() + frameHeight  < worldHeight ) {
    MultiframeSprite::update(ticks);
    advanceFrame(ticks);
  }
  else {
      dead = true;
  }
}

