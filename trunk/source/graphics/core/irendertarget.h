#ifndef maid2_graphics_core_irendertarget_h
#define maid2_graphics_core_irendertarget_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IRenderTarget : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IRenderTarget>	SPRENDERTARGET;
}}

#endif