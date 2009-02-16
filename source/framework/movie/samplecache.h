#ifndef maid2_framework_xiph_samplecache_h
#define maid2_framework_xiph_samplecache_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"ibuffer.h"

#include <boost/smart_ptr.hpp>

namespace Maid { namespace Xiph {

  typedef std::list<OGGSAMPLE> OGGSAMPLELIST;

  class SampleCache
  {
  public:
    SampleCache();
    ~SampleCache();
    void PushBack( const SAMPLE& sample );
    void Clear();

    double GetBeginTime() const;
    double GetTotalTime() const;
    size_t GetSize() const;
    int Pop( double EndTime, SAMPLELIST& Out );

  private:
    SAMPLELIST  m_List;
  };
}}
 
#endif