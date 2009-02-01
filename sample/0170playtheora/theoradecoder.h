#ifndef theoradecoder_h
#define theoradecoder_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include"ioggdecoder.h"

#include <theora/theora.h>

class TheoraDecoder : public IOggDecoder
{
public:
  virtual void Initialize( const OggPacket& NewPacket );
  virtual void Update( const OggPacket& NewPacket );
  virtual void Finalize();

private:
  theora_info      m_Info;
  theora_comment   m_Comment;
  theora_state     m_State;
};


#endif