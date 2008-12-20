#ifndef maid2_graphics_core_iconstant_h
#define maid2_graphics_core_iconstant_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IConstant : public IResource
  {
  public:
    IConstant():IResource(TYPE_CONSTANT){}
  };

  typedef	boost::shared_ptr<IConstant>	SPCONSTANT;
}}

#endif