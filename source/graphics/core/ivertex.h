#ifndef maid2_graphics_core_ivertex_h
#define maid2_graphics_core_ivertex_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IVertex : public IResource
  {
  public:
    IVertex():IResource(TYPE_VERTEX){}
  };

  typedef	boost::shared_ptr<IVertex>	SPVERTEX;
}}

#endif