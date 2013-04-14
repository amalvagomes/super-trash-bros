#include <cmath>
#include "manager.h"
#include <algorithm>

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for(unsigned i = 0; i < stars.size(); i++) {
    delete stars[i];
  }
  SDL_FreeSurface(screen);
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
  backWorld( backFrame, gdata->getXmlFloat("backScale"), 2.0),
  midSurface( io.loadAndSet(gdata->getXmlStr("midFile"), 
                gdata->getXmlBool("midTransparency")) ),
  midFrame(new Frame(midSurface,
                gdata->getXmlInt("midWidth"), 
                gdata->getXmlInt("midHeight"), 
                gdata->getXmlInt("midSrcX"), 
                gdata->getXmlInt("midSrcY")) 
  ),
  midWorld( midFrame, gdata->getXmlFloat("midScale"), 1.5 ),
  midfrontSurface( io.loadAndSet(gdata->getXmlStr("midfrontFile"), 
                gdata->getXmlBool("midfrontTransparency")) ),
  midfrontFrame(new Frame(midfrontSurface,
                gdata->getXmlInt("midfrontWidth"), 
                gdata->getXmlInt("midfrontHeight"), 
                gdata->getXmlInt("midfrontSrcX"), 
                gdata->getXmlInt("midfrontSrcY")) 
  ),
  midfrontWorld( midfrontFrame, gdata->getXmlFloat("midfrontScale"), 1.2 ),
  foreSurface( io.loadAndSet(gdata->getXmlStr("boardwalkFile"), 
                gdata->getXmlBool("boardwalkTransparency")) ),
  foreFrame(new Frame(foreSurface,
                gdata->getXmlInt("boardwalkWidth"), 
                gdata->getXmlInt("boardwalkHeight"), 
                gdata->getXmlInt("boardwalkSrcX"), 
                gdata->getXmlInt("boardwalkSrcY")) 
  ),
  foreWorld( foreFrame, gdata->getXmlFloat("boardwalkScale")),
  viewport( Viewport::getInstance() ),
  player(std::string("mario")),
  player2(std::string("yoshi")),
  currentStar(0),
//  deathFrames(),
  displayHelpText(false),
  stars(),
  TICK_INTERVAL( gdata->getXmlInt("tickInterval") ),
  nextTime(clock.getTicks()+TICK_INTERVAL)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
  // We now reserve space for the sprites; thus, obviating 
  // a lot of copies, reallocations, and deletions:
  FrameFactory& frameFact = FrameFactory::getInstance();
  stars.reserve(gdata->getXmlInt("shellCount")+2);
//  makeDyingCrow();
  makeShells();
  float scale;
  std::vector<Frame*> frames = frameFact.getFrameVector("pokeball", &scale);
  stars.push_back( new MultiframeSprite("pokeball", frames, scale) );
  sort(stars.begin(), stars.end(), DrawableComparator());
  viewport.setObjectToTrack(player.getSprite());
}

/*void Manager::makeDyingcrow() {
  unsigned numberOfFrames = gdata->getXmlInt("dyingcrowFrames");
  Uint16 pwidth = gdata->getXmlInt("dyingcrowWidth");
  Uint16 pheight = gdata->getXmlInt("dyingcrowHeight");
  Uint16 srcX = gdata->getXmlInt("dyingcrowSrcX");
  Uint16 srcY = gdata->getXmlInt("dyingcrowSrcY");

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * pwidth + srcX;
    deathFrames.push_back( 
      new Frame(dyingcrowSurface, pwidth, pheight, frameX, srcY) );
  }
}*/

void Manager::makeShells() {
  FrameFactory& frameFact = FrameFactory::getInstance();
  for(unsigned i = 0; i < (unsigned)gdata->getXmlInt("shellCount"); i++) {
    float scale;
    Frame* frame = frameFact.getFrame("shell", &scale, true);
    stars.push_back( new Sprite("shell", frame, scale) );
  }
}

void Manager::draw() const {
  bool backWorldDrawn = false;
  bool midWorldDrawn = false;
  bool midfrontWorldDrawn = false;
  bool foreWorldDrawn = false;
  bool playerDrawn = false;
  bool player2Drawn = false;

  for (unsigned i = 0; i < stars.size(); ++i) {
    if(!backWorldDrawn && stars[i]->getScale() > backWorld.getScale()) {
      backWorld.draw();
      backWorldDrawn = true;
    }
    if(!midWorldDrawn && stars[i]->getScale() > midWorld.getScale()) {
      midWorld.draw();
      midWorldDrawn = true;
    }
    if(!midfrontWorldDrawn && stars[i]->getScale() > midfrontWorld.getScale()) {
      midfrontWorld.draw();
      midfrontWorldDrawn = true;
    }
    if(!foreWorldDrawn && stars[i]->getScale() > foreWorld.getScale()) {
      foreWorld.draw();
      foreWorldDrawn = true;
    }
    if(!playerDrawn && stars[i]->getScale() > player.getSprite()->getScale()) {
      player.draw();
      playerDrawn = true;
    }
    if(!player2Drawn && stars[i]->getScale() > player2.getSprite()->getScale()) {
      player2.draw();
      player2Drawn = true;
    }
    stars[i]->draw();
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
  if(!foreWorldDrawn) {
      foreWorld.draw();
  }
  if(displayHelpText) {
    viewport.draw();
  }
}

void Manager::play() {
  SDL_Event* event = new SDL_Event();

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
    if(event->type == SDL_KEYUP) { keyCatch = false; }
    if(event->type == SDL_KEYDOWN) {
      switch ( event->key.keysym.sym ) {
        case SDLK_ESCAPE : done = true; break;
        case SDLK_q      : done = true; break;
        case SDLK_t      : {
          if (!keyCatch) {
            keyCatch = true;
            currentStar = (currentStar+1) % stars.size();
            viewport.setObjectToTrack(stars[currentStar]);
          }
          break;
        }
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
//            killPlayer();
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
        default          : break;
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
    }

    Uint32 ticks = clock.getElapsedTicks();
    for (unsigned i = 0; i < stars.size(); ++i) {
      stars[i]->update(ticks);
    }
    backWorld.update();
    midWorld.update();
    midfrontWorld.update();
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

/*void Manager::killPlayer() {
    const Drawable* sprite = (player.getSprite());
      MultiframeSprite* death = 
	new PlayerDeath("dyingcrow", deathFrames);
      player = death;
      death->setPosition( player->getPosition() );
      return;
    }
  }
}*/
