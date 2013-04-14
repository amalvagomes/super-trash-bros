#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>

#include "drawable.h"

class MultiframeSprite : public Drawable {
public:
  MultiframeSprite(const std::string& n, const std::vector<Frame*>& fms, const float sc);
  MultiframeSprite(const MultiframeSprite& s);
  virtual ~MultiframeSprite() { } 
  virtual const Frame* getFrame() const { return frames[currentFrame]; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);

protected:
  unsigned currentFrame;
  unsigned numberOfFrames;
  int frameHeight;
  int frameWidth;
  int worldWidth;
  int worldHeight;

private:
  const std::vector<Frame *> frames;
  float dt;
  unsigned frameInterval;
  void advanceFrame(Uint32 ticks);
};
#endif
