#include"deviced3d09.h"
#include"../../../../auxiliary/debug/trace.h"
#include"defaultpixelshader.h"

namespace Maid { namespace Graphics {

hVertex DeviceD3D09::CreateVertex( const CREATEVERTEXPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT  length = param.Length;
  const DWORD usage = 0;
  const DWORD fvf = VERTEXFORMAT2FVF( param.Format );
  const D3DPOOL pool = D3DPOOL_DEFAULT;

  IDirect3DVertexBuffer9* p=NULL;
  {
    const HRESULT ret = pDev->CreateVertexBuffer( length, usage, fvf, pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexBuffer()")); goto MAID_LABEL; }
  }

  {
    VOID* pDst = NULL;

    {
      const HRESULT ret = p->Lock( 0, 0, &pDst, D3DLOCK_DISCARD );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Lock()")); goto MAID_LABEL; }
    }

    ::memcpy( pDst, param.pData, param.Length );

    {
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Unlock()")); goto MAID_LABEL; }
    }
  }

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_RESOURCEVERTEX) ) );

  return hVertex((hObject::INSTANCEID)p);

MAID_LABEL:
  if( p!=NULL ) { p->Release(); }
  return hVertex(hObject::ID_NONE);
}


hIndex DeviceD3D09::CreateIndex( const CREATEREINDEXPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT len = param.Length;
	const DWORD use = 0;
  const D3DFORMAT fmt = param.Format==16? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	const D3DPOOL pool = D3DPOOL_MANAGED;

  IDirect3DIndexBuffer9* p=NULL;
  {
    const HRESULT ret = pDev->CreateIndexBuffer( len, use, fmt, pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateIndexBuffer()")); goto MAID_ERROR; }
  }

  {
    VOID* pDst = NULL;

    {
      const HRESULT ret = p->Lock( 0, 0, &pDst, D3DLOCK_DISCARD );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DIndexBuffer9::Lock()")); goto MAID_ERROR; }
    }

    ::memcpy( pDst, param.pData, param.Length );

    {
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DIndexBuffer9::Unlock()")); goto MAID_ERROR; }
    }
  }

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_RESOURCEINDEX) ) );

  return hIndex((hObject::INSTANCEID)p);


MAID_ERROR:
  if( p!=NULL ) { p->Release(); }
  return hIndex(hObject::ID_NONE);
}

hTexture2D DeviceD3D09::CreateTexture2D( const CREATERETEXTURE2DPARAM& param )
{
  return hTexture2D(hObject::ID_NONE);
}

hConstant DeviceD3D09::CreateConstant( const CREATERECONSTANTPARAM& param )
{
  return hConstant(hObject::ID_NONE);
}



hRenderTarget DeviceD3D09::CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )
{
  return hRenderTarget(hObject::ID_NONE);
}

hDepthStencil DeviceD3D09::CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )
{
  return hDepthStencil(hObject::ID_NONE);
}
hMaterial DeviceD3D09::CreateShaderMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )
{
  return hMaterial(hObject::ID_NONE);
}

hVertexShader DeviceD3D09::CreateVertexShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  IDirect3DVertexShader9* pShader=NULL;

  const DWORD ver = D3DXGetShaderVersion( (DWORD*)pData );


  const HRESULT ret = pDev->CreateVertexShader( (DWORD*)pData, &pShader );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexShader()")); goto MAID_LABEL; }

  VertexShaderD3D09* p = new VertexShaderD3D09(pShader,ver);

  return hVertexShader((hObject::INSTANCEID)p);

MAID_LABEL:

  return hVertexShader(hObject::ID_NONE);
}

hPixelShader DeviceD3D09::CreatePixelShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  PixelShaderD3D09* p = NULL;

  if( Length<=4 )
  {
    DWORD no = 0;
    memcpy( &no, pData, Length  );

    switch( no )
    {
    case 1: { p = new PS_001; }break;
    case 2: { p = new PS_002; }break;
    }
  }else
  {

    IDirect3DPixelShader9* pShader;

    const HRESULT ret = pDev->CreatePixelShader( (DWORD*)pData, &pShader );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreatePixelShader()")); goto MAID_LABEL; }

    p = new PS_SHADERCODE(pShader);

  }

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_PIXELSHADER) ) );

  return hPixelShader((hObject::INSTANCEID)p);

MAID_LABEL:
  return hPixelShader(hObject::ID_NONE);
}

hSamplerState DeviceD3D09::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  return hSamplerState(hObject::ID_NONE);
}

hRasterizerState DeviceD3D09::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  return hRasterizerState(hObject::ID_NONE);
}

hBlendState DeviceD3D09::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  return hBlendState(hObject::ID_NONE);
}

hInputLayout DeviceD3D09::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  return hInputLayout(hObject::ID_NONE);
}

bool DeviceD3D09::CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )
{
  const std::string SrcData = String::ConvertMAIDtoSJIS(Source);

  //  固定機能を使う場合は言語サイズが4バイト以下かつ
  //  全部数字であることが条件なのでそれを元に作る
  if( SrcData.length()<=4 )
  {
    const unt32 no = atoi( SrcData.c_str() );
    Binary.resize( sizeof(no) );
    memcpy( &(Binary[0]), &no, sizeof(no)  );
    return true;
  }



  //  そうでない場合は普通にやる
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  ret = D3DXAssembleShader( SrcData.c_str(), SrcData.length(), 
      NULL,NULL, 0,
      &pShader,&pErrorMsgs );

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
    ErrorMessage    = String::ConvertSJIStoMAID(str);
    goto MAID_ERROR;  
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );

MAID_ERROR:
   if( pShader!=NULL ) { pShader->Release(); }
   if( pErrorMsgs!=NULL ) { pErrorMsgs->Release(); }
   return SUCCEEDED(ret);
}


}}


