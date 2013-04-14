#include <iostream>
#include "world.h"

void World::update() {
  float temp = (view.X() / factor);
  while(temp>static_cast<float>(frameWidth)) {
    temp -= static_cast<float>(frameWidth);
  }
  viewX = temp;
  viewY = view.Y();
}

void World::draw() const { 
  frame1.draw(viewX, viewY, 0, 0); 
  frame2.draw(0, viewY, frameWidth-viewX, 0); 
}

