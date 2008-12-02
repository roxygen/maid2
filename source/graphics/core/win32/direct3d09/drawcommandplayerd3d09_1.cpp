#include"drawcommandplayerd3d09.h"

namespace Maid { namespace Graphics {

DrawCommandPlayerD3D09::DrawCommandPlayerD3D09( const SPD3D09DEVICE& pDev, const std::vector<SPOBJECTINFO>& ObjectList )
  :m_pDevice(pDev), m_ObjectList(ObjectList)
{

}

void DrawCommandPlayerD3D09::Begin()
{

}

void DrawCommandPlayerD3D09::End()
{

}

void DrawCommandPlayerD3D09::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{

}

void DrawCommandPlayerD3D09::ClearRenderTarget( const COLOR_A32B32G32R32F& Col )
{

  m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0x80), 0, 0 );

}


}}

