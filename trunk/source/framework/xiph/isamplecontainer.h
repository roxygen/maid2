#ifndef framework_xiph_idecoder_h
#define framework_xiph_isamplecontainer_h

#include"../../config/define.h"

#include"idecodedbuffer.h"

#include <boost/smart_ptr.hpp>

namespace Xiph {


  struct SAMPLE
  {
    double BeginTime; //  開始時間（単位は秒）
    double EndTime;   //  終了時間（単位は秒）
    SPDECODEDBUFFER pBuffer;

    //  BeginTime <= x < EndTime の範囲になる
  };

  typedef std::list<OGGDECODESAMPLE> OGGDECODESAMPLELIST;


  class IDecoder
  {

  };

}
 
#endif