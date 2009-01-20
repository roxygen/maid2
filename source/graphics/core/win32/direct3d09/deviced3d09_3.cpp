#include"deviced3d09.h"



namespace Maid { namespace Graphics {


void DeviceD3D09::CreateDefaultCertexShader( int no, std::vector<unt08>& Binary )
{
  std::string code;

  switch( no )
  {
  case 100:
    { //  頂点と色を流し込むシェーダー
      code = "vs.1.1\n"
             "dcl_position v0\n"
             "dcl_color v1\n"
             "mov oPos, v0\n"
             "mov oD0, v1\n"
             ;
    }break;

  case 101:
    { //  頂点と色とUVを流し込むシェーダー
      code =  "vs.1.1\n"
              "dcl_position v0\n"
              "dcl_color    v1\n"
              "dcl_texcoord v2\n"
              "mov oPos, v0\n"
              "mov oD0,  v1\n"
			        "mov oT0,  v2\n"
             ;
    }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  ret = D3DXAssembleShader( code.c_str(), code.length(), 
      NULL,NULL, 0,
      &pShader,&pErrorMsgs );

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
//    ErrorMessage    = String::ConvertSJIStoMAID(str);
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


