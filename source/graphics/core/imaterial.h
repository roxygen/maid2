#ifndef maid2_graphics_core_imaterial_h
#define maid2_graphics_core_imaterial_h


#include"../../config/define.h"
#include"iview.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IMaterial : IView
  {
  public:
  };

  typedef	boost::shared_ptr<IMaterial>	SPMATERIAL;
}}

#endif