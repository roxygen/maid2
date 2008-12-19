#include"rasterizerstate.h"


namespace Maid
{

RasterizerState::RasterizerState()
{

}

RasterizerState::~RasterizerState()
{
  Delete();
}

void RasterizerState::Create( const Graphics::IDevice::RASTERIZERSTATEPARAM& state )
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::hRasterizerState obj = pDevice->CreateRasterizerState( state );
  IRasterizerState::Set( obj );
}

void RasterizerState::Delete()
{
  const Graphics::hRasterizerState& obj = IRasterizerState::Get();
  if( obj.empty() ) { return ; }

  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  pDevice->DeleteObject( obj );
  IRasterizerState::Clear();
}

}