#include"graphicsdeviced3d09.h"


#include"../../../../auxiliary/exception.h"
#include"../../../../auxiliary/debug/trace.h"
#include"../../../../auxiliary/debug/warning.h"
#include"../../../../auxiliary/string.h"


#include"SurfaceBufferD3D09.h"

namespace Maid
{

void GraphicsDeviceD3D09::SetRenderTarget( const ISurfaceBuffer* pColor, const ISurfaceBuffer* pDepthStencil )
{
/*
	const CSurfaceBufferD3D9* p = static_cast<const CSurfaceBufferD3D9*>(pBuffer);

	HRESULT ret;	
	ret = m_pDevice->SetRenderTarget( 0, p->pSurface.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetRenderTarget()")); }
*/
}


void GraphicsDeviceD3D09::ResetRenderTarget()
{
/*
	HRESULT ret;	
	ret = m_pDevice->SetDepthStencilSurface( m_pDefaultDepthStencil.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetDepthStencilSurface()")); }
*/
}




//! IVideoCardDriver::SetTextureBuffer() を参照
void GraphicsDeviceD3D09::SetTextureBuffer( int stage, const ITextureBuffer* pBuffer )
{
/*
	MAID_ASSERT( pBuffer.get()==NULL, "NULL" );
	CTextureBufferD3D9* p = static_cast<CTextureBufferD3D9*>(pBuffer.get());

	HRESULT ret;	
	ret = m_pDevice->SetTexture( stage, p->pTexture.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetTextureBuffer()")); }
*/
}

void GraphicsDeviceD3D09::SetIndexBuffer( const IIndexBuffer* pBuffer )
{
/*
	MAID_ASSERT( pBuffer.get()==NULL, "NULL" );
	CIndexBufferD3D9* p = static_cast<CIndexBufferD3D9*>(pBuffer.get());

	HRESULT ret;	
	ret = m_pDevice->SetIndices( p->pBuffer.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetIndices()")); }
*/
}

//! IVideoCardDriver::SetVertexBuffer() を参照
void GraphicsDeviceD3D09::SetVertexBuffer( int pos, const IVertexBuffer* pBuffer )
{
/*
	MAID_ASSERT( pBuffer.get()==NULL, "NULL" );
	CVertexBufferD3D9* p = static_cast<CVertexBufferD3D9*>(pBuffer.get());

	HRESULT ret;	
	ret = m_pDevice->SetStreamSource( pos, p->pBuffer.get(), 0, p->Stride );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetStreamSource()")); }
*/
}


//! IVideoCardDriver::SetVertexShader() を参照
void GraphicsDeviceD3D09::SetVertexShader( const IVertexShaderBuffer* pShader )
{
/*
	MAID_ASSERT( pShader.get()==NULL, "NULL" );

	CVertexShaderBufferD3D9* p = static_cast<CVertexShaderBufferD3D9*>(pShader.get());
	p->Setup(m_pDevice);
*/
}

//! IVideoCardDriver::SetPixelShader() を参照
void GraphicsDeviceD3D09::SetPixelShader( const IPixelShaderBuffer* pShader )
{
/*
	MAID_ASSERT( pShader.get()==NULL, "NULL" );

	CPixelShaderBufferD3D9* p = static_cast<CPixelShaderBufferD3D9*>(pShader.get());
	p->Setup(m_pDevice);
*/
}

void GraphicsDeviceD3D09::SetVertexDeclaration( const IVertexDeclarationBuffer* pDecl )
{
/*
	CVertexDeclarationBufferD3D9* p = static_cast<CVertexDeclarationBufferD3D9*>(pDecl.get());

	HRESULT ret;	
	ret = m_pDevice->SetVertexDeclaration( p->pBuffer.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetVertexDeclaration()")); }
*/
}

void GraphicsDeviceD3D09::SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ )
{
/*
	D3DVIEWPORT9 vp;
	vp.X      = screen.x;
	vp.Y      = screen.y;
	vp.Width  = screen.w;
	vp.Height = screen.h;
	vp.MinZ   = MinZ;
	vp.MaxZ   = MaxZ;

	HRESULT ret;	
	ret = m_pDevice->SetViewport( &vp );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetViewPort()")); }
*/
}


void GraphicsDeviceD3D09::SetVertexShaderConstF( int pos, const VECTOR4DF& vec )
{
/*
	HRESULT ret;	
	ret = m_pDevice->SetVertexShaderConstantF( pos, (float*)&vec, 1 );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetVertexShaderConstantF()")); }
*/
}

void GraphicsDeviceD3D09::CopySurface( const ISurfaceBuffer* pSrc, SurfaceBufferMemory& Dst )
{
/*
	CSurfaceBufferD3D9* pSurfaceD3D = static_cast<CSurfaceBufferD3D9*>(pSrc.get());
	CSurfaceBufferD3D9 Sys;

	const int w = pSurfaceD3D->Desc.Width;
	const int h = pSurfaceD3D->Desc.Height;
	const D3DFORMAT fmt = pSurfaceD3D->Desc.Format;

	//	まずはシステムメモリに持ってくる
	{
		{
			IDirect3DSurface9* p;

			const HRESULT ret = m_pDevice->CreateOffscreenPlainSurface( w, h, fmt, D3DPOOL_SYSTEMMEM, &p, NULL );
			if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateOffscreenPlainSurface()")); }
			Sys = CSurfaceBufferD3D9(p);
		}

		{
			const HRESULT ret = m_pDevice->GetRenderTargetData( pSurfaceD3D->pSurface.get(), Sys.pSurface.get() );
			if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetRenderTargetData()")); }
		}
	}

	{
		pDst.reset( new CSurfaceBufferMemory );
		pDst->Create( SIZE2DI(w,h), PIXELFORMAT_R08G08B08I );
	}

	//	次に転送
	const RECT2DI rc( 0, 0, w, h );

	CPlaneTransiter::Blt( *pDst, &rc, Sys, &rc );
*/
}

void GraphicsDeviceD3D09::SetRenderState( RENDERSTATE state, unt value )
{
/*
	D3DRENDERSTATETYPE type;
	DWORD v;

	switch( state )
	{
	case RENDERSTATE_CULLINGMODE:
		{
			type = D3DRS_CULLMODE;

			switch( value )
			{
			case CULLINGMODE_NONE: { v = D3DCULL_NONE; }break;
			case CULLINGMODE_LEFT: { v = D3DCULL_CCW; }break;
			case CULLINGMODE_RIGHT:{ v = D3DCULL_CW; }break;
			}
		}break;

	case RENDERSTATE_ZWRITEENABLE:
		{
			type = D3DRS_ZWRITEENABLE;

			switch( value )
			{
			case STATE_TRUE: { v = TRUE; }break;
			case STATE_FALSE:{ v = FALSE; }break;
			}

		}break;

	case RENDERSTATE_ZWRITEFUNC:
		{
			type = D3DRS_ZFUNC;

			v = CMPTYPE2D3DCMPFUNC( (IVideoCardDriver::CMPTYPE)value );
		}break;

	case RENDERSTATE_ZTESTENABLE:
		{
			type = D3DRS_ZENABLE;

			switch( value )
			{
			case STATE_TRUE: { v = D3DZB_TRUE; }break;
			case STATE_FALSE:{ v = D3DZB_FALSE; }break;
			}

		}break;
	}

	const HRESULT ret = m_pDevice->SetRenderState(type, v );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetRenderState()")); }
*/
}


void GraphicsDeviceD3D09::SetSamplerState( int Stage, SAMPLERSTATE State, unt Value )
{
/*
	D3DSAMPLERSTATETYPE t;
	DWORD v;

	switch( State )
	{
	case SAMPLERSTATE_MAGFILTER:
	case SAMPLERSTATE_MINFILTER:
		{
			if( State==SAMPLERSTATE_MAGFILTER ) { t = D3DSAMP_MAGFILTER; }
			ef( State==SAMPLERSTATE_MINFILTER ) { t = D3DSAMP_MINFILTER; }

			switch( Value )
			{
			case FILTERTYPE_POINT:		{ v = D3DTEXF_POINT; }break;
			case FILTERTYPE_LINEAR:		{ v = D3DTEXF_LINEAR; }break;
			case FILTERTYPE_ANISOTROPIC:{ v = D3DTEXF_ANISOTROPIC; }break;
			}
			
		}break;

	case SAMPLERSTATE_TADDRESSU:
	case SAMPLERSTATE_TADDRESSV:
		{
			if( State==SAMPLERSTATE_TADDRESSU ) { t = D3DSAMP_ADDRESSU; }
			ef( State==SAMPLERSTATE_TADDRESSV ) { t = D3DSAMP_ADDRESSV; }

			switch( Value )
			{
			case TEXTUREADDRESS_WRAP:	{ v = D3DTADDRESS_WRAP; }break;
			case TEXTUREADDRESS_MIRROR:	{ v = D3DTADDRESS_MIRROR; }break;
			case TEXTUREADDRESS_CLAMP:	{ v = D3DTADDRESS_CLAMP; }break;
			}
			
		}break;

	}
	const HRESULT ret = m_pDevice->SetSamplerState(Stage, t, v );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetSamplerState()")); }
*/
}


}

