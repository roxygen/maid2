#ifndef maid2_framework_streamdecodermultithread_h
#define maid2_framework_streamdecodermultithread_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/icodec.h"
#include"mediatimer.h"
#include"isample.h"
#include"streamdecoder.h"
#include"streamsamplecache.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IStreamDecoderMultiThread
  {
  public:
    IStreamDecoderMultiThread();

    void Initialize( const IMediaTimer& Timer, const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );
    int PopCache( double EndTime, STREAMSAMPLELIST& Out );

    void BeginSeek( double TargetTime );
    bool IsSeeking() const;

    void Update( STREAMSAMPLE& out );

    int  GetPacketCount() const;
    bool IsStreamEnd() const;
    bool IsPacketEmpty() const;

    virtual bool IsCacheFull() const=0;

    size_t GetCacheCount() const;
    double GetCacheTime() const;



  private:
    const IMediaTimer*   m_pTimer;

    StreamDecoder m_Decoder;

    StreamSampleCache m_Cache;


    ThreadMutex m_DecoderMutex; //  m_Decoder の更新用
    ThreadMutex m_SampleMutex;  //  m_Cache の更新用

    unt   m_SleepCounter; //  一定量たまるとsleepする


    //  同期待ちが頻繁に起こらないようにするキャッシュ
    double  m_TotalTime;
    double  m_BeginTime;
    size_t  m_SampleCount;
    int     m_PacketCount;  //  処理できるパケットが後何個あるか？
    bool    m_IsSeeking;    //  シーク中か？
    bool    m_IsStreamEnd;

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };

  typedef boost::shared_ptr<IStreamDecoderMultiThread> SPSTREAMDECODERMULTITHREAD;

  class StreamDecoderMultiThreadVideo 
    : public IStreamDecoderMultiThread
  {
  public:
    StreamDecoderMultiThreadVideo( size_t count )
      :m_Count(count){}

    virtual bool IsCacheFull() const
    {
      return m_Count < GetCacheCount();
    }

  private:
    const size_t  m_Count;
  };

  class StreamDecoderMultiThreadSound 
    : public IStreamDecoderMultiThread
  {
  public:
    StreamDecoderMultiThreadSound( double time )
      :m_Time(time){}

    virtual bool IsCacheFull() const
    {
      return m_Time < GetCacheTime();
    }

  private:
    const double  m_Time;
  };

}

#endif
