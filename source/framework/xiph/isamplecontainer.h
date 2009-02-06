#ifndef framework_xiph_isamplecontainer_h
#define framework_xiph_isamplecontainer_h

#include"../../config/define.h"

#include"ibuffer.h"

#include <boost/smart_ptr.hpp>

namespace Xiph {

  class ISampleContainer
  {
  public:

    void Add( double begin, double end, const SPBUFFER& pBuffer );


    //  溜まっているバッファから指定した時間のデータを抜き出す
    virtual void Flush( double TargetTime, SPBUFFER& Out )=0;

    //  バッファリングしてる最中か？
    virtual bool IsBuffering() const=0;

  protected:
    struct SAMPLE
    {
      double BeginTime; //  開始時間（単位は秒）
      double EndTime;   //  終了時間（単位は秒）
      SPBUFFER pBuffer;

      //  BeginTime <= x < EndTime の範囲になる
    };
    typedef std::list<SAMPLE> SAMPLELIST;

    SAMPLELIST  m_List;

  };

}
 
#endif