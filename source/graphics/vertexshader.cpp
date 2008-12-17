#include"vertexshader.h"


namespace Maid
{

  StorageObjectTemplate<String,VERTEXSHADEROPTION>::STORAGEMAP  StorageObjectTemplate<String,VERTEXSHADEROPTION>::s_Storage;

  class VertexShaderCompiler : public IStorageFunction
  {
  public:

    //  入力
    String Text;


    //  出力
    LPD3DXBUFFER  pShader;
    LPD3DXBUFFER  pErrorMsgs;

    void Execute()
    {
      std::string SrcData = String::ConvertMAIDtoSJIS(Text);

      D3DXAssembleShader( SrcData.c_str(), SrcData.length(), 
          NULL,NULL, 0,
          &pShader,&pErrorMsgs );
    }

  };

  VertexShader::VertexShader()
  {

  }

	void VertexShader::DeleteFirst( const String& id  )
  {
    const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    pDevice->DeleteObject( IVertexShader::Get() );
  }

	void VertexShader::LoadEndFirst( const IStorageFunction* param, VERTEXSHADEROPTION& option  )
  {
    const VertexShaderCompiler* p = static_cast<const VertexShaderCompiler*>(param);
    LPVOID pBuff = p->pShader->GetBufferPointer();
    DWORD Len = p->pShader->GetBufferSize();

    const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    option.hObject = pDevice->CreateVertexShader( pBuff, Len );

    IVertexShader::Set( option.hObject );
  }

	void VertexShader::LoadEnd( const VERTEXSHADEROPTION& option  )
  {
    IVertexShader::Set( option.hObject );
  }

  IStorageFunction* VertexShader::CreateFunction( const String& id )
  {
    VertexShaderCompiler* pRet = new VertexShaderCompiler;
    pRet->Text = id;
    return pRet;
  }
	
}

