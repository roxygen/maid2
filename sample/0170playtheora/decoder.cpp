#include"decoder.h"

#include"../../source/auxiliary/mathematics.h"
#include"../../source/auxiliary/debug/warning.h"
#include"oggdecodernull.h"

/*

  ogg の内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1,pv2,pv3]   page[ps0,ps1,ps2,ps3]




  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1,pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[program,theora]
  を参照！
*/



using namespace Maid;


static const int DECODER_EMPTY = -1;

Decoder::Decoder()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
{

}

void Decoder::Initialize()
{
  m_State = STATE_INITIALIZING;
  m_Thread.SetFunc( MakeThreadObject(&Decoder::ThreadFunction,this) );
  m_Thread.Execute();

/*  //  ちょっとしたテスト用のコード。そのうち消す
  ThreadController::BRIGEDATA brige;
  brige.IsExit = false;
  brige.IsExecute = false;
  brige.IsAssert = false;

  ThreadFunction( brige );
*/
}

bool Decoder::IsInitialized() const
{
  //  デコーダーの構築も終わってないならまだ
  if( m_State!=STATE_WORKING ) { return false; }

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = ite->second;

    boost::shared_ptr<OggDecoderTheora> pTheora = boost::static_pointer_cast<OggDecoderTheora>(dat.pDecoder);

    if( !pTheora->IsSetupped() ) { return false; }
  }

  if( m_VorbisSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
    const DATA& dat = ite->second;

//    boost::shared_ptr<OggDecoderTheora> pTheora = boost::static_pointer_cast<OggDecoderTheora>(dat.pDecoder);
//    if( !pTheora->IsSetupped() ) { return false; }
  }

  return true;
}

void Decoder::GetFileInfo( FILEINFO& Info ) const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );

  Info.IsImage = false;
  Info.IsPCM = false;

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    Info.IsImage = true;
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = ite->second;

    const theora_info& th = boost::static_pointer_cast<OggDecoderTheora>(dat.pDecoder)->GetInfo();

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

}

void Decoder::FlushImage( double time, SPOGGDECODEDBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }

  DATA& dat = m_BindData[m_TheoraSerial];

  //  dat.DecodedInfo は時間が古い順に並んでいるので、それをtimeより新しいものを探す
  {
    ThreadMutexLocker lock( dat.Mutex );
    std::list<OGGDECODEDINFO>& InfoList = dat.DecodedInfo;

    while( true )
    {
      if( InfoList.empty() ) { break; }

      std::list<OGGDECODEDINFO>::iterator ite = InfoList.begin();

      if( time <= ite->Time )
      {
        pOutput = boost::static_pointer_cast<OggDecodedBufferImage>(ite->pBuffer);
      }
      
      //  ついでに古いものは破棄
      InfoList.erase(ite);

      if( pOutput.get()!=NULL ) { break; }
    }
  }
}

void Decoder::FlushPCM( std::vector<unt08>& Buffer )
{
  if( !IsInitialized() ) { return ; }

}



unt Decoder::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  m_Buffer.Initialize();

  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    const OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    const int serial = page.GetSerialNo();
    DATA& data = m_BindData[serial];

    {
      data.Stream.Initialize( serial );
      data.Stream.PageIn( page );

      const OggPacket& packet = data.Stream.GetCurrentPacket();
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      SPOGGDECODER pDec;

      {
        if( theora_packet_isheader(&p)==1 ) 
        {
          pDec.reset( new OggDecoderTheora );
          m_TheoraSerial = serial;
        }
/*
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          pDec.reset( new OggDecoderVorbis );
          m_VorbisSerial = serial;
        }
*/
        else { pDec.reset( new OggDecoderNULL ); }
      }
      pDec->Initialize();

      //  最初のページは１パケットしかない&デコードしないのでここでループする必要はない
      pDec->PacketIn(packet);

      data.pDecoder = pDec;
    }

    m_Buffer.NextPage();
  }


  //  ではエンコードの開始
  while( true )
  {
    if( brige.IsExit ) { break; }


    if( m_Buffer.IsEnd() )
    { //  末端まですすんだらお休み
      ThreadController::Sleep(1);
      continue;
    }

    const OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());

    MAID_ASSERT( ite==m_BindData.end(), "仕様外のページが見つかりました" );

    DATA& dat = ite->second;

    dat.Stream.PageIn(page);


    {
      //  シーク命令があったら、ここでシークする
      //  パケットの途中にシークすることもあるので、調節がむずいぞと。

    }


    while( true )
    {
      if( dat.Stream.IsPacketEmpty() ) { break; }

      Sleep( dat, brige );

      const OggPacket& packet = dat.Stream.GetCurrentPacket();

      dat.pDecoder->PacketIn(packet);

      if( dat.pDecoder->IsDecoding() )
      {
        OGGDECODEDINFO info;
        info.Time = dat.pDecoder->GetTime();
        info.pBuffer = dat.pDecoder->CreateDecodedBuffer();

        dat.Mutex.Lock();
        dat.DecodedInfo.push_back(info);
        dat.Mutex.Unlock();
      }

      dat.Stream.NextPacket();
    }


    m_Buffer.NextPage();
  }

  m_State = STATE_FINALIZING;

  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    DATA& dat = ite->second;

    dat.pDecoder->Finalize();
    dat.Stream.Finalize();
  }

  m_BindData.clear();


  m_Buffer.Finalize();

  m_State = STATE_EMPTY;
  return 0;
}

int Decoder::Sleep( DATA& dat,  volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    //  バッファがある程度あるなら、減るまで待機
    size_t count = 0;
    {
      dat.Mutex.Lock();
      count = dat.DecodedInfo.size();
      dat.Mutex.Unlock();
    }

    if( count<5 ) 
    {
      break; 
    }
    else 
    {
      m_State = STATE_WORKING;
      ThreadController::Sleep(1);
    }
  }

  return 0;
}

