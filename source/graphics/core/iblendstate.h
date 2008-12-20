#ifndef maid2_graphics_core_iblendstate_h
#define maid2_graphics_core_iblendstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IBlendState : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IBlendState>	SPBLENDSTATE;
}}

#endif