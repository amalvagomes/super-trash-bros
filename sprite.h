#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include <iostream>

#include "drawable.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string& n, const Frame*, const float sc);
  Sprite(const Sprite& s);
  virtual ~Sprite() {};
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(const Frame* f) { frame = f; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned getPixel(Uint32, Uint32) const;
  int getFrameWidth() const { return frameWidth; }

  Vector2f getCenter() const { 
    return Vector2f( X()+frame->getWidth()/2, Y()+frame->getHeight()/2 );
  }

protected:
  int frameWidth;
  int frameHeight;

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
