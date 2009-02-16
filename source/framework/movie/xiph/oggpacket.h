#ifndef maid2_framework_xiph_oggpacket_h
#define maid2_framework_xiph_oggpacket_h


#include"../../config/define.h"

#include<ogg/ogg.h>
#include<vector>
#include<string>

namespace Maid { namespace Xiph {

  class OggStream;
  class OggPacket
  {
    friend OggStream;
  public:
    OggPacket();


    void Copy( std::vector<unsigned char>& packet, const OggPacket& SrcPacket );

    const ogg_packet& Get() const;

    ogg_int64_t  GetGranulePosition() const;
    ogg_int64_t  GetPacketNo() const; 
    long  GetSize() const; 

    std::string GetDebugString() const;

  private:
    ogg_packet  m_Packet;
  };


}}


#endif
