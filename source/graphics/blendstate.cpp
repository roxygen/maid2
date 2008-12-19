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
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::hBlendState obj = pDevice->CreateBlendState( state );
  IBlendState::Set( obj );
}

void BlendState::Delete()
{
  const Graphics::hBlendState& obj = IBlendState::Get();
  if( obj.empty() ) { return ; }

  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  pDevice->DeleteObject( obj );
  IBlendState::Clear();
}

}