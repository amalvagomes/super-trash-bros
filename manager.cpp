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
  player2(std::string("yoshi")),
  playerPickup(false),
  player2Pickup(false),
  currentStar(0),
  itemTimer(gdata->getXmlInt("itemInterval")),
  itemTime(0),
  displayHelpText(false),
  sprites(),
  pokemonFrames(),
  TICK_INTERVAL( gdata->getXmlInt("tickInterval") ),
  nextTime(clock.getTicks()+TICK_INTERVAL)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
    std::cout << "Adding pokeball" << std::endl;
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

void Manager::setNumberOfItems(int number) {
  FrameFactory& frameFact = FrameFactory::getInstance();
  int numberOfPokeballs=gdata->getXmlInt("itemCount");
  if ( number > numberOfPokeballs ) {
    number = number - numberOfPokeballs;
    for (int i = 0; i < number; ++i) {
	float scale;
    	std::cout << "Adding pokeball" << std::endl;
   	std::vector<Frame*> frames = frameFact.getFrameVector("pokeball", &scale);
    	sprites.push_back( new Item("pokeball", frames, scale));
    }
  }
  else {
    number = numberOfPokeballs - number;
    for (int i = 0; i < number; ++i) {
      delete sprites.back();
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
}

void Manager::play() {
  clock.unpause();
  SDL_Event* event = new SDL_Event();
  SDLSound sound;
  bool done = false;
  bool keyCatch = false;
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
	case SDLK_k      : {
          if (!keyCatch) {
            keyCatch = true;
            killPokeball();
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
                item->velocityY(-350.0);
                float veloc = player2.getSprite()->velocityX();
                veloc += (veloc/abs(veloc))*300.0;
                item->velocityX(veloc);
                sprites.push_back(static_cast<Drawable*>(item));
                sprites.sort(DrawableComparator());
            } else {
                player2Pickup = true;
            }
            sound[0];
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
                item->velocityY(player.getSprite()->velocityY()-25.0);
                float veloc = player.getSprite()->velocityX();
                veloc += (veloc/abs(veloc))*200.0;
                item->velocityX(veloc);
                sprites.push_back(static_cast<Drawable*>(item));
                sprites.sort(DrawableComparator());
            } else {
                playerPickup = true;
            }
            sound[0];
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
    if(keystate[SDLK_DOWN]) {
      player.down();
    }
    if(keystate[SDLK_UP]) {
      player.up();
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
    if(keystate[SDLK_s]) {
      player2.down();
    }
    if(keystate[SDLK_w]) {
      player2.up();
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
            item->X((int)getRand(0.0,640.0));
            item->Y(0);
            item->fall();
            itemTime = 0;
          }
        } else {
            if(playerPickup && player.collideWith((*it))) {
                player.setItem(item);
                playerPickup = false;
                it = sprites.erase(it);
                continue;
            }
            if(player2Pickup && player2.collideWith((*it))) {
                player2.setItem(item);
                player2Pickup = false;
                it = sprites.erase(it);
                continue;
            }
        }
      }
      if(dynamic_cast<PlayerDeath*>(*it) && static_cast<PlayerDeath*>(*it)->isDead()){
	makeNewItem();
	it = sprites.erase(it);
	sprites.sort(DrawableComparator());
	continue;
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

void Manager::killPokeball() {
    std::list<Drawable*>::iterator it = sprites.begin();
    while (it != sprites.end()) {
      Drawable* sprite (dynamic_cast<Item*>(*it));
      if(sprite)
      {
          MultiframeSprite *tmp = new PlayerDeath(std::string("pokemon"),pokemonFrames);
          tmp->setPosition(sprite->getPosition());
	  sprite = tmp;
	  sprites.push_back(sprite);
   	  it = sprites.erase(it);
	  return;
      }
      else it++;
    }
}
