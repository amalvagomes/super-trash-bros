#ifndef FRAME_FACTORY
#define FRAME_FACTORY
#include <string>
#include <vector>
#include <map>
#include <limits>
#include "frame.h"
#include "gamedata.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
  Frame* getFrame(const std::string&, float * const scale, bool ignorePrevious);
  std::vector<Frame*> getFrameVector(const std::string&, float * const scale);

private:
  Gamedata* gdata;
  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, Frame*> frames;
  std::map<std::string, std::vector<Frame*> > frameVectors;

  FrameFactory() : 
    gdata( Gamedata::getInstance() ), surfaces(), frames(), frameVectors()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);

  float getRand(int min, int max) {
    return min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min);
  }

  float getRand(float min, float max) {
    return min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min);
  }
};
#endif
