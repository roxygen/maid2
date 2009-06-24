﻿#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

//  ここは各シェーダーステージのset,getを書く

#include"vertexd3d09.h"
#include"indexd3d09.h"
#include"constantd3d09.h"
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"depthstencilstated3d09.h"
#include"samplerstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::SetIndex( const SPBUFFER& pBuffer, size_t Offset )
{
  IndexD3D09* p = static_cast<IndexD3D09*>(pBuffer.get());

  const HRESULT ret = m_Device.GetDevice()->SetIndices( p->pObject.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetIndices()")); return; }

  m_IndexOffset = Offset;
}

void DrawCommandExecuteD3D09::SetVertex( int Slot, const SPBUFFER& pVertex, int Offset, int Stride )
{
  VertexD3D09* p = static_cast<VertexD3D09*>(pVertex.get());

  if( Slot==0 )
  { //  基本的に設定される頂点の数は同じなはずなので
    //  設定頻度のたかいところで記録しておく
    m_VertexMax = (p->GetParam().Length-Offset) / Stride;
  }

  const HRESULT ret = m_Device.GetDevice()->SetStreamSource( Slot, p->pBuffer.get(), Offset, Stride );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertex()")); return; }
}

void DrawCommandExecuteD3D09::SetInputLayout( const SPINPUTLAYOUT& pLayout )
{
  InputLayoutD3D09* p = static_cast<InputLayoutD3D09*>(pLayout.get());

  const HRESULT ret = m_Device.GetDevice()->SetVertexDeclaration( p->pDecl.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexDeclaration()")); return; }
}

void DrawCommandExecuteD3D09::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  D3DPRIMITIVETYPE t = D3DPT_POINTLIST;
  switch( Topology )
  {
  case PRIMITIVE_TOPOLOGY_POINTLIST:    { t = D3DPT_POINTLIST; }break;
  case PRIMITIVE_TOPOLOGY_LINELIST:     { t = D3DPT_LINELIST ; }break;
  case PRIMITIVE_TOPOLOGY_LINESTRIP:    { t = D3DPT_LINESTRIP; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLELIST: { t = D3DPT_TRIANGLELIST; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:{ t = D3DPT_TRIANGLESTRIP; }break;
  }

  m_PrimitiveType = t;
}






void DrawCommandExecuteD3D09::VSSetMaterial( int stage, const SPMATERIAL& pBuffer )
{
  MAID_ASSERT( true, "実装不可能" );
}
void DrawCommandExecuteD3D09::VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  MAID_ASSERT( true, "実装不可能" );
}


void DrawCommandExecuteD3D09::VSSetShader( const SPVERTEXSHADER& Shader )
{
  VertexShaderD3D09* p = static_cast<VertexShaderD3D09*>(Shader.get());

  const D3DDEVICE_CREATION_PARAMETERS& creat = m_Device.GetD3DDEVICE_CREATION_PARAMETERS();
  const D3DCAPS9& caps = m_Device.GetD3DCAPS9();

  //  VertexShader がハードで実行できるか調べる
  if( IsFlag(creat.BehaviorFlags,D3DCREATE_MIXED_VERTEXPROCESSING) )
  {
    BOOL IsSoft = TRUE;
    if( caps.VertexShaderVersion < p->Version ) { IsSoft = TRUE; }
    else { IsSoft = FALSE; }

    const HRESULT ret = m_Device.GetDevice()->SetSoftwareVertexProcessing( IsSoft );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetSoftwareVertexProcessing()")); return; }
  }

  const HRESULT ret = m_Device.GetDevice()->SetVertexShader( p->pShader.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexShader()")); return; }
}


void DrawCommandExecuteD3D09::VSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  MAID_ASSERT( slot!=0, "スロットは０しか指定できません" );

  ConstantD3D09* p = static_cast<ConstantD3D09*>(pBuffer.get());

  const float* data = (float*)&(p->Buffer[0]);
  const int count = p->Buffer.size() / (sizeof(float)*4);

  const HRESULT  ret =  m_Device.GetDevice()->SetVertexShaderConstantF( 0, data, count );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexShaderConstantF()")); return; }
}









void DrawCommandExecuteD3D09::PSSetMaterial( int stage, const SPMATERIAL& pMaterial )
{
  MAID_ASSERT( pMaterial.get()==NULL, "pMaterialが確保されていません" );

  Texture2DD3D09* pTexture = static_cast<Texture2DD3D09*>(pMaterial->GetResource().get());

  MAID_ASSERT( pTexture->pTexture.get()==NULL, "テクスチャが確保されていません" );

  const HRESULT ret = m_Device.GetDevice()->SetTexture( stage, pTexture->pTexture.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetTexture()")); return; }
}

void DrawCommandExecuteD3D09::PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  SamplerStateD3D09* p = static_cast<SamplerStateD3D09*>(Sampler.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->State.size(); ++i )
  {
    const SAMPLERSTATE& s = p->State[i];

    const HRESULT ret = pDev->SetSamplerState( stage, s.Type, s.Value );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetSamplerState() ")<<s.Type <<":"<< s.Value); return; }
  }
}

void DrawCommandExecuteD3D09::PSSetShader( const SPPIXELSHADER& Shader )
{
  PixelShaderD3D09* p = static_cast<PixelShaderD3D09*>(Shader.get());

  p->Setup( m_Device.GetDevice() );
}

void DrawCommandExecuteD3D09::PSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  MAID_ASSERT( slot!=0, "スロットは０しか指定できません" );

  ConstantD3D09* p = static_cast<ConstantD3D09*>(pBuffer.get());

  const float* data = (float*)&(p->Buffer[0]);
  const int count = p->Buffer.size() / (sizeof(float)*4);

  const HRESULT  ret =  m_Device.GetDevice()->SetPixelShaderConstantF( 0, data, count );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetPixelShaderConstantF()")); return; }
}





void DrawCommandExecuteD3D09::SetViewPort( const VIEWPORT& port )
{
  D3DVIEWPORT9 vp;
  vp.X      = port.Screen.x;
  vp.Y      = port.Screen.y;
  vp.Width  = port.Screen.w;
  vp.Height = port.Screen.h;
  vp.MinZ   = port.MinZ;
  vp.MaxZ   = port.MaxZ;

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();

  const HRESULT ret = pDev->SetViewport( &vp );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetViewport() ")); return; }
}

void DrawCommandExecuteD3D09::SetRasterizerState( const SPRASTERIZERSTATE& State )
{
  RasterizerStateD3D09* p = static_cast<RasterizerStateD3D09*>(State.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    const HRESULT ret = pDev->SetRenderState( s.Type, s.Value );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetRenderState() ")<<s.Type <<":"<< s.Value); return; }
  }
}

void DrawCommandExecuteD3D09::SetDepthStencilState( const SPDEPTHSTENCILSTATE& State )
{
  DepthStencilStateD3D09* p = static_cast<DepthStencilStateD3D09*>(State.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    const HRESULT ret = pDev->SetRenderState( s.Type, s.Value );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetRenderState() ")<<s.Type <<":"<< s.Value); return; }
  }
}

void DrawCommandExecuteD3D09::SetBlendState( const SPBLENDSTATE& State )
{
  BlendStateD3D09* p = static_cast<BlendStateD3D09*>(State.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    const HRESULT ret = pDev->SetRenderState( s.Type, s.Value );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetRenderState() ")<<s.Type <<":"<< s.Value); return; }
  }
}

void DrawCommandExecuteD3D09::SetRenderTarget( int no, const SPRENDERTARGET& pColor, const SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( !(no<8), "値が大きすぎます "<< no );

  m_NowRenderTarget[no] = pColor;
  m_NowDepthStencil[no] = pDepth;

  IDirect3DSurface9* c = NULL;
  IDirect3DSurface9* d = NULL;

  if( pColor.get()!=NULL ){ c = static_cast<Texture2DD3D09*>(pColor->GetResource().get())->pSurface.get(); }
  if( pDepth.get()!=NULL ){ d = static_cast<Texture2DD3D09*>(pDepth->GetResource().get())->pSurface.get(); }

#pragma  COMPILERMSG( "Draw()時に切り替えるようにしたほうがいいかもなー" )

  {
    const HRESULT ret = GetDevice()->SetRenderTarget( no, c );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetRenderTarget() ")); return; }
  }
  {
    const HRESULT ret = GetDevice()->SetDepthStencilSurface( d );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetDepthStencilSurface() ")); return; }
  }
}

VIEWPORT DrawCommandExecuteD3D09::GetViewport()
{
  D3DVIEWPORT9 v;
  {
    const HRESULT ret = GetDevice()->GetViewport( &v );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetViewport() ")); goto MAID_ERROR; }
  }

  return VIEWPORT( RECT2DI(v.X,v.Y,v.Width,v.Height), v.MinZ, v.MaxZ );

MAID_ERROR:
  return VIEWPORT();
}

void DrawCommandExecuteD3D09::GetRenderTarget( int no, SPRENDERTARGET& pColor, SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  pColor = m_NowRenderTarget[no];
  pDepth = m_NowDepthStencil[no];
}


}}

