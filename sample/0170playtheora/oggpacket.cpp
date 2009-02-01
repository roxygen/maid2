#include"oggpacket.h"


const ogg_packet& OggPacket::Get() const
{
  return m_Packet;
}


ogg_int64_t  OggPacket::GetGranulePosition() const
{
  return m_Packet.granulepos;
}

ogg_int64_t  OggPacket::GetPacketNo() const
{
  return m_Packet.packetno;
}
