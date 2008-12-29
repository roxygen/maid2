#include"rendertargetscreen.h"


namespace Maid
{

RenderTargetScreen::RenderTargetScreen()
{
  Graphics::SPRENDERTARGET obj = GlobalPointer<GraphicsCore>::Get()->GetDevice()->GetDefaultRenderTarget();

  IRenderTarget::Set( obj );
}


SIZE2DI RenderTargetScreen::GetSize() const
{
  const Graphics::SPRESOURCE pResource = IRenderTarget::Get()->GetResource();

  const Graphics::ITexture2D* pTexture = static_cast<const Graphics::ITexture2D*>(pResource.get());

  return pTexture->GetParam().Size;

}

}