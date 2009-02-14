#ifndef maid2_framework_xiph_samplecache_h
#define maid2_framework_xiph_samplecache_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"ibuffer.h"

#include <boost/smart_ptr.hpp>

namespace Maid { namespace Xiph {

  struct SAMPLE
  {
    double BeginTime; //  開始時間（単位は秒）
    double EndTime;   //  終了時間（単位は秒）
    SPBUFFER pBuffer;

    //  BeginTime <= x < EndTime の範囲になる
  };
  typedef std::list<SAMPLE> SAMPLELIST;

  class SampleCache
  {
  public:
    SampleCache();
    ~SampleCache();
    void PushBack( const SAMPLE& sample );
    void Clear();

    double GetTotalTime() const;
    size_t GetSize() const;
    int Pop( double EndTime, SAMPLELIST& Out );

  private:
    SAMPLELIST  m_List;
  };

  class ISampleCacheChecker
  {
  public:
    virtual ~ISampleCacheChecker(){}

    virtual bool IsFull( const Xiph::SampleCache& cache ) const=0;
  };

  typedef boost::shared_ptr<ISampleCacheChecker> SPSAMPLECACHECHECKER;

  class SampleCacheCheckerCount
    :public ISampleCacheChecker
  {
  public:
    SampleCacheCheckerCount( size_t num )
      :m_Count(num)
    {
    }

    virtual bool IsFull( const Xiph::SampleCache& cache ) const
    {
      const size_t count = cache.GetSize();
      return m_Count <= count;
    }

  private:
    const size_t  m_Count;
  };


  class SampleCacheCheckerTime
    :public ISampleCacheChecker
  {
  public:
    SampleCacheCheckerTime( double time )
      :m_Time(time)
    {
    }

    virtual bool IsFull( const Xiph::SampleCache& cache ) const
    {
      const double time = cache.GetTotalTime();
      return m_Time <= time;
    }
  private:
    const double  m_Time;
  };

}}
 
#endif