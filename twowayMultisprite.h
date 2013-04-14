#ifndef TWOWAY_MULTISPRITE__H
#define TWOWAY_MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"

class TwowayMultiframeSprite : public Drawable {
public:
  TwowayMultiframeSprite(const std::string& n, 
                   std::vector<Frame*>& fmsLeft,
                   std::vector<Frame*>& fmsRight, const float sc);
  TwowayMultiframeSprite(const TwowayMultiframeSprite& s);
  virtual ~TwowayMultiframeSprite() { } 
  virtual const Frame* getFrame() const { return frames[currentFrame]; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);

private:
  std::vector<Frame *> framesLeft;
  std::vector<Frame *> framesRight;
  std::vector<Frame *> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  void advanceFrame(Uint32 ticks);
  TwowayMultiframeSprite& operator=(const TwowayMultiframeSprite&);
};
#endif
