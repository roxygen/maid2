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

  page[pv0,pv1,pv2,pv3]   page[ps0,ps1,ps2,ps3]




  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1,pv2,pv3]);
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
  InitializeStream( m_FileName );

  while( true )
  {
    const double pos = std::max(0.0,m_SeekPosition);
    m_SeekPosition = -1.0f;
    PlayDecode( pos, brige );

    //  関数を出たということは、スレッドの終了か、シーク命令がでたことになる
    if( brige.IsExit ) { break; }

    //  スレッド終了じゃないので、シーク用に再構築する

    FinalizeStream();
    InitializeStream( m_FileName );
  }

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

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    const int serial = page.GetSerialNo();

    {
      boost::shared_ptr<DATA> pData( new DATA );
      pData->Stream.Initialize( serial );
      pData->Stream.PageIn( page );

      const Xiph::OggPacket& packet = pData->Stream.GetCurrentPacket();
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

          //m_FileInfo
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::DecoderVorbis> vb( new Xiph::DecoderVorbis );
          vb->Initialize(packet);

          pDec = vb;
          m_VorbisSerial = serial;
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

    m_Buffer.NextPage();
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

void OggPlayer::SeekStream( double StartTime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  bool IsSeeking = true;


  while( IsSeeking )
  {
    if( brige.IsExit ) { return; }
    if( m_Buffer.IsEnd() ) { break; }

    {
      //  全デコーダが時間まで進んだら終了
      bool IsBreak = true;
      for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
      {
        DATA& dat = *(ite->second);

        const double now = dat.pDecoder->GetTime();

        if( dat.pDecoder->IsSetupped() && StartTime <= now ) { continue; }

        IsBreak = false;
        break;
      }
      if( IsBreak ) { break; }
    }


    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());

    DATA& dat = *(ite->second);

    Xiph::IDecoder&     decoder = *(dat.pDecoder);
    Xiph::OggStream&    stream  = dat.Stream;
    Xiph::SampleCache&  cache   = dat.Cache;

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      const Xiph::OggPacket& packet = stream.GetCurrentPacket();


      if( !decoder.IsSetupped() )
      {
        //  ストリームの初期化中なら、パケットを送るだけ
        decoder.Decode(packet, Xiph::SPBUFFER() );

        //  初期化が終わったら＝＝シーク開始の合図
        if( decoder.IsSetupped() )
        {
          decoder.BeginSeekMode(StartTime);
        }
      }else
      {
        Xiph::SAMPLE  sample;
        sample.BeginTime = decoder.GetTime();
        decoder.Decode(packet,sample.pBuffer);
        sample.EndTime = decoder.GetTime();

        if( sample.pBuffer.get()!=NULL )
        {
          cache.PushBack( sample );
        }
      }

      stream.NextPacket();
    }
    m_Buffer.NextPage();
  }

}

void OggPlayer::PlayDecode( double begintime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  //  目標時間までシークする
  {
    SeekStream( begintime, brige );
  }

  //  ではデコードの開始
  while( true )
  {
    if( brige.IsExit ) { return; }
    if( IsSetSeek()  ) { break; }

    if( m_Buffer.IsEnd() )
    { //  末端まですすんだらお休み
      ThreadController::Sleep(1);
      continue;
    }

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());
    DATA& dat = *(ite->second);

    Xiph::IDecoder&     decoder = *(dat.pDecoder);
    Xiph::OggStream&    stream  = dat.Stream;
    Xiph::SampleCache&  cache   = dat.Cache;

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      if( IsSetSeek()  ) { break;; }

      const Xiph::OggPacket& packet = stream.GetCurrentPacket();
/*
    //  デコード処理が遅れていたら、このあたりにシーク処理を入れる
    //  とうめん放置
*/


      Xiph::SAMPLE  sample;
      sample.BeginTime = decoder.GetTime();
      decoder.Decode(packet,sample.pBuffer);
      sample.EndTime = decoder.GetTime();

      if( sample.pBuffer.get()!=NULL )
      {
        cache.PushBack( sample );
      }

      stream.NextPacket();
    }

    Sleep( brige );

    m_Buffer.NextPage();
  }

}






void OggPlayer::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    //  すべてのストリームを調べてバッファがそろっているなら、減るまで待機

    if( m_TheoraSerial!=DECODER_EMPTY )
    {
      //  動画は８フレーム欲しい
       BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
       if( ite->second->Cache.GetSize() < 8 ) { return ; }
    }

    if( m_VorbisSerial!=DECODER_EMPTY )
    {
      //  音声は２秒欲しい
       BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
       if( ite->second->Cache.GetTotalTime() < 2.0 ) { return ; }
    }

    m_State = STATE_WORKING;
    ThreadController::Sleep(1);
  }
}
