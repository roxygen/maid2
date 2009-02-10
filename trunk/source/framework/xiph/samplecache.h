#ifndef framework_xiph_samplecache_h
#define framework_xiph_samplecache_h

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
    void Pop( double EndTime, SAMPLELIST& Out );

    double GetBeginTime() const { return m_BeginTime; }

  private:
    void UpdateCache();

  private:
    ThreadMutex m_Mutex;  //  これのあるクラスとないクラスがあったほうが便利かも？？
    SAMPLELIST  m_List;

    //  同期待ちが頻繁に起こらないようにするキャッシュ
    double m_BeginTime;
    double m_TotalTime;
    size_t m_SampleCount;
  };

  typedef boost::shared_ptr<SampleCache> SPSAMPLECACHE;

}}
 
#endif