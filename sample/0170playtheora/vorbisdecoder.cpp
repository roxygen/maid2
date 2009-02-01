#include"vorbisdecoder.h"
#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"


void VorbisDecoder::Initialize( const OggPacket& NewPacket )
{
  MAID_ASSERT( NewPacket.GetPacketNo()!=0, "パケット番号が合いません" );

  vorbis_info_init( &m_Info );
  vorbis_comment_init( &m_Comment );

  ogg_packet& packet = (ogg_packet&)NewPacket.Get();


  const int ret = vorbis_synthesis_headerin( &m_Info, &m_Comment, &packet );
  if( ret<0 )  { MAID_WARNING( "vorbis_synthesis_headerin" ); }
}

void VorbisDecoder::Update( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();

  if( no<3 )
  {

  }
}
