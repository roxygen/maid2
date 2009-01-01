#ifndef maid2_sound_soundobjectlocal_h
#define maid2_sound_soundobjectlocal_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"

namespace Maid {

  class SoundObjectPCMStatic : public ISoundObject
  {
  public:
    Sound::SPBUFFER  pBuffer;
  };

  class SoundObjectPCMStream : public ISoundObject
  {
  public:
    Sound::SPBUFFER  pBuffer;
//			SPSOUNDDECODER	pDecoder;	//	デコーダ
  };


}


#endif
