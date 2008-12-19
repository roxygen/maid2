#include"vertex.h"


namespace Maid
{

Vertex::Vertex()
{

}

Vertex::~Vertex()
{
  Delete();
}

void Vertex::Create( size_t Length )
{
  Create( NULL, Length );
}

void Vertex::Create( const void* pData, size_t Length )
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::CREATEVERTEXPARAM param;
  param.pData  = pData;
  param.Length = Length;

  Graphics::hVertex obj = pDevice->CreateVertex( param );

  IVertex::Set( obj );
}

void Vertex::Delete()
{
  const Graphics::hVertex& obj = IVertex::Get();
  if( obj.empty() ) { return ; }

  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  pDevice->DeleteObject( obj );
  IVertex::Clear();

}

}