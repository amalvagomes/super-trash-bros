#ifndef ITEM__H
#define ITEM__H

#include <string>
#include <iostream>
#include <vector>

#include "drawable.h"

class Item : public Drawable {
public:
  Item(const std::string& n, const std::vector<Frame*>& fms, const float sc);
  Item(const Item& s);
  virtual ~Item() { } 
  virtual const Frame* getFrame() const { return frames[currentFrame]; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void fall();
  void stop();
  void toggleMovement();
  void toggleAnimation();
  bool isReleased() { return released; }
  bool isStopped() { return stopped; }
  bool isAnimated() { return animated; }
  unsigned getPixel(Uint32, Uint32) const;
  int getLastTouched() { return lastTouched; }
  void setLastTouched(int x) { lastTouched = x; }

protected:
  unsigned currentFrame;
  unsigned numberOfFrames;
  int frameHeight;
  int frameWidth;
  int worldWidth;
  int worldHeight;
  bool animated;
  bool stopped;
  bool released;
  int lastTouched;

private:
  const std::vector<Frame *> frames;
  float dt;
  unsigned frameInterval;
  void advanceFrame(Uint32 ticks);
};
#endif
