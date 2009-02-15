#include"../../source/config/define.h"
#include"../../source/framework/xiph/oggcontainer.h"
#include"../../source/framework/xiph/oggpacket.h"
#include"../../source/framework/xiph/oggpage.h"
#include"../../source/framework/xiph/oggstream.h"
#include"../../source/framework/xiph/decodertheora.h"
#include"../../source/framework/xiph/decodervorbis.h"
#include"../../source/storage/storage.h"
#include"../../source/auxiliary/timer.h"

#include<stdio.h>
#include<ogg/ogg.h>
#include<theora/theora.h>
#include<windows.h>
#include<vorbis/codec.h>
#include<map>
#include<boost/smart_ptr.hpp>

using namespace Maid;
using namespace Xiph;

struct BENCHINFO
{
  int Serial;
  int PacketNo;
};


void main( int argc, char *argv[] )
{
  if( argc<2 ) { printf("ファイル名を指定してください\n" ); return ; }

  Timer CurrentTimer;

  {
    String::Initialize();
  }

  Storage strage;
  strage.Initialize();


  OggContainer  container;
  boost::shared_ptr<OggStream>  th_stream;
  boost::shared_ptr<IDecoder>   th_decoder;

  boost::shared_ptr<OggStream>  vb_stream;
  boost::shared_ptr<IDecoder>   vb_decoder;


  std::multimap<double,BENCHINFO> BenchResult;

  container.Initialize( String::ConvertSJIStoMAID(argv[1]) );

  bool IsTheora = false;

  //  まずはtheora 探し
  while( true )
  {
    if( container.IsEnd() ) { break; }

    const OggPage& page = container.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    OggPacket packet;

    boost::shared_ptr<OggStream>  pStream( new OggStream );

    pStream->Initialize( page.GetSerialNo() );
    pStream->PageIn( page );
    pStream->PacketOut( packet );

    ogg_packet& p = const_cast<ogg_packet&>(packet.Get());
    if( theora_packet_isheader(&p)==1 ) 
    {
      boost::shared_ptr<DecoderTheora> pDecoder( new DecoderTheora );
      pDecoder->Initialize(packet);

      const theora_info& info = pDecoder->GetInfo();
      const char* FORMAT[] =
      {
        "OC_PF_420",
        "OC_PF_RSVD",
        "OC_PF_422",
        "OC_PF_444",
      };

      char buf[1024];
      sprintf( buf, "size(%d,%d) frame(%d,%d) offset(%d,%d) fps:%d/%d aspect:%d/%d bitrate:%d color:%s",
        info.width, info.height, info.frame_width, info.frame_height,
        info.offset_x, info.offset_y, info.fps_numerator, info.fps_denominator,
        info.aspect_numerator, info.aspect_denominator, info.target_bitrate, FORMAT[info.pixelformat] );

      printf( "[theora] serial:%d %s\n", page.GetSerialNo(), buf );

      th_decoder = pDecoder;
      th_stream = pStream;
    }
    ef( vorbis_synthesis_idheader(&p)==1 )
    {
      boost::shared_ptr<DecoderVorbis> pDecoder( new Xiph::DecoderVorbis );
      pDecoder->Initialize(packet);

      const vorbis_info& vi = pDecoder->GetInfo();

      char buf[1024];
      sprintf( buf, "rate:%d channels:%d", vi.rate, vi.channels );
      printf( "[vorbis] serial:%d %s\n", page.GetSerialNo(), buf );

      vb_decoder = pDecoder;
      vb_stream = pStream;
    }

    container.NextPage();
  }

  unt totalcount = 0;

  while( true )
  {
    if( container.IsEnd() ) { break; }

    const OggPage& page = container.GetCurrentPage();

    OggStream* pStream = NULL;
    IDecoder* pDecoder = NULL;
    std::string info;

    if( th_stream->GetSerialNo()==page.GetSerialNo() )
    {
      pStream = th_stream.get(); 
      pDecoder = th_decoder.get(); 
      info = "[theora]";
    }
    ef( vb_stream->GetSerialNo()==page.GetSerialNo() )
    {
      pStream = vb_stream.get(); 
      pDecoder = vb_decoder.get(); 
      info = "[vorbis]";
    }

    if( pStream!=NULL )
    {
      printf( "  [page]%s\n", page.GetDebugString().c_str() );

      {
        const unt t = CurrentTimer.Get();
        pStream->PageIn( page );
        totalcount += CurrentTimer.Get() - t;
      }
      while( true )
      {
        OggPacket packet;
        {
          const unt t = CurrentTimer.Get();
          const bool IsSuccess = pStream->PacketOut( packet );
          totalcount += CurrentTimer.Get() - t;
          if( !IsSuccess ) { break; }
        }
        printf( "    [packet]%s\n", packet.GetDebugString().c_str() );

        std::string packetinfo = pDecoder->GetDebugString(packet);
        const double BeginTime = pDecoder->GetTime();

        const unt BeginClock = CurrentTimer.Get();
        pDecoder->Decode( packet, Xiph::SPBUFFER() );
        const unt EndClock   = CurrentTimer.Get();
        const double EndTime = pDecoder->GetTime();

        const double decode_time = double(EndClock-BeginClock)/1000.0;

        {
          char buf[1024];
          sprintf( buf, "%s %.4f-%.4f decodetime:%.4f + %.4f",
            packetinfo.c_str(), BeginTime, EndTime, decode_time, double(totalcount)/1000.0 );
          printf( "      %s%s\n", info.c_str(), buf);
        }

        {
          BENCHINFO info;
          info.Serial   = pStream->GetSerialNo();
          info.PacketNo = (int)packet.GetPacketNo();

          BenchResult.insert( std::make_pair(decode_time,info) );
        }
        totalcount = 0;
      }
    }
    {
      const unt t = CurrentTimer.Get();
      container.NextPage();
      totalcount += CurrentTimer.Get() - t;
    }
  }


  {
    const int count = BenchResult.size() / 10;
    std::multimap<double,BENCHINFO>::reverse_iterator ite = BenchResult.rbegin();

    printf( " --------------warst %d---------------\n", count );
    for( int i=0; i<count; ++i )
    {
      printf( "serial %06d: packet %06d: time %.4f\n",
        ite->second.Serial, ite->second.PacketNo, ite->first );
      ++ite;
    }
  }

}