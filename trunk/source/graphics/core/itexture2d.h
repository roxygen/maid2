#ifndef maid2_graphics_core_itexture2d_h
#define maid2_graphics_core_itexture2d_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class ITexture2D : public IResource
  {
  public:
    ITexture2D():IResource(TYPE_TEXTURE2D){}
  };

  typedef	boost::shared_ptr<ITexture2D>	SPTEXTURE2D;
}}

#endif