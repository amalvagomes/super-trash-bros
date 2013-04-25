#include <sstream>
#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"

FrameFactory::~FrameFactory() {
//  std::cout << "The FrameFactory is down sizing!" << std::endl;
  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface( itSurf->second );
    ++itSurf;
  }
  std::map<std::string, Frame*>::iterator itFrame = frames.begin();
  while ( itFrame != frames.end() ) {
    delete itFrame->second;
    ++itFrame;
  }
  std::map<std::string, std::vector<Frame*> >::iterator itFrameVect = frameVectors.begin();
  while ( itFrameVect != frameVectors.end() ) {
    for(unsigned i = 0; i<itFrameVect->second.size(); i++) {
      delete itFrameVect->second[i];
    }
    ++itFrameVect;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

std::vector<Frame*> FrameFactory::getFrameVector(const std::string& name, float * const scale) {
  std::map<std::string, std::vector<Frame*> >::const_iterator pos = frameVectors.find(name); 
  if ( pos == frameVectors.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata->getXmlStr(name+"File"),
          gdata->getXmlBool(name+"Transparency"));
    surfaces[name+"_multiframe"] = surface;
    *scale = getRand(Gamedata::getInstance()->getXmlFloat(name+"ScaleMin"),
          Gamedata::getInstance()->getXmlFloat(name+"ScaleMax"));
    unsigned numberOfFrames = gdata->getXmlInt(name+"Frames");
    Uint16 pwidth = gdata->getXmlInt(name+"Width");
    Uint16 pheight = gdata->getXmlInt(name+"Height");
    Uint16 srcX = gdata->getXmlInt(name+"SrcX");
    Uint16 srcY = gdata->getXmlInt(name+"SrcY");
    std::vector<Frame*> framesTemp;
    framesTemp.reserve((surface->w - srcX)/pwidth);

    for (unsigned i = 0; i < numberOfFrames; ++i) {
      unsigned frameX = i * pwidth + srcX;
      framesTemp.push_back( 
        new Frame(surface, pwidth, pheight, frameX, srcY) );
    }
    frameVectors[name] = framesTemp;
    return framesTemp;
  } else {
    return pos->second;
  }
}

Frame* FrameFactory::getFrame(const std::string& name, float * const scale, bool ignorePrevious = false) {
  std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() || ignorePrevious) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata->getXmlStr(name+"File"),
          gdata->getXmlBool(name+"Transparency"));
    *scale = getRand(Gamedata::getInstance()->getXmlFloat(name+"ScaleMin"),
          Gamedata::getInstance()->getXmlFloat(name+"ScaleMax"));
    std::ostringstream temp;
    temp << *scale;
    std::string scaleString = temp.str();
    SDL_Surface* tmp = 
          rotozoomSurface(surface, 0.0, static_cast<double>(*scale), SMOOTHING_ON);
    SDL_FreeSurface(surface);
    if(!ignorePrevious) {
      surfaces[name] = tmp;
    } else {
      surfaces[name + scaleString] = tmp;
    }
    Frame * const frame =new Frame(tmp,
                gdata->getXmlInt(name+"Width")* *scale, 
                gdata->getXmlInt(name+"Height")* *scale,
                gdata->getXmlInt(name+"SrcX"), 
                gdata->getXmlInt(name+"SrcY")); 
    if(!ignorePrevious) {
      frames[name] = frame;
    } else {
      frames[name + scaleString] = frame;
    }
    return frame;
  }
  else {
    return pos->second;
  }
}

