#ifndef maid2_graphics_core_iindex_h
#define maid2_graphics_core_iindex_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IIndex : public IResource
  {
  public:
    IIndex():IResource(TYPE_INDEX){}
  };

  typedef	boost::shared_ptr<IIndex>	SPINDEX;
}}

#endif