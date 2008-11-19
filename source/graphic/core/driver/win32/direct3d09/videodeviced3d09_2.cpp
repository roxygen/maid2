#include"videodeviced3d09.h"


#include"../../../../../auxiliary/exception.h"
#include"../../../../../auxiliary/debug/trace.h"
#include"../../../../../auxiliary/debug/warning.h"
#include"../../../../../auxiliary/string.h"


#include"SurfaceBufferD3D09.h"

namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateRenderTargetSurface() 参照
SurfaceBuffer* VideoDeviceD3D09::CreateRenderTargetSurface( const SIZE2DI& Size, PIXELFORMAT Format, SAMPLETYPE type, int Quality )
{
  IDirect3DSurface9* pSurface;

  const HRESULT ret = m_pDevice->CreateRenderTarget( 
    Size.w, Size.h, PIXELFORMATtoD3DFORMAT(Format),
    SAMPLETYPEtoD3DMULTISAMPLE_TYPE(type), Quality,
    FALSE,
    &pSurface, NULL
    );

  if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateRenderTarget()")); }

  return new SurfaceBufferD3D09(pSurface);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateDepthStencileSurface() 参照
SurfaceBuffer* VideoDeviceD3D09::CreateDepthStencileSurface( const SIZE2DI& Size, PIXELFORMAT Format, SAMPLETYPE type, int Quality )
{
  IDirect3DSurface9* pSurface;

  const HRESULT ret = m_pDevice->CreateDepthStencilSurface( 
    Size.w, Size.h, PIXELFORMATtoD3DFORMAT(Format),
    SAMPLETYPEtoD3DMULTISAMPLE_TYPE(type), Quality,
    FALSE,
    &pSurface, NULL
    );

  if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateRenderTarget()")); }

  return new SurfaceBufferD3D09(pSurface);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateTextureBuffer() 参照
TextureBuffer* VideoDeviceD3D09::CreateTextureBuffer( const TextureBufferMemory& buffer )
{

  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateRenderTargetTextureBuffer() 参照
TextureBuffer* VideoDeviceD3D09::CreateRenderTargetTextureBuffer( const SIZE2DI& Size, PIXELFORMAT Format )
{
  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateIndexBuffer() 参照
IndexBuffer* VideoDeviceD3D09::CreateIndexBuffer( const std::vector<unt08>& data, int Format )
{
  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateVertexBuffer() 参照
VertexBuffer* VideoDeviceD3D09::CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format )
{
  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateVertexShader() 参照
VertexShaderBuffer* VideoDeviceD3D09::CreateVertexShader( const std::vector<unt08>& Code )
{
  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreatePixelShader() 参照
PixelShaderBuffer* VideoDeviceD3D09::CreatePixelShader( const std::vector<unt08>& Code )
{
  return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::CreateVertexDeclarationBuffer() 参照
VertexDeclarationBuffer* VideoDeviceD3D09::CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer )
{
  return NULL;
}


}

