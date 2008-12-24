#include"rendertargetscreen.h"


namespace Maid
{

RenderTargetScreen::RenderTargetScreen()
{
  Graphics::SPRENDERTARGET obj = GlobalPointer<GraphicsCore>::Get()->GetDevice()->GetDefaultRenderTarget();

  IRenderTarget::Set( obj );
}
	
}