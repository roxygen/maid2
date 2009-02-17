#ifndef maid2_framework_xiph_ibuffer_h
#define maid2_framework_xiph_ibuffer_h


#include"../../../config/define.h"
#include"../../../auxiliary/buffer.h"
#include"../../../auxiliary/mathematics.h"

#include <boost/smart_ptr.hpp>
#include <list>



namespace Maid { namespace Xiph {

  class IBuffer
  {
  public:
    virtual ~IBuffer(){}
  };

  typedef boost::shared_ptr<IBuffer> SPBUFFER;


}}

#endif
