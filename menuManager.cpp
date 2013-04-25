#include <cmath>
#include <sstream>
#include "sound.h"
#include "menuManager.h"
#include "manager.h"

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  io( IOManager::getInstance() ),
  backColor(),
  menu(),
  numberOfOrbs(0),
  soundEnabled(true)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance()->getXmlInt("backRed");
  backColor.g = Gamedata::getInstance()->getXmlInt("backGreen");
  backColor.b = Gamedata::getInstance()->getXmlInt("backBlue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
    menu.drawBG();
}

void MenuManager::getNumberOfItems() {
//  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event* event = new SDL_Event();
  bool done = false;
  bool nameDone = false;
  const string msg("How many Pokeballs: ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(event) )
    switch (event->type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 200);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;
      strm << number;
      strm >> numberOfOrbs;
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      strm << "Okay -- you'll see " << numberOfOrbs << " pokeballs";
//      cout << strm.str() << endl;
      io.printMessageAt(strm.str(), 20, 240);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::getSoundOptions() {
//  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event* event = new SDL_Event();
  bool done = false;
  bool nameDone = false;
  const string msg("Sound? (on/off) ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(event) )
    switch (event->type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 200);
    if ( nameDone ) {
      std::string answer = io.getString();
      std::stringstream strm;
      if(answer=="on") {
        strm << "Okay -- you'll hear music!";
        soundEnabled = true;
      } else if(answer=="off") {
        strm << "Okay -- we'll keep things quiet...";
        soundEnabled = false;
      } else {
        strm << "That response doesn't look like an 'on' or 'off'...";
        soundEnabled = true;
      }
      io.printMessageAt(strm.str(), 20, 240);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::displayHelp() {
//  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event* event = new SDL_Event();
  bool done = false;
//  bool nameDone = false;
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(event) )
    switch (event->type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
      io.printMessageCenteredAt("Press p to pause game", 150);
      io.printMessageCenteredAt("Press ESC to return to menu", 170);
      io.printMessageCenteredAt("Press F1 to see in game help", 190);

      io.printMessageAt("MARIO CONTROLS", 375, 230);
      io.printMessageAt("Left: LEFT arrow ", 375, 270);
      io.printMessageAt("Right: RIGHT arrow", 375, 290);
      io.printMessageAt("Jump: UP arrow ", 375, 310);
      io.printMessageAt("Sprint: RSHIFT", 375, 330);
      io.printMessageAt("Pick/Throw Item: /", 375, 350);

      io.printMessageAt("LUIGI CONTROLS", 75, 230);
      io.printMessageAt("Left: a ", 75, 270);
      io.printMessageAt("Right: d", 75, 290);
      io.printMessageAt("Jump: w ", 75, 310);
      io.printMessageAt("Sprint: e", 75, 330);
      io.printMessageAt("Pick/Throw Item: q", 75, 350);

    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event* event = new SDL_Event();
  SDLSound sound;
  sound.toggleMusic();
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game:
  Manager manager;

  while ( not done ) {

    drawBackground();
    menu.draw();
    SDL_Flip(screen);

    SDL_PollEvent(event);
    if (event->type ==  SDL_QUIT) { break; }
    if(event->type == SDL_KEYDOWN) {
      switch ( event->key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN: case SDLK_RIGHT: {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getIconClicked() == "Start Game") {
              // Here is where we call the play() function in Manager:
              manager.play();
              menu.lightOff();
//              std::cout << "Starting game ..." << std::endl;
            }
            if ( menu.getIconClicked() == "Parameters" ) {
              getNumberOfItems();
              manager.setNumberOfItems( numberOfOrbs );
              getSoundOptions();
              manager.setSoundEnabled( soundEnabled );
            }
            if ( menu.getIconClicked() == "Help" ) {
              // Here is where we explain how to play the game"
              //std::cout << "Give help  ..." << std::endl;
	      displayHelp();
            }
            if ( menu.getIconClicked() == "Exit" ) {
              drawBackground();
              menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
            }
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.increment();
            io.clearString();
            io.buildString(event);
            if(soundEnabled) sound[1];
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
            io.clearString();
            io.buildString(event);
            if(soundEnabled) sound[1];
          }
          break;
        }
        default          :  break;
      }
      keyCatch = true;
    }
    if(event->type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }
  delete event;
}
