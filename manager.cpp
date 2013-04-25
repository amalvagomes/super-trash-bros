#include <cmath>
#include <algorithm>
#include "sound.h"
#include "manager.h"
#include "item.h"
#include "playerDeath.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  std::list<Drawable*>::iterator it = sprites.begin();
  while(it != sprites.end()) {
    delete *it;
    it++;
  }

for (unsigned i = 0; i < pokemonFrames.size(); ++i) {
    delete pokemonFrames[i];
  }
  SDL_FreeSurface(pokemonSurface);
  SDL_FreeSurface(screen);
  SDL_FreeSurface(midbackSurface);
  delete midbackFrame;
  SDL_FreeSurface(midfrontSurface);
  delete midfrontFrame;
  SDL_FreeSurface(midSurface);
  delete midFrame;
  SDL_FreeSurface(foreSurface);
  delete foreFrame;
  SDL_FreeSurface(backSurface);
  delete backFrame;
  SDL_FreeSurface(indicatorSurface);
  delete indicatorFrame;
  SDL_FreeSurface(indicatorLeftSurface);
  delete indicatorLeftFrame;
  SDL_FreeSurface(indicator2Surface);
  delete indicator2Frame;
  SDL_FreeSurface(indicator2LeftSurface);
  delete indicator2LeftFrame;
  delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  backSurface( io.loadAndSet(gdata->getXmlStr("backFile"), 
                gdata->getXmlBool("backTransparency")) ),
  backFrame(new Frame(backSurface,
                gdata->getXmlInt("backWidth"), 
                gdata->getXmlInt("backHeight"), 
                gdata->getXmlInt("backSrcX"), 
                gdata->getXmlInt("backSrcY")) 
  ),
  backWorld( backFrame, gdata->getXmlFloat("backScale"), 5.0),
  midSurface( io.loadAndSet(gdata->getXmlStr("midFile"), 
                gdata->getXmlBool("midTransparency")) ),
  midFrame(new Frame(midSurface,
                gdata->getXmlInt("midWidth"), 
                gdata->getXmlInt("midHeight"), 
                gdata->getXmlInt("midSrcX"), 
                gdata->getXmlInt("midSrcY")) 
  ),
  midWorld( midFrame, gdata->getXmlFloat("midScale"), 4.0 ),
  midfrontSurface( io.loadAndSet(gdata->getXmlStr("midfrontFile"), 
                gdata->getXmlBool("midfrontTransparency")) ),
  midfrontFrame(new Frame(midfrontSurface,
                gdata->getXmlInt("midfrontWidth"), 
                gdata->getXmlInt("midfrontHeight"), 
                gdata->getXmlInt("midfrontSrcX"), 
                gdata->getXmlInt("midfrontSrcY")) 
  ),
  midfrontWorld( midfrontFrame, gdata->getXmlFloat("midfrontScale"), 3.0 ),
  midbackSurface( io.loadAndSet(gdata->getXmlStr("midbackFile"),
                gdata->getXmlBool("midbackTransparency")) ),
  midbackFrame(new Frame(midbackSurface,
                gdata->getXmlInt("midbackWidth"),
                gdata->getXmlInt("midbackHeight"),
                gdata->getXmlInt("midbackSrcX"),
                gdata->getXmlInt("midbackSrcY"))
  ),
  midbackWorld( midbackFrame, gdata->getXmlFloat("midbackScale"), 2.0 ),
  foreSurface( io.loadAndSet(gdata->getXmlStr("boardwalkFile"), 
                gdata->getXmlBool("boardwalkTransparency")) ),
  foreFrame(new Frame(foreSurface,
                gdata->getXmlInt("boardwalkWidth"), 
                gdata->getXmlInt("boardwalkHeight"), 
                gdata->getXmlInt("boardwalkSrcX"), 
                gdata->getXmlInt("boardwalkSrcY")) 
  ),
  foreWorld( foreFrame, gdata->getXmlFloat("boardwalkScale"), 2.0 ),
  viewport( Viewport::getInstance() ),
  pokemonSurface( io.loadAndSet(gdata->getXmlStr("pokemonFile"),
		gdata->getXmlBool("pokemonTransparency")) ),
  player(std::string("mario")),
  player2(std::string("luigi"), player, 120.0),
  indicatorSurface( io.loadAndSet(gdata->getXmlStr("indicatorFile"), 
                gdata->getXmlBool("indicatorTransparency")) ),
  indicatorFrame(new Frame(indicatorSurface,
                gdata->getXmlInt("indicatorWidth"), 
                gdata->getXmlInt("indicatorHeight"), 
                gdata->getXmlInt("indicatorSrcX"), 
                gdata->getXmlInt("indicatorSrcY")) 
  ),
  indicator("indicator",indicatorFrame, 1.0 ),
  indicatorLeftSurface( io.loadAndSet(gdata->getXmlStr("indicatorLeftFile"), 
                gdata->getXmlBool("indicatorLeftTransparency")) ),
  indicatorLeftFrame(new Frame(indicatorLeftSurface,
                gdata->getXmlInt("indicatorLeftWidth"), 
                gdata->getXmlInt("indicatorLeftHeight"), 
                gdata->getXmlInt("indicatorLeftSrcX"), 
                gdata->getXmlInt("indicatorLeftSrcY")) 
  ),
  indicatorLeft("indicatorLeft",indicatorLeftFrame, 1.0 ),

  indicator2Surface( io.loadAndSet(gdata->getXmlStr("indicator2File"), 
                gdata->getXmlBool("indicator2Transparency")) ),
  indicator2Frame(new Frame(indicator2Surface,
                gdata->getXmlInt("indicator2Width"), 
                gdata->getXmlInt("indicator2Height"), 
                gdata->getXmlInt("indicator2SrcX"), 
                gdata->getXmlInt("indicator2SrcY")) 
  ),
  indicator2("indicator2",indicator2Frame, 1.0 ),
  indicator2LeftSurface( io.loadAndSet(gdata->getXmlStr("indicator2LeftFile"), 
                gdata->getXmlBool("indicator2LeftTransparency")) ),
  indicator2LeftFrame(new Frame(indicator2LeftSurface,
                gdata->getXmlInt("indicator2LeftWidth"), 
                gdata->getXmlInt("indicator2LeftHeight"), 
                gdata->getXmlInt("indicator2LeftSrcX"), 
                gdata->getXmlInt("indicator2LeftSrcY")) 
  ),
  indicator2Left("indicator2Left",indicator2LeftFrame, 1.0 ),
  playerPickup(false),
  player2Pickup(false),
  playerVictory(0),
  currentStar(0),
  itemTimer(gdata->getXmlInt("itemInterval")),
  itemTime(0),
  displayHelpText(false),
  sprites(),
  pokemonFrames(),
  TICK_INTERVAL( gdata->getXmlInt("tickInterval") ),
  nextTime(clock.getTicks()+TICK_INTERVAL),
  soundEnabled(true)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
  // We now reserve space for the sprites; thus, obviating 
  // a lot of copies, reallocations, and deletions:
  makeItems();
  makePokemon();
  sprites.sort(DrawableComparator());
  viewport.setObjectToTrack(player.getSprite());
  clock.pause();
}

void Manager::makePokemon() {
  unsigned numberOfFrames = gdata->getXmlInt("pokemonFrames");
  Uint16 pwidth = gdata->getXmlInt("pokemonWidth");
  Uint16 pheight = gdata->getXmlInt("pokemonHeight");
  Uint16 srcX = gdata->getXmlInt("pokemonSrcX");
  Uint16 srcY = gdata->getXmlInt("pokemonSrcY");

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * pwidth + srcX;
    pokemonFrames.push_back( 
      new Frame(pokemonSurface, pwidth, pheight, frameX, srcY) );
  }
}

void Manager::makeItems() {
  FrameFactory& frameFact = FrameFactory::getInstance();
  for(unsigned i = 0; i < (unsigned)gdata->getXmlInt("itemCount"); i++) {
    float scale;
//    std::cout << "Adding pokeball" << std::endl;
    std::vector<Frame*> frames = frameFact.getFrameVector("pokeball", &scale);
    sprites.push_back( new Item("pokeball", frames, 0.97));
  }
}

void Manager::makeNewItem(){
    FrameFactory& frameFact = FrameFactory::getInstance();
    float scale;
    std::vector<Frame*> frames = frameFact.getFrameVector("pokeball", &scale);
    sprites.push_back(new Item("pokeball", frames, 0.97));
}

void Manager::setSoundEnabled(bool p) {
    soundEnabled = p;
}

void Manager::setNumberOfItems(int number) {
  FrameFactory& frameFact = FrameFactory::getInstance();
//  int numberOfPokeballs=gdata->getXmlInt("itemCount");
  int numberOfPokeballs=sprites.size();
  if ( number > numberOfPokeballs ) {
    number = number - numberOfPokeballs;
    for (int i = 0; i < number; ++i) {
	float scale;
//    	std::cout << "Adding pokeball" << std::endl;
//	std::cout << "Sprite Size: " <<sprites.size()<<std::endl;
   	std::vector<Frame*> frames = frameFact.getFrameVector("pokeball", &scale);
    	sprites.push_back( new Item("pokeball", frames, 0.97));
    }
  }
  else {
    number = numberOfPokeballs - number;
    for (int i = 0; i < number; ++i) {
      delete sprites.back();
//      std::cout << "Deleting pokeball" << std::endl;
      sprites.pop_back();
    }
  }
}

void Manager::draw() const {
  bool backWorldDrawn = false;
  bool midWorldDrawn = false;
  bool midfrontWorldDrawn = false;
  bool midbackWorldDrawn = false;
  bool foreWorldDrawn = false;
  bool playerDrawn = false;
  bool player2Drawn = false;

  std::list<Drawable*>::const_iterator it = sprites.begin();
  while(it != sprites.end()) {
    if(!backWorldDrawn && (*it)->getScale() > backWorld.getScale()) {
      backWorld.draw();
      backWorldDrawn = true;
    }
    if(!midWorldDrawn && (*it)->getScale() > midWorld.getScale()) {
      midWorld.draw();
      midWorldDrawn = true;
    }
    if(!midfrontWorldDrawn && (*it)->getScale() > midfrontWorld.getScale()) {
      midfrontWorld.draw();
      midfrontWorldDrawn = true;
    }
    if(!midbackWorldDrawn && (*it)->getScale() > midbackWorld.getScale()) {
      midbackWorld.draw();
      midbackWorldDrawn = true;
    }
    if(!foreWorldDrawn && (*it)->getScale() > foreWorld.getScale()) {
      foreWorld.draw();
      foreWorldDrawn = true;
    }
    if(!playerDrawn && (*it)->getScale() > player.getSprite()->getScale()) {
      player.draw();
      playerDrawn = true;
    }
    if(!player2Drawn && (*it)->getScale() > player2.getSprite()->getScale()) {
      player2.draw();
      player2Drawn = true;
    }
    (*it)->draw();
    it++;
  }
  if(!backWorldDrawn) {
      backWorld.draw();
  }
  if(!midWorldDrawn) {
      midWorld.draw();
  }
  if(!midfrontWorldDrawn) {
      midfrontWorld.draw();
  }
  if(!midbackWorldDrawn) {
      midbackWorld.draw();
  }
  if(!foreWorldDrawn) {
      foreWorld.draw();
  }
  if(player.getSprite()->X() < viewport.X()){
	const_cast<Sprite&>(indicatorLeft).X(viewport.X());
	const_cast<Sprite&>(indicatorLeft).Y(player.getWorldHeight()-
		((viewport.X()-player.getSprite()->X())*0.5));
	indicatorLeft.draw();
  }
  if(player.getSprite()->X() > (viewport.X() + viewport.getViewWidth())){
	const_cast<Sprite&>(indicator).X((viewport.X() + viewport.getViewWidth())-
		indicator.getFrameWidth());
	const_cast<Sprite&>(indicator).Y(player.getWorldHeight() -
		(abs((viewport.X() + viewport.getViewWidth())-player.getSprite()->X())*0.5));
	indicator.draw();
  }
  if(player2.getSprite()->X() < viewport.X()){
	const_cast<Sprite&>(indicator2Left).X(viewport.X());
	const_cast<Sprite&>(indicator2Left).Y(player2.getWorldHeight() -
		((viewport.X() - player2.getSprite()->X())*0.5));
	indicator2Left.draw();
  }
  if(player2.getSprite()->X() > (viewport.X() + viewport.getViewWidth())){
	const_cast<Sprite&>(indicator2).X((viewport.X() + viewport.getViewWidth())-
		indicator2.getFrameWidth());
	const_cast<Sprite&>(indicator2).Y(player2.getWorldHeight() -
		(abs((viewport.X() + viewport.getViewWidth())-player2.getSprite()->X())*0.5));
	indicator2.draw();
  }
  if(!playerDrawn) {
      player.draw();
  }
  if(!player2Drawn) {
      player2.draw();
  }
  if(displayHelpText) {
    viewport.draw();
  }
  io.printDamage(player.getDamage(),70,400);
  io.printMessageAt("Mario:",70,380);
  io.printDamage(player2.getDamage(),520,400);
  io.printMessageAt("Luigi:",520,380);
  if(playerVictory!=0) {
    SDL_Rect bg = SDL_Rect();
    bg.x = 0;
    bg.y = 0;
    bg.w = 640;
    bg.h = 580;
    SDL_FillRect(io.getScreen(), &bg, SDL_MapRGBA(io.getScreen()->format,0,0,0,200));
    io.printMessageCenteredAt("GAME OVER",250);
    switch(playerVictory) {
      case 1:
        io.printMessageCenteredAt("Mario Wins!",290);
        break;
      case 2:
        io.printMessageCenteredAt("Luigi Wins!",290);
        break;
      default:
        io.printMessageCenteredAt("WHHUUUTTT??  Looks like you've tied... somehow...",290);
    }
  }
}

void Manager::play() {
  clock.unpause();
  SDL_Event* event = new SDL_Event();
  SDLSound sound;
  if(!soundEnabled) sound.toggleMusic();
  playerVictory = 0;
  bool done = false;
  bool keyCatch = false;
  int gameOverTimer = 5000;
  while ( ! done ) {
    draw();
    SDL_Flip(screen);
    ++clock;
    SDL_Delay(timeLeft());
    nextTime += TICK_INTERVAL;

    SDL_PollEvent(event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event->type ==  SDL_QUIT) { break; }
    if(event->type == SDL_KEYUP) { 
        keyCatch = false; 
        playerPickup = false; 
        player2Pickup = false; 
    }
    if(event->type == SDL_KEYDOWN) {
      switch ( event->key.keysym.sym ) {
        case SDLK_ESCAPE : clock.pause(); done = true; break;
        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
        case SDLK_F1     : {
          if (!keyCatch) {
            keyCatch = true;
            displayHelpText = !displayHelpText;
          }
          break;
        }
        case SDLK_F2     : {
          if (!keyCatch) {
            keyCatch = true;
            viewport.setObjectToTrack(player.getSprite());
          }
          break;
        }
        case SDLK_F3     : {
          if (!keyCatch) {
            keyCatch = true;
            viewport.setObjectToTrack(player2.getSprite());
          }
          break;
        }

        case SDLK_q      : {
          if (!keyCatch) {
            keyCatch = true;
            io.clearString();
            io.buildString(event);
            if(player2.hasItem()) {
                Item* item = player2.getItem();
                item->toggleMovement();
                item->toggleAnimation();
                player2.setItem(NULL);
//                item->velocityY(-350.0);
                float veloc = player2.getSprite()->velocityX();
                veloc += (veloc/abs(veloc))*300.0;
                item->velocityX(veloc);
                sprites.push_back(static_cast<Drawable*>(item));
                sprites.sort(DrawableComparator());
            } else {
                player2Pickup = true;
            }
            if(soundEnabled) sound[0];
          }
	  break;
	}
        case SDLK_SLASH  : {
          if (!keyCatch) {
            keyCatch = true;
            io.clearString();
            io.buildString(event);
            if(player.hasItem()) {
                Item* item = player.getItem();
                item->toggleMovement();
                item->toggleAnimation();
                player.setItem(NULL);
  //              item->velocityY(player.getSprite()->velocityY()-25.0);
                float veloc = player.getSprite()->velocityX();
                veloc += (veloc/abs(veloc))*200.0;
                item->velocityX(veloc);
                sprites.push_back(static_cast<Drawable*>(item));
                sprites.sort(DrawableComparator());
            } else {
                playerPickup = true;
            }
            if(soundEnabled) sound[0];
          }
	  break;
	}
	default         : {
            if (!keyCatch) {
              keyCatch = true;
              io.buildString(event);
              //break;
            }
	    break;
	}
      }
    }
    if(keystate[SDLK_LEFT]) {
       player.left();
    }
    if(keystate[SDLK_UP]) {
      player.jump();
    }
    if(keystate[SDLK_RIGHT]) {
      player.right();
    }
    if(keystate[SDLK_RSHIFT] && keystate[SDLK_LEFT]) {
      player.leftaccelerate();
    }
    if(keystate[SDLK_RSHIFT] && keystate[SDLK_RIGHT]) {
      player.rightaccelerate();
    }
    if(keystate[SDLK_a]) {
      player2.left();
    }
    if(keystate[SDLK_w]) {
      player2.jump();
    }
    if(keystate[SDLK_d]) {
      player2.right();
    }
    if(keystate[SDLK_e] && keystate[SDLK_a]) {
      player2.leftaccelerate();
    }
    if(keystate[SDLK_e] && keystate[SDLK_d]) {
      player2.rightaccelerate();
    }
    if(keystate[SDLK_SPACE]) {
      if (!keyCatch) {
      	keyCatch = true;
       	player.jump();
      }
    }
    

    Uint32 ticks = clock.getElapsedTicks();
    std::list<Drawable*>::iterator it = sprites.begin();
    while (it != sprites.end()) {
      Item* item = dynamic_cast<Item*>(*it);
      if(item != 0) {
        if(!item->isReleased()) { 
          if(++itemTime == itemTimer) {
            item->X((int)getRand(0.0,static_cast<float>(gdata->getXmlInt("worldWidth"))));
            item->Y(0);
            item->fall();
            itemTime = 0;
          }
        } else {
            if(playerPickup && player.collideWith((*it))) {
		item->setLastTouched(1);
                player.setItem(item);
                playerPickup = false;
                it = sprites.erase(it);
                continue;
            }
            if(player2Pickup && player2.collideWith((*it))) {
		item->setLastTouched(2);
                player2.setItem(item);
                player2Pickup = false;
                it = sprites.erase(it);
                continue;
            }
	    if(player.collideWith(*it) && (item->getLastTouched() == 2)){
		float veloc;
	        player.damageIncr(10.0);
		if(player.getSprite()->velocityX() < 0)
		        veloc = 30;
		else veloc = -30;
	        const_cast<Drawable*>(player.getSprite())->velocityX(veloc+(veloc*(player.getDamage()*0.1)));
		it = killPokeball(it);
		continue;
	    }
	    if(player2.collideWith(*it) && (item->getLastTouched() == 1)){
		float veloc;
	        player2.damageIncr(10.0);
		if(player2.getSprite()->velocityX() < 0)
		        veloc = 30;
		else veloc = -30;
	        const_cast<Drawable*>(player2.getSprite())->velocityX(veloc+(veloc*(player2.getDamage()*0.1)));
		it = killPokeball(it);
		continue;
	    }

        }
      }
      if(dynamic_cast<PlayerDeath*>(*it)){
	if(static_cast<PlayerDeath*>(*it)->isDead()){
	  makeNewItem();
	  it = sprites.erase(it);
	  sprites.sort(DrawableComparator());
	  continue;
        }

        if(player.collideWith(*it)){
	  player.damageIncr(.1);
	  float veloc = player.getSprite()->velocityX();
	  if(veloc == 0)
		veloc = 1000;
	  const_cast<Drawable*>(player.getSprite())->velocityX(veloc+(veloc*(player.getDamage()/50.0)));
        }

        if(player2.collideWith(*it)){
	  player2.damageIncr(.1);
	  float veloc = player2.getSprite()->velocityX();
	  if(veloc == 0)
		veloc = 1000;
	  const_cast<Drawable*>(player2.getSprite())->velocityX(veloc+(veloc*(player2.getDamage()/50.0)));
        }
      }
      (*it)->update(ticks);
      it++;
    }

    if(player.collideWith(player2.getSprite())) {
      if(player.getSprite()->X() > player2.getSprite()->X()) {
        float x1 = player.getSprite()->X();
        float x2 = player2.getSprite()->X();
        const_cast<Drawable*>(player.getSprite())->X(x1+10);
        const_cast<Drawable*>(player2.getSprite())->X(x2-10);
      } else {
        float x1 = player.getSprite()->X();
        float x2 = player2.getSprite()->X();
        const_cast<Drawable*>(player.getSprite())->X(x1-10);
        const_cast<Drawable*>(player2.getSprite())->X(x2+10);
      }
      if(abs(player.getSprite()->velocityX()) > abs(player2.getSprite()->velocityX())) {
        player2.damageIncr(2.0);
        float veloc = player.getSprite()->velocityX();
        const_cast<Drawable*>(player2.getSprite())->velocityX(veloc+(veloc*(player2.getDamage()/75.0)));
      } else {
        player.damageIncr(2.0);
        float veloc = player2.getSprite()->velocityX();
        const_cast<Drawable*>(player.getSprite())->velocityX(veloc+(veloc*(player.getDamage()/75.0)));
      }
    }
    if(player.getDamage() > gdata->getXmlFloat("p1LifeThreshold")) {
        if(gameOverTimer<0) {
          player.setDamage(0.0);
          player2.setDamage(0.0);
          done = true;
          clock.pause();
        }
        gameOverTimer -= ticks;
        playerVictory = 2;
    }
    if(player2.getDamage() > gdata->getXmlFloat("p2LifeThreshold")) {
        if(gameOverTimer<0) {
          player.setDamage(0.0);
          player2.setDamage(0.0);
          done = true;
          clock.pause();
        }
        gameOverTimer -= ticks;
        playerVictory = 1;
    }
    backWorld.update();
    midWorld.update();
    midfrontWorld.update();
    midbackWorld.update();
    foreWorld.update();
    player.update(ticks);
    player2.update(ticks);
    viewport.update();
  }
  delete event;
}


Uint32 Manager::timeLeft() {
  Uint32 now = SDL_GetTicks();
  return (nextTime <= now)?0:(nextTime - now);
}

std::list<Drawable*>::iterator Manager::killPokeball(std::list<Drawable*>::iterator it) {
      Drawable* sprite (dynamic_cast<Item*>(*it));
          MultiframeSprite *tmp = new PlayerDeath(std::string("pokemon"),pokemonFrames);
          tmp->setPosition(sprite->getPosition());
	  if(tmp->Y() + tmp->getFrameHeight() > tmp->getWorldHeight())
		tmp->Y(tmp->getWorldHeight()-tmp->getFrameHeight());
	  sprite = tmp;
	  sprites.push_back(sprite);
   	  return it = sprites.erase(it);
}
