#include"inputlayout.h"


namespace Maid
{

InputLayout::InputLayout()
{

}

InputLayout::~InputLayout()
{
  Delete();
}

void InputLayout::Create( const Graphics::INPUT_ELEMENT* pElement, int Count,  const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::SPINPUTLAYOUT obj = pDevice->CreateInputLayout( pElement, Count, pShaderBytecodeWithInputSignature, BytecodeLength );
  IInputLayout::Set( obj );
}

void InputLayout::Delete()
{
  IInputLayout::Clear();
}

}