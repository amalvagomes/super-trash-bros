#include <cmath>
#include <sstream>
#include "menuManager.h"
#include "manager.h"

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  numberOfOrbs(0)
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
  IOManager& io = IOManager::getInstance().getInstance();
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
    io.printStringAfterMessage(msg, 20, 120);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;
      strm << number;
      strm >> numberOfOrbs;
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      strm << "Okay -- you'll see " << numberOfOrbs << " pokeballs";
//      cout << strm.str() << endl;
      io.printMessageAt(strm.str(), 20, 160);
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
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event* event = new SDL_Event();
  bool done = false;
//  bool nameDone = false;
  const string msg("----------------------- HELP MENU -----------------------");
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
//    io.printStringAfterMessage(msg, 20, 120);
      io.printMessageCenteredAt(msg, 50);
    
      io.printMessageCenteredAt("Press p to pause game", 90);
      io.printMessageCenteredAt("Press ESC to return to menu", 110);
      io.printMessageCenteredAt("Press F1 to see in game help", 130);

      io.printMessageAt("MARIO CONTROLS", 375, 180);
      io.printMessageAt("Left: LEFT arrow ", 375, 220);
      io.printMessageAt("Right: RIGHT arrow", 375, 240);
      io.printMessageAt("Jump: UP arrow ", 375, 260);
      io.printMessageAt("Sprint: RSHIFT", 375, 280);
      io.printMessageAt("Pick/Throw Item: /", 375, 300);

      io.printMessageAt("LUIGI CONTROLS", 75, 180);
      io.printMessageAt("Left: a ", 75, 220);
      io.printMessageAt("Right: d", 75, 240);
      io.printMessageAt("Jump: w ", 75, 260);
      io.printMessageAt("Sprint: e", 75, 280);
      io.printMessageAt("Pick/Throw Item: q", 75, 300);

    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event* event = new SDL_Event();
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
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getIconClicked() == "Start Game") {
              // Here is where we call the play() function in Manager:
              manager.play();
//              std::cout << "Starting game ..." << std::endl;
            }
            if ( menu.getIconClicked() == "Parameters" ) {
              getNumberOfItems();
              // After we get the parameter, we must pass it to Manager:
              manager.setNumberOfItems( numberOfOrbs );
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
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
          }
          break;
        }
        default          : break;
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
