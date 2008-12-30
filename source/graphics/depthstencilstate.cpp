#include"depthstencilstate.h"


namespace Maid
{


void DepthStencilState::Create( const Graphics::DEPTHSTENCILSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPDEPTHSTENCILSTATE obj = pDevice->CreateDepthStencilState( state );
  IDepthStencilState::Set( obj );
}

void DepthStencilState::Delete()
{
  IDepthStencilState::Clear();
}

}