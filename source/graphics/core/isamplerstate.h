#ifndef maid2_graphics_core_isamplerstate_h
#define maid2_graphics_core_isamplerstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class ISamplerState : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<ISamplerState>	SPSAMPLERSTATE;
}}

#endif