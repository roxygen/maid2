#include"deviced3d10_0.h"


namespace Maid { namespace Graphics {

static const char* pCode0100 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;



static const char* pCode0101 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n   Out.TexCoords = In.TexCoords;"
"\n"
"\n   return Out;"
"\n }"
;

void DeviceD3D10_0::CreateDefaultVertexShader( int no, std::vector<unt08>& Binary )
{
  ID3D10Blob*  pShader=NULL;
  ID3D10Blob*  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::string str;
  switch( no )
  {
  case 100:
    { //  頂点と色を流し込むシェーダー
      str = pCode0100;
    }break;

  case 101:
    { //  頂点と色とUVを流し込むシェーダー
      str = pCode0101;
    }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }
  ret = D3DX10CompileFromMemory(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    NULL,
    "main",
    "vs_4_0",
    0,
    0,
    NULL,
    &pShader,
    &pErrorMsgs,
    NULL
    );

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい " << str );
    return ;
  }
/*
  {
    ID3D10Blob* pDst=NULL;

    D3DXDisassembleShader(
      pShader->GetBufferPointer(),
      pShader->GetBufferSize(),
      FALSE,
      NULL,
      &pDst );

    std::string text = (char*)pDst->GetBufferPointer();
    pDst->Release();

    MAID_WARNING( text );

  }
*/

  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();

}

}}


