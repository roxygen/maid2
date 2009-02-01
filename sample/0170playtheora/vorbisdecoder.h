#ifndef vorbisdecoder_h
#define vorbisdecoder_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include"ioggdecoder.h"

#include <vorbis/codec.h>

class VorbisDecoder : public IOggDecoder
{
public:
  virtual void Initialize( const OggPacket& NewPacket );
  virtual void Update( const OggPacket& NewPacket );

private:
  vorbis_info      m_Info;
  vorbis_comment   m_Comment;
  vorbis_dsp_state m_State;
  vorbis_block     m_Block;
};


#endif