#include"shadercompiler.h"


namespace Maid
{
  /*!
      @class  ShaderCompiler shadercompiler.h
      @brief  各種シェーダーを別スレッドでコンパイル、キャッシュしてくれるクラス
  \n          コンパイルはデバイスの仕事なので、このクラスはデバイスが違っても変更しなくてすむ
  */

  ShaderCompiler::CACHE::INFOMAP  ShaderCompiler::CACHE::s_InfoMap;
  ThreadMutex  ShaderCompiler::CACHE::s_Mutex;


  void ShaderCompiler::Compile( const String& ShaderLanguage, const Graphics::SPDEVICE& pDevice )
  {
    m_Cache.Start( KEEPOUT::ShaderInput(ShaderLanguage,pDevice) );
  }

  bool ShaderCompiler::IsCompiling()const
  {
    return m_Cache.IsExecuting();
  }

  const std::vector<unt08>& ShaderCompiler::GetByteCode() const
  {
    return m_Cache.GetOutput().ByteCode;
  }

  const String& ShaderCompiler::GetShaderLanguage() const
  {
    return m_Cache.GetInput().ShaderLanguage;
  }


}

