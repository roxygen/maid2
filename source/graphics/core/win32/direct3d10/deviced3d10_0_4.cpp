#include"deviced3d10_0.h"

#include"../defaultshader.h"

namespace Maid { namespace Graphics {

/*
static const char* pCode0200 = 
"\n struct PS_INPUT"
"\n {"
"\n   float4 Position   : SV_Position; "
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n float4 main( PS_INPUT In ) : SV_Target"
"\n {"
"\n   return In.Diffuse;"
"\n }"
;


static const char* pCode0210 = 
"\n Texture2D<float4> tx0;"
"\n sampler sampler0;"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n   float4 Position   : SV_Position; "
"\n    float2 ImageUV : TEXCOORD0; "
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n float4 main( PS_INPUT In ) : SV_Target"
"\n {"
"\n   float4 ret = tx0.Sample(sampler0,In.ImageUV);"
"\n   ret.x = 0;"
"\n   ret.y = 0;"
"\n   ret.z = 1;"
"\n   ret.w = 0.5;"
"\n   return ret;"
"\n }"
;

static const char* pCode0211 = 
"\n struct PS_INPUT"
"\n {"
"\n   float4 Position   : SV_Position; "
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n float4 main( PS_INPUT In ) : SV_Target"
"\n {"
"\n   return In.Diffuse;"
"\n }"
;
*/

void DeviceD3D10_0::CreateDefaultPixelShader( int no, std::vector<unt08>& Binary )
{
  ID3D10Blob*  pShader=NULL;
  ID3D10Blob*  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::string str;
  switch( no )
  {
  case 200: { str = s_SHADERCODE0200; }break;
  case 210: { str = s_SHADERCODE0210; }break;
  case 211: { str = s_SHADERCODE0211; }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }
  ret = D3DX10CompileFromMemory(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    NULL,
    "main",
    "ps_4_0",
    0,
    0,
    NULL,
    &pShader,
    &pErrorMsgs,
    NULL
    );

  if( FAILED(ret) ) 
  {
    std::string text = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい" << text << str );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();

}

}}


