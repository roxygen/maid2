#include"graphicsdeviced3d09.h"


#include"../../../../auxiliary/exception.h"
#include"../../../../auxiliary/debug/trace.h"
#include"../../../../auxiliary/debug/warning.h"
#include"../../../../auxiliary/string.h"


namespace Maid
{

	//	ここには ClearXXX, ポリゴン描画系を書く

D3DPRIMITIVETYPE toD3DPRIMITIVETYPE( IGraphicsDevice::PRIMITIVE prim )
{
	D3DPRIMITIVETYPE t = D3DPT_TRIANGLELIST;
	switch( prim )
	{
	case IGraphicsDevice::PRIMITIVE_POINTLIST:		{ t = D3DPT_POINTLIST;		}break;
	case IGraphicsDevice::PRIMITIVE_LINELIST:		{ t = D3DPT_LINELIST;		}break;
	case IGraphicsDevice::PRIMITIVE_LINESTRIP:		{ t = D3DPT_LINESTRIP;		}break;
	case IGraphicsDevice::PRIMITIVE_TRIANGLELIST:	{ t = D3DPT_TRIANGLELIST;	}break;
	case IGraphicsDevice::PRIMITIVE_TRIANGLESTRIP:	{ t = D3DPT_TRIANGLESTRIP;	}break;
	case IGraphicsDevice::PRIMITIVE_TRIANGLEFAN:	{ t = D3DPT_TRIANGLEFAN;	}break;
	}

	return t;
}




//!	IVideoCardDriver::ClearColor() を参照
void GraphicsDeviceD3D09::ClearRenderTarget( const COLOR_A32B32G32R32F& Color )
{
/*
	HRESULT ret;
	ret = m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, Color, 0.0f, 0 );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::Clear()")); }
*/
}

//!	IVideoCardDriver::ClearColor() を参照
void GraphicsDeviceD3D09::ClearDepthStencil( float Depth, unt32 Stencil )
{
/*
	HRESULT ret;
	ret = m_pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, val, 0 );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::Clear()")); }
*/
}


//!	IVideoCardDriver::DrawPrimitive() を参照
void GraphicsDeviceD3D09::DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount )
{
/*
	D3DPRIMITIVETYPE t = toD3DPRIMITIVETYPE(prim);

	HRESULT ret;
	ret = m_pDevice->DrawPrimitive( t, StartVertex, PrimitiveCount );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::DrawPrimitive() ")); }
*/
}

//!	IVideoCardDriver::DrawPrimitiveUP() を参照
void GraphicsDeviceD3D09::DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount )
{
/*
	D3DPRIMITIVETYPE t = toD3DPRIMITIVETYPE(prim);

	HRESULT ret;
	ret = m_pDevice->DrawPrimitiveUP( t, PrimitiveCount, pVertex, Stride );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::DrawPrimitiveUP() ")); }
*/
}

void GraphicsDeviceD3D09::DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount )
{
/*
	D3DPRIMITIVETYPE t = toD3DPRIMITIVETYPE(prim);

	HRESULT ret;
	ret = m_pDevice->DrawIndexedPrimitive( t, BaseVertexOffset, MinIndex, NumVertices, StartIndex, PrimitiveCount );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::DrawIndexedPrimitive() ")); }
*/
}


void GraphicsDeviceD3D09::DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  )
{
/*
	D3DPRIMITIVETYPE t = toD3DPRIMITIVETYPE(prim);

	const  D3DFORMAT ifmt = IndexSize==2? D3DFMT_INDEX16 : D3DFMT_INDEX32;

	HRESULT ret;
	ret = m_pDevice->DrawIndexedPrimitiveUP( t, MinIndex, NumVertices, PrimitiveCount, pIndex, ifmt, pVertex, VertexSize );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::DrawIndexPrimitiveUP() ")); }
*/
}


}

