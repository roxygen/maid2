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
    void PushBack( const SAMPLE& sample );
    void Clear();

    double GetBeginTime() const;
    double GetTotalTime() const;
    size_t GetSize() const;
    void PopFront( SAMPLE& Out );
    void PopAll( SAMPLELIST& Out );
    void Pop( double EndTime, SAMPLELIST& Out );

    bool IsEmpty() const;

    SAMPLE GetLastSample() const;

  private:
    mutable ThreadMutex m_Mutex;  //  これのあるクラスとないクラスがあったほうが便利かも？？
    SAMPLELIST  m_List;
  };

  typedef boost::shared_ptr<SampleCache> SPSAMPLECACHE;

}}
 
#endif