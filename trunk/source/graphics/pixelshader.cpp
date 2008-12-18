#include"pixelshader.h"


namespace Maid
{

  PixelShader::STORAGEMAP  PixelShader::s_Storage;

  class PixelShaderCompiler : public IStorageFunction
  {
  public:
    //  入力
    String Text;
    Graphics::SPDEVICE pDevice;

    //  出力 
    Graphics::hPixelShader hObject;

    void Execute()
    {
      std::vector<unt08> Binary;
      String ErrorMSG;

      const bool IsSuccess = pDevice->CompileShaderLanguage(Text,Binary,ErrorMSG);

      if( !IsSuccess )
      {
        OnFaile(ErrorMSG);
        return ;
      }
      hObject = pDevice->CreatePixelShader(&(Binary[0]),Binary.size());
    }

  };

	void PixelShaderBase::DeleteFirst( const String& id  )
  {
    if( IPixelShader::Get().ID==Graphics::hObject::ID_NONE ) { return ; }

    const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    pDevice->DeleteObject( IPixelShader::Get() );
  }

	void PixelShaderBase::LoadEndFirst( const IStorageFunction* param, OPTION& option  )
  {
    const PixelShaderCompiler* p = static_cast<const PixelShaderCompiler*>(param);

    if( p->IsSucess() )
    {
      option.hObject = Graphics::hPixelShader();
    }else
    {
      option.hObject = p->hObject;
    }
    IPixelShader::Set( option.hObject );
  }

	void PixelShaderBase::LoadEnd( const OPTION& option  )
  {
    IPixelShader::Set( option.hObject );
  }

  IStorageFunction* PixelShaderBase::CreateFunction( const String& id )
  {
    PixelShaderCompiler* pRet = new PixelShaderCompiler;
    pRet->Text  = id;
    pRet->pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    return pRet;
  }

  String PixelShaderBase::ConvertID( const String& id )
  {
    return id; 
  }

}

