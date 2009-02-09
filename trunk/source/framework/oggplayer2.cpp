#include"oggplayer.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
#include"xiph/decodernull.h"

/*

  ogg の内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1]page[pv2,pv3,,,]   page[ps0,ps1,ps2,ps3,,,]



  ogg page の塊がoggコンテナ（実ファイル）となっています
  pageの種類を区別するのにシリアルナンバーが振ってあります
  oggが勝手に割り振るので、固定してはいけません

  page,page,page,,,,



  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1]);
  stream.PageIn(page[pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[theora]
  を参照！
*/



using namespace Maid;

//  ここの実装はデコードスレッドが使う関数を書いておきます
//  ゲーム側が使う関数は 1.cpp を参照



//  シーク命令が出ているか？
bool OggPlayer::IsSetSeek()const
{
  return m_SeekPosition>=0.0;
}




unt OggPlayer::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
//  MAID_WARNING( "InitializeStream" );
  InitializeStream( m_FileName );

  while( true )
  {
    const double pos = std::max(0.0,m_SeekPosition);
    m_SeekPosition = -1.0f;
//  MAID_WARNING( "PlayDecode IN" );
    PlayDecode( pos, brige );
//  MAID_WARNING( "PlayDecode EXIT" );

    //  関数を出たということは、スレッドの終了か、シーク命令がでたことになる
    if( brige.IsExit ) { break; }

    //  スレッド終了じゃないので、シーク用に再構築する

    FinalizeStream();
    InitializeStream( m_FileName );
  }
//  MAID_WARNING( "FinalizeStream" );

  m_State = STATE_FINALIZING;
  FinalizeStream();
  m_State = STATE_EMPTY;

  return 0;
}





void OggPlayer::InitializeStream( const String& FileName )
{
  m_Buffer.Initialize( FileName );

  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    Xiph::OggPage page;
    const bool IsSuccess = m_Buffer.PageOut( page );

    if( !IsSuccess ) { continue; }
    if( !page.IsBeginOfStream() ) { break; }

    const int serial = page.GetSerialNo();

    {
      boost::shared_ptr<DATA> pData( new DATA );
      pData->Stream.Initialize( serial );
      pData->Stream.PageIn( page );

      Xiph::OggPacket packet;
      
      pData->Stream.PacketOut( packet );
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      Xiph::SPDECODER pDec;
      Xiph::SPSAMPLECACHE pCache;

      {
        if( theora_packet_isheader(&p)==1 ) 
        {
          boost::shared_ptr<Xiph::DecoderTheora> th( new Xiph::DecoderTheora );

          th->Initialize(packet);
          pDec = th;
          m_TheoraSerial = serial;

//        MAID_WARNING( "theora:" << serial );

        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::DecoderVorbis> vb( new Xiph::DecoderVorbis );
          vb->Initialize(packet);

          pDec = vb;
          m_VorbisSerial = serial;
//        MAID_WARNING( "vorbis:" << serial );
        }
        else 
        {
          pDec.reset( new Xiph::DecoderNULL ); 
        }
      }

      //  最初のページは１パケットしかないのでここでループする必要はない
      pData->pDecoder = pDec;
      m_BindData[serial] = pData;
    }
  }

  //  すべてのデコーダの準備が整うまでデータを流し続ける

  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    bool IsOK = true;
    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      const Xiph::IDecoder& dec = *(ite->second->pDecoder);

      if( dec.IsSetupped() ) { continue; }

      DATA& dat = *(ite->second);

      Xiph::IDecoder&     decoder = *(dat.pDecoder);
      Xiph::OggStream&    stream  = dat.Stream;

      while( true )
      {
        if( decoder.IsSetupped()   ) { break; }
        Xiph::OggPacket packet;
        const bool IsSucess = stream.PacketOut( packet );
        if( !IsSucess ) { PageSeek( stream.GetSerialNo() ); break; }
        decoder.Decode(packet, Xiph::SPBUFFER() );
      }

      IsOK = false;
    }
    if( IsOK ) { break; }
  }


  { //  ここまできたということは、デコード準備ができているってことなので
    //  各種フォーマットを決める

    FILEINFO& Info = m_FileInfo;

    Info.IsImage = false;
    Info.IsPCM = false;

    if( m_TheoraSerial!=DECODER_EMPTY )
    {
      Info.IsImage = true;
      BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
      const DATA& dat = *(ite->second);

      const theora_info& th = boost::static_pointer_cast<Xiph::DecoderTheora>(dat.pDecoder)->GetInfo();

      FILEINFO::IMAGE& img = Info.Image;
      img.EncodedSize = SIZE2DI(th.width, th.height);
      img.DisplaySize = SIZE2DI(th.frame_width,th.frame_width);
      img.DisplayOffset = POINT2DI( th.offset_x, th.offset_y );

      img.FpsNumerator = th.fps_numerator;
      img.FpsDenominator = th.fps_denominator;
      img.AspectNumerator = th.aspect_numerator;
      img.AspectDenominator = th.aspect_numerator;

      switch( th.pixelformat )
      {
      case OC_PF_444: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV444; }break;
      case OC_PF_422: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV422; }break;
      case OC_PF_420: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV420; }break;
      default: { MAID_ASSERT( true, "不明" ); }break;
      }
    }

    if( m_VorbisSerial!=DECODER_EMPTY )
    {
      Info.IsPCM = true;

      BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
      const DATA& dat = *(ite->second);

      boost::shared_ptr<Xiph::DecoderVorbis> pVorbis = boost::static_pointer_cast<Xiph::DecoderVorbis>(dat.pDecoder);
      const vorbis_info& vi = pVorbis->GetInfo();

      FILEINFO::PCM& pcm = Info.Pcm;

      pcm.Format.SamplesPerSecond = vi.rate;
      pcm.Format.Channels = vi.channels;
      pcm.Format.BitPerSamples = 16;
    }
  }

}

void OggPlayer::PlayDecode( double StartTime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  //  ０より大きいときは指定箇所まで飛ぶ必要があるので全部シークモードにする
  //  OggPage の GranulePosition をみて飛んでいく方法もあるけど
  //  なんかめんどそうなのでしない。　改善案ですね。
  if( 0<StartTime )
  {
    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      DATA& dat = *(ite->second);
      dat.pDecoder->BeginSeekMode( StartTime );
    }
  }

  const unt timer_begin = m_Timer.Get();


  //  ではデコードの開始
  //  シークモードは各デコーダが勝手に解除します。
  bool IsLoop = true;
  while( IsLoop )
  {
    if( brige.IsExit ) { return; }
    if( IsSetSeek()  ) { break; }

    //  キャッシュが全部いっぱいになっているならお休みする
    Sleep( brige );

    const unt timer_current = m_Timer.Get() - timer_begin;
    const double NowTime = double(timer_current) / 1000.0;

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      if( IsCacheFull(ite->first) ) { continue; }

      DATA& dat = *(ite->second);

      Xiph::IDecoder&     decoder = *(dat.pDecoder);
      Xiph::OggStream&    stream  = dat.Stream;
      Xiph::SampleCache&  cache   = dat.Cache;

      Xiph::OggPacket packet;
      const bool IsSuccess = stream.PacketOut(packet);

      //  パケットがなかったらページを注入してもらう
      if( !IsSuccess ) { PageSeek( stream.GetSerialNo() ); }
      else
      {
        //  パケットがあるならデコード
        Xiph::SAMPLE  sample;
        sample.BeginTime = decoder.GetTime();
        decoder.Decode(packet,sample.pBuffer);
        sample.EndTime = decoder.GetTime();

        if( StartTime<=sample.BeginTime && sample.pBuffer.get()!=NULL )
        {
          cache.PushBack( sample );
        }
/*
        if( stream.GetSerialNo()==m_TheoraSerial )
        {
          MAID_WARNING( "Decode:" << sample.BeginTime <<"-" << sample.EndTime );
        }
*/
      }

      {
        //  デコードが遅れていたらシークモードに以降
        const double sa = NowTime - decoder.GetTime();
        if( 1.0 < sa )
        {
          decoder.BeginSeekMode( NowTime + 1.0 );
        }
      }

    }

    //  ストリームがすべて終了していたらスリープする
    StreamCheck( brige );
  }
}


//  指定したシリアルのストリームを充填する
void OggPlayer::PageSeek( int Serial )
{
  while( true )
  {
    //  終端にいたら終了
    if( m_Buffer.IsEnd() ) { break; }

    Xiph::OggPage page;
    const bool IsSuccess = m_Buffer.PageOut(page);

    if( !IsSuccess ) { continue; }

    const int PageSerial = page.GetSerialNo();

    DATA& dat = *(m_BindData[PageSerial]);

    dat.Stream.PageIn(page);

    if( PageSerial==Serial ) { break; }
  }
}


bool OggPlayer::IsCacheFull( int DecoderSerial ) const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);

  if( ite==m_BindData.end() ) { return true; }

  const Xiph::OggStream& stream  = ite->second->Stream;
  const Xiph::SampleCache& cache = ite->second->Cache;

  if( stream.IsEnd() ) { return true; }

  if( m_TheoraSerial==DecoderSerial )
  {
    //  動画は５フレーム欲しい
    const size_t count = cache.GetSize();
    return 5 <= count;
  }
  ef( m_VorbisSerial==DecoderSerial )
  {
    //  音声は２秒欲しい
    const double time = cache.GetTotalTime();

    return 2.0 <= time;
  }

  return true;
}



void OggPlayer::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    //  すべてのストリームを調べてバッファがそろっているなら、減るまで待機

    if( !IsCacheFull(m_TheoraSerial) ) { return ; }
    if( !IsCacheFull(m_VorbisSerial) ) { return ; }

    m_State = STATE_WORKING;
    ThreadController::Sleep(1);
  }
}

void OggPlayer::StreamCheck( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      DATA& dat = *(ite->second);

      Xiph::IDecoder&     decoder = *(dat.pDecoder);
      Xiph::OggStream&    stream  = dat.Stream;
      Xiph::SampleCache&  cache   = dat.Cache;

      if( !stream.IsEnd() ) { return ; }
    }
    ThreadController::Sleep(1);
  }
}


void OggPlayer::FinalizeStream()
{
  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    DATA& dat = *(ite->second);

    dat.pDecoder->Finalize();
    dat.Cache.Clear();
    dat.Stream.Finalize();
  }

  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;
  m_BindData.clear();
  m_Buffer.Finalize();
}