#include"deviced3d10_0.h"

/*
#include"vertexd3d09.h"
#include"indexd3d09.h"
#include"constantd3d09.h"
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"samplerstated3d09.h"
#include"depthstencilstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"
*/

#include"../../../../auxiliary/debug/trace.h"


namespace Maid { namespace Graphics {


SPBUFFER DeviceD3D10_0::CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  return SPBUFFER();
}


SPTEXTURE2D DeviceD3D10_0::CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )
{
  return SPTEXTURE2D();
}

SPRENDERTARGET DeviceD3D10_0::CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM& param )
{
  return SPRENDERTARGET();
}

SPDEPTHSTENCIL DeviceD3D10_0::CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM& param )
{
 return SPDEPTHSTENCIL();
}

SPMATERIAL DeviceD3D10_0::CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM& param )
{
  return SPMATERIAL();
}

SPVERTEXSHADER DeviceD3D10_0::CreateVertexShader( const void* pData, size_t Length )
{
  return SPVERTEXSHADER();
}

SPPIXELSHADER DeviceD3D10_0::CreatePixelShader( const void* pData, size_t Length )
{
  return SPPIXELSHADER();
}


SPSAMPLERSTATE DeviceD3D10_0::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  return SPSAMPLERSTATE();
}

SPRASTERIZERSTATE DeviceD3D10_0::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  return SPRASTERIZERSTATE();
}

SPDEPTHSTENCILSTATE DeviceD3D10_0::CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option )
{
  return SPDEPTHSTENCILSTATE();
}


SPBLENDSTATE DeviceD3D10_0::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  return SPBLENDSTATE();
}

SPINPUTLAYOUT DeviceD3D10_0::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  return SPINPUTLAYOUT();
}

bool DeviceD3D10_0::CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )
{
#if 0
  const std::string SrcData = String::ConvertMAIDtoSJIS(Source);

  //  固定機能を使う場合は言語サイズが4バイト以下かつ
  //  全部数字であることが条件なのでそれを元に作る
  //  いろいろしらべたけど、シェーダーバイナリの最小は32byteっぽいので、これなら自由にやれるはず
  if( SrcData.length()<=4 )
  {
    const unt32 no = atoi( SrcData.c_str() );

    if( 100<=no && no<=199 )
    {
      //  vertex shader はここで作る
      CreateDefaultVertexShader( no, Binary );
      return true;
    }
    ef( 200<=no && no<=299 ) 
    {
      //  PixelShader の場合は CreatePixelShader で作ってもらう
      Binary.resize( sizeof(no) );
      memcpy( &(Binary[0]), &no, sizeof(no)  );
      return true;
    }
    else { MAID_ASSERT( true, "このデバイスでは対応していません" ); }
  }

  //  そうでない場合は普通にやる
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  const char* sl_ver[] =
  {
    "vs_1_1", "vs_2_0", "vs_2_sw",
    "ps_1_1", "ps_1_2", "ps_1_3", "ps_1_4", "ps_2_0" "ps_2_sw",
  };
    #pragma COMPILERMSG("TODO:シェーダーの先頭にバージョンとか追記してもらうほうがいいかもしれない")	

  for( int i=0; i<NUMELEMENTS(sl_ver); ++i )
  {
    ret = D3DXCompileShader(
      SrcData.c_str(),
      SrcData.length(),
      NULL,
      NULL,
      "main",
      sl_ver[i],
      0,
      &pShader,
      &pErrorMsgs,
      NULL );

    if( FAILED(ret) )
    {
      std::string str = (char*)pErrorMsgs->GetBufferPointer();
      pErrorMsgs->Release();
    }
    if( SUCCEEDED(ret) ) { break; }
  }

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
    ErrorMessage    = String::ConvertSJIStoMAID(str);
    pErrorMsgs->Release();
    goto MAID_ERROR;  
  }

/*  //  逆アセ用コメント
  {
    LPD3DXBUFFER pDis;

    HRESULT a = D3DXDisassembleShader(
	   (DWORD*)pShader->GetBufferPointer(),
	  FALSE,
	  NULL,
	  &pDis
	);
  const char* text = (char*)pDis->GetBufferPointer();

  MAID_WARNING( text );
  pDis->Release();
  }
//*/
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );

MAID_ERROR:
   if( pShader!=NULL ) { pShader->Release(); }
   if( pErrorMsgs!=NULL ) { pErrorMsgs->Release(); }
   return SUCCEEDED(ret);

#endif
   return true;
}



}}


