#include"deviced3d09.h"
#include"../../../../auxiliary/debug/trace.h"

#include"bufferd3d09.h"
#include"texture2dd3d09.h"
#include"constantd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"samplerstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"


namespace Maid { namespace Graphics {

/*
hVertex DeviceD3D09::CreateVertex( const CREATEVERTEXPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT  length = param.Length;
  const DWORD usage = 0;
  const DWORD fvf = 0;
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
*/

IBuffer* DeviceD3D09::CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{


MAID_ERROR:
  return NULL;
}

ITexture2D* DeviceD3D09::CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )
{
  return NULL;
}


IRenderTarget* DeviceD3D09::CreateRenderTarget( const IResource* resource, const CREATERENDERTARGETPARAM& param )
{
  return NULL;
}

IDepthStencil* DeviceD3D09::CreateDepthStencil( const IResource* resource, const CREATEDEPTHSTENCILPARAM& param )
{
  return NULL;
}

IMaterial* DeviceD3D09::CreateMaterial( const IResource* resource, const CREATESHADERMATERIALPARAM& param )
{
  return NULL;
}

IVertexShader* DeviceD3D09::CreateVertexShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  IDirect3DVertexShader9* pShader=NULL;

  const DWORD ver = D3DXGetShaderVersion( (DWORD*)pData );


  const HRESULT ret = pDev->CreateVertexShader( (DWORD*)pData, &pShader );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexShader()")); goto MAID_LABEL; }

  return new VertexShaderD3D09(pShader,ver);

MAID_LABEL:
  return NULL;
}

IPixelShader* DeviceD3D09::CreatePixelShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  PixelShaderD3D09* p = NULL;

  if( Length==4 )
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

  return p;

MAID_LABEL:
  return NULL;
}




ISamplerState* DeviceD3D09::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  return NULL;
}

IRasterizerState* DeviceD3D09::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  std::vector<RENDERSTATE> v;

  {
    RENDERSTATE s;
    s.Type = D3DRS_FILLMODE;
    switch( Option.Fill )
    {
    case RASTERIZERSTATEPARAM::FILL_WIREFRAME: { s.Value = D3DFILL_WIREFRAME; }break;
    case RASTERIZERSTATEPARAM::FILL_SOLID: { s.Value = D3DFILL_SOLID; }break;
    }

    v.push_back( s );
  }

  {
    RENDERSTATE s;
    s.Type = D3DRS_CULLMODE;
    switch( Option.Culling )
    {
    case RASTERIZERSTATEPARAM::CULLING_NONE:  { s.Value = D3DCULL_NONE; }break;
    case RASTERIZERSTATEPARAM::CULLING_LEFT:  { s.Value = D3DCULL_CCW; }break;
    case RASTERIZERSTATEPARAM::CULLING_RIGHT: { s.Value = D3DCULL_CW ; }break;
    }

    v.push_back( s );
  }
  {
    RENDERSTATE s;
    s.Type =  D3DRS_MULTISAMPLEANTIALIAS;

    if( Option.MultiSample ) { s.Value = TRUE; }
    else { s.Value = FALSE; }
    v.push_back( s );
  }

  return new RasterizerStateD3D09(v);
}


IBlendState* DeviceD3D09::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  //  D3D9だと ８つあるRenderTargetは全部同じ設定でしかレンダリングできない。
  //  このクラス内でごにょごにょすればうまく出来そうだけど、めんどいので
  //  Target[0]以外は破棄してしまう。

  const BLENDSTATEPARAM::TARGET& t = Option.Target[0];
  std::vector<RENDERSTATE> v;

  v.push_back( RENDERSTATE(D3DRS_ALPHABLENDENABLE,BOOLtobool(t.Enable)) );

  v.push_back( RENDERSTATE(D3DRS_SRCBLEND, TARGETBLENDtoD3DBLEND(t.ColorSrc)) );
  v.push_back( RENDERSTATE(D3DRS_DESTBLEND,TARGETBLENDtoD3DBLEND(t.ColorDst)) );
  v.push_back( RENDERSTATE(D3DRS_BLENDOP,  TARGETOPERATIONtoD3DBLENDOP(t.ColorOp)) );

  v.push_back( RENDERSTATE(D3DRS_SRCBLENDALPHA, TARGETBLENDtoD3DBLEND(t.AlphaSrc)) );
  v.push_back( RENDERSTATE(D3DRS_DESTBLENDALPHA,TARGETBLENDtoD3DBLEND(t.AlphaDst)) );
  v.push_back( RENDERSTATE(D3DRS_BLENDOPALPHA,  TARGETOPERATIONtoD3DBLENDOP(t.AlphaOp)) );

  v.push_back( RENDERSTATE(D3DRS_COLORWRITEENABLE,  t.WriteMask ) );

  return new BlendStateD3D09( v );
}

IInputLayout* DeviceD3D09::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  std::vector<D3DVERTEXELEMENT9>  elem( Count );

  for( int i=0; i<Count; ++i )
  {
    const INPUT_ELEMENT& src = Element[i];
    D3DVERTEXELEMENT9&   dst = elem[i];

    dst.Stream = src.SlotNo;
    dst.Offset = src.Offset;
    dst.Type = INPUT_ELEMENTTYPEtoD3DDECLTYPE(src.Type);
    dst.Method = INPUT_ELEMENTMETHODtoD3DDECLMETHOD(src.Method);

    dst.Usage = INPUT_ELEMENTSemanticNametoD3DDECLUSAGE(src.SemanticName);
    dst.UsageIndex = src.SemanticIndex;
  }

  {
    const D3DVERTEXELEMENT9 end = D3DDECL_END();
    elem.push_back( end );
  }

  IDirect3DVertexDeclaration9* p=NULL;

  const HRESULT ret = m_pDevice->CreateVertexDeclaration( &(elem[0]), &p );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexDeclaration()")); goto MAID_ERROR; }


MAID_ERROR:
  return NULL;
}

bool DeviceD3D09::CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )
{
  const std::string SrcData = String::ConvertMAIDtoSJIS(Source);

  //  固定機能を使う場合は言語サイズが4バイト以下かつ
  //  全部数字であることが条件なのでそれを元に作る
  //  いろいろしらべたけど、シェーダーバイナリの最小は32byteっぽいので、これなら自由にやれるはず
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


