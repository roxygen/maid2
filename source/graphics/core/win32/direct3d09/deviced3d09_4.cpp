#include"deviced3d09.h"

#include"../defaultshader.h"


namespace Maid { namespace Graphics {

void DeviceD3D09::CreateDefaultPixelShader( int no, std::vector<unt08>& Binary )
{
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::string str;
  switch( no )
  {
  case 200: { str = s_SHADERCODE0200; }break;
  case 210: { str = s_SHADERCODE0210; }break;
  case 211: { str = s_SHADERCODE0211; }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

  ret = m_ShaderCompilerDefault(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    "main",
    "ps_2_0",
    0,
    &pShader,
    &pErrorMsgs,
    NULL );


  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい" );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();
}


}}


