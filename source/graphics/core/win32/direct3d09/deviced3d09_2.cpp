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

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_VERTEXSHADER) ) );

  return hVertexShader((hObject::INSTANCEID)p);

MAID_LABEL:

  return hVertexShader(hObject::ID_NONE);
}

hPixelShader DeviceD3D09::CreatePixelShader( const void* pData, size_t Length )
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


  RasterizerStateD3D09* p = new RasterizerStateD3D09(v);

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_RASTERIZESTATE) ) );

  return hRasterizerState((hObject::INSTANCEID)p);
}

hBlendState DeviceD3D09::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  return hBlendState(hObject::ID_NONE);
}


D3DDECLTYPE INPUT_ELEMENTTYPEtoD3DDECLTYPE( IDevice::INPUT_ELEMENT::TYPE t )
{
  D3DDECLTYPE ret;

  switch( t )
  {
  case IDevice::INPUT_ELEMENT::TYPE_FLOAT1: { ret = D3DDECLTYPE_FLOAT1; }break;
  case IDevice::INPUT_ELEMENT::TYPE_FLOAT2: { ret = D3DDECLTYPE_FLOAT2; }break;
  case IDevice::INPUT_ELEMENT::TYPE_FLOAT3: { ret = D3DDECLTYPE_FLOAT3; }break;
  case IDevice::INPUT_ELEMENT::TYPE_FLOAT4: { ret = D3DDECLTYPE_FLOAT4; }break;
  case IDevice::INPUT_ELEMENT::TYPE_COLOR:  { ret = D3DDECLTYPE_D3DCOLOR; }break;
  }
  return ret;
}

D3DDECLMETHOD INPUT_ELEMENTMETHODtoD3DDECLMETHOD( IDevice::INPUT_ELEMENT::METHOD m )
{
  D3DDECLMETHOD ret = D3DDECLMETHOD_DEFAULT;

  switch( m )
  {
  case IDevice::INPUT_ELEMENT::METHOD_DEFAULT: { ret = D3DDECLMETHOD_DEFAULT; }break;
  case IDevice::INPUT_ELEMENT::METHOD_PARTIALU: { ret = D3DDECLMETHOD_PARTIALU; }break;
  case IDevice::INPUT_ELEMENT::METHOD_PARTIALV: { ret = D3DDECLMETHOD_PARTIALV; }break;
  case IDevice::INPUT_ELEMENT::METHOD_CROSSUV: { ret = D3DDECLMETHOD_UV; }break;
  case IDevice::INPUT_ELEMENT::METHOD_LOOKUP:  { ret = D3DDECLMETHOD_LOOKUP; }break;
  case IDevice::INPUT_ELEMENT::METHOD_LOOKUPPRESAMPLED:  { ret = D3DDECLMETHOD_LOOKUPPRESAMPLED; }break;
  }
  return ret;
}

D3DDECLUSAGE INPUT_ELEMENTSemanticNametoD3DDECLUSAGE( const char* p )
{
  D3DDECLUSAGE ret = D3DDECLUSAGE_POSITION ;
  std::string str = p;

  std::transform(str.begin(), str.end(), str.begin(), tolower );

  if( str=="position" ) { ret = D3DDECLUSAGE_POSITION; }
  ef( str=="blendweight" ) { ret = D3DDECLUSAGE_BLENDWEIGHT; }
  ef( str=="blendindices" ) { ret = D3DDECLUSAGE_BLENDINDICES; }
  ef( str=="normal" ) { ret = D3DDECLUSAGE_NORMAL; }
  ef( str=="psize" ) { ret = D3DDECLUSAGE_PSIZE; }
  ef( str=="texcoord" ) { ret = D3DDECLUSAGE_TEXCOORD; }
  ef( str=="tangent" ) { ret = D3DDECLUSAGE_TANGENT; }
  ef( str=="binormal" ) { ret = D3DDECLUSAGE_BINORMAL; }
  ef( str=="tessfactor" ) { ret = D3DDECLUSAGE_TESSFACTOR; }
  ef( str=="positiont" ) { ret = D3DDECLUSAGE_POSITIONT; }
  ef( str=="color" ) { ret = D3DDECLUSAGE_COLOR; }
  ef( str=="fog" ) { ret = D3DDECLUSAGE_FOG; }
  ef( str=="depth" ) { ret = D3DDECLUSAGE_DEPTH; }
  ef( str=="sample" ) { ret = D3DDECLUSAGE_SAMPLE; }

  return ret;
}


hInputLayout DeviceD3D09::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
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

  RegistObject( p, SPOBJECTDESC( new ObjectDesc(ObjectDesc::TYPE_INPUTLAYOUT) ) );

  return hInputLayout((hObject::INSTANCEID)p);

MAID_ERROR:
  return hInputLayout(hObject::ID_NONE);
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


