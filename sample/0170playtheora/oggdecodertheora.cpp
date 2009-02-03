#include"oggdecodertheora.h"

#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"


#include"../../source/graphics/imagefile/bitmap.h"
#include"../../source/storage/fileio/filewrite.h"

using namespace Maid;


/*
  theora の内部構造

  先頭パケット３つが theoraヘッダ。 theora_decode_init() を呼び続ける
  残りのパケットはフレームデータ 1フレーム 1パケット

  theora_decode_packetin() でパケットを送ることで、 theoraデコーダの時間が進みます。(同時にデコードも行われる）
  デコード済みのバッファを拾うには theora_decode_YUVout() でOK


  こんな仕様なのでスキップ、シーク時などデコード処理を省きたいときにも
  theora_decode_packetin() を呼ぶ必要があるのですが、pp level を下げることで、負荷を低減できるはず

  int level = 0;
  theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&level,sizeof(level));

  こんな感じ

*/



OggDecoderTheora::OggDecoderTheora()
:m_MaxPostProcessLevel(-1)
,m_CurrentPostProcessLevel(-1)
,m_State(STATE_EMPTY)
{
  ZERO( &m_TheoraInfo, sizeof(m_TheoraInfo) );
  ZERO( &m_TheoraComment, sizeof(m_TheoraComment) );
  ZERO( &m_TheoraState, sizeof(m_TheoraState) );
}


void OggDecoderTheora::Initialize()
{
  theora_info_init( &m_TheoraInfo );
  theora_comment_init( &m_TheoraComment );

  m_State = STATE_INITIALIZING;
}

void OggDecoderTheora::Finalize()
{
  theora_clear(&m_TheoraState);
  theora_comment_clear(&m_TheoraComment);
  theora_info_clear(&m_TheoraInfo);
  m_State = STATE_EMPTY;
}

bool OggDecoderTheora::IsSetupped() const
{
  return m_State==STATE_BEGINDECODE
    ||   m_State==STATE_DECODING
    ;
}

const theora_info& OggDecoderTheora::GetInfo() const
{
  MAID_ASSERT( !IsSetupped(), "まだ初期化されていません" );
  return m_TheoraInfo;
}


void OggDecoderTheora::PacketIn( const OggPacket& NewPacket )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_BEGINDECODE: { OnDecode( NewPacket ); m_State = STATE_DECODING; }break;
  case STATE_DECODING:    { OnDecode( NewPacket ); }break;
  }
}

double OggDecoderTheora::GetTime()
{
  const double time = theora_granule_time( &m_TheoraState, m_TheoraState.granulepos );
//  const int frame = theora_granule_frame( &m_TheoraState, m_TheoraState.granulepos );
  return time;
}

void OggDecoderTheora::BeginSeekMode()
{
  SetPPLevel( 0 );
}

void OggDecoderTheora::EndSeekMode()
{
  SetPPLevel( m_MaxPostProcessLevel );
}

bool OggDecoderTheora::IsDecoding() const
{
  return m_State==STATE_DECODING;
}

SPOGGDECODEDBUFFER OggDecoderTheora::CreateDecodedBuffer() const
{
  MAID_ASSERT( !IsDecoding(), "まだ準備できていません" );

  yuv_buffer src;

  theora_state& state = const_cast<theora_state&>(m_TheoraState);

  const int ret = theora_decode_YUVout(&state, &src);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_YUVout" ); return SPOGGDECODEDBUFFER(); }

  boost::shared_ptr<OggDecodedBufferImage> pBuffer( new OggDecodedBufferImage );

  {
    OggDecodedBufferImage& buf = *pBuffer;
    buf.SizeY  = SIZE2DI( src.y_width, src.y_height );
    buf.PitchY = src.y_stride;
    buf.BitsY.resize( src.y_stride * src.y_height );
    ::memcpy( &(buf.BitsY[0]), src.y, buf.BitsY.size() );

    buf.SizeUV  = SIZE2DI( src.uv_width, src.uv_height );
    buf.PitchUV = src.uv_stride;
    buf.BitsU.resize( src.uv_stride * src.uv_height );
    ::memcpy( &(buf.BitsU[0]), src.u, buf.BitsU.size() );
    buf.BitsV.resize( src.uv_stride * src.uv_height );
    ::memcpy( &(buf.BitsV[0]), src.v, buf.BitsV.size() );
  }

  return pBuffer;
}

void OggDecoderTheora::SetPPLevel( int level )
{
  if( m_MaxPostProcessLevel < level )
  {
    level = m_MaxPostProcessLevel;
  }

  int ret;
  ret = theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&level, sizeof(level));
  if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }

  m_CurrentPostProcessLevel = level;
}


void OggDecoderTheora::OnSetup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = theora_decode_header(&m_TheoraInfo,&m_TheoraComment,&packet);
  if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    theora_decode_init(&m_TheoraState,&m_TheoraInfo);

    //  ポストプロセスレベルを最大にしておく
    //  ポストプロセスレベルってデコードの精度ってことでいいのかな？
    int pp;
    int ret;

    ret = theora_control(&m_TheoraState,TH_DECCTL_GET_PPLEVEL_MAX,&pp, sizeof(pp));
    if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }
    m_MaxPostProcessLevel     = pp;
    m_CurrentPostProcessLevel = pp;
    ret = theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&pp,sizeof(pp));
    if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }

    m_State = STATE_BEGINDECODE;
  }

}

void OggDecoderTheora::OnDecode( const OggPacket& NewPacket )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();
  {
    /*HACK: This should be set after a seek or a gap, but we might not have
       a granulepos for the first packet (we only have them for the last
       packet on a page), so we just set it as often as we get it.
      To do this right, we should back-track from the last packet on the
       page and compute the correct granulepos for the first packet after
       a seek or a gap.*/
    //  訳がよくわからんけどパケット間のギャップを埋めるためのものかな？
    ogg_int64_t pos = NewPacket.GetGranulePosition();
    if( pos >= 0 )
    {
      theora_control(&m_TheoraState, TH_DECCTL_SET_GRANPOS, &pos, sizeof(pos));
    }
  }

  const int ret = theora_decode_packetin(&m_TheoraState,&packet);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_packetin" ); }



/*
  //  成功したんで、デコードする
  //  yuv の変換はここみてね
  //  http://vision.kuee.kyoto-u.ac.jp/~hiroaki/firewire/yuv.html
  //  http://d.hatena.ne.jp/Ko-Ta/20090124

  {
    Surface& Frame = pBuffer->Frame;
    yuv_buffer src;

    theora_decode_YUVout(&m_TheoraState, &src);

    //  パケット上は FrameSize の大きさで入っている
    Frame.Create( FrameSize, PIXELFORMAT_X08R08G08B08I );

    //  いろいろパフォーマンスがいりそうなので、直接メモリに書き込む
    for( int y_y=0; y_y<src.y_height; ++y_y )
    {
      const int uv_y = y_y * src.uv_height / src.y_height ;

      const unsigned char* pSrcPixelY = src.y + src.y_stride  * y_y;
      const unsigned char* pSrcPixelU = src.u + src.uv_stride * uv_y;
      const unsigned char* pSrcPixelV = src.v + src.uv_stride * uv_y;
      unt08* pDstPixel = (unt08*)Frame.GetLinePTR( m_TheoraInfo.offset_y + y_y );
      pDstPixel += m_TheoraInfo.offset_x * 4;

      const unt32 offset = (src.uv_width << 8) / src.y_width;
      unt32 fix_uv_x = 0;

      for( int y_x=0; y_x<src.y_width; ++y_x )
      {
        const unt32 uv_x = fix_uv_x >> 8;

        int y = pSrcPixelY[y_x ];
        int u = pSrcPixelU[uv_x];
        int v = pSrcPixelV[uv_x];

        y=(y-16)*298 >> 8;    //Y成分を16～235→0～256に伸縮
        u=(u-128)*291 >>8;    //U成分をCbに変換
        v=(v-128)*291 >>8;    //V成分をCrに変換
        int r = (y*256        +v*358) >> 8; //R = Y            + 1.40200V
        int g = (y*256 -u*88  -v*183) >> 8; //G = Y - 0.34414U - 0.71414V
        int b = (y*256 +u*454       ) >> 8; //B = Y + 1.77200U
        //0～255の閾値処理
        if( r<   0  ) { r=0; }
        ef(   255<r ) { r = 255; }
        if( g<   0  ) { g=0; }
        ef(   255<g ) { g = 255; }
        if( b<   0  ) { b=0; }
        ef(   255<b ) { b = 255; }

        unt08* dst = pDstPixel + (y_x*4);

        dst[0] = r;
        dst[1] = g;
        dst[2] = b;
        fix_uv_x += offset;
      }
    }
    pDst = pBuffer;

/*
    if( pFrame.get()!=NULL )
    {
      m_Frame.push_back( FRAME(pFrame,gra,time) );

    end1 = TIMER.Get();

    MAID_WARNING( "time:" << (end0-begin) << " " << (end1-end0) );

    std::vector<unt08> FileImage;

    Bitmap::Save( *pFrame, FileImage );

    static int no;
    String str = String::PrintFormat( "hoge%04d.bmp", no );
    
    ++no;

    {
      FileWrite hFile;

      hFile.Open( str, FileWrite::OPENOPTION_NEW );
      hFile.Write( &(FileImage[0]), FileImage.size() );

    }

    }
  }
*/
}
