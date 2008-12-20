#ifndef maid2_graphics_core_irasterizerstate_h
#define maid2_graphics_core_irasterizerstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IRasterizerState : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IRasterizerState>	SPRASTERIZERSTATE;
}}

#endif