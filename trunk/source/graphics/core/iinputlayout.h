#ifndef maid2_graphics_core_iinputlayout_h
#define maid2_graphics_core_iinputlayout_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IInputLayout : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IInputLayout>	SPINPUTLAYOUT;
}}

#endif