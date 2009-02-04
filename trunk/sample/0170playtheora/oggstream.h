#ifndef oggstream_h
#define oggstream_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include<ogg/ogg.h>
#include<boost/smart_ptr.hpp>

#include"oggpage.h"
#include"oggpacket.h"

class OggStream
{
public:
  OggStream();
  ~OggStream();

  void Initialize( int serial );
  void Finalize();

  void PageIn( const OggPage& Page );
  void Reset();

  const OggPacket& GetCurrentPacket()const;
  void NextPacket();

  bool IsPacketEmpty() const;

  int GetSerialNo() const;

private:

  ogg_stream_state m_Stream;

  OggPacket m_Packet;
  bool  m_IsEmpty;
};

#endif
