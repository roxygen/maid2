#include"blendstate.h"


namespace Maid
{

BlendState::BlendState()
{

}

BlendState::~BlendState()
{
  Delete();
}

void BlendState::Create( const Graphics::BLENDSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::SPBLENDSTATE obj = pDevice->CreateBlendState( state );
  IBlendState::Set( obj );
}

void BlendState::Delete()
{
  IBlendState::Clear();
}

}