﻿#include"pixelshader.h"


namespace Maid
{
  PixelShader::CACHE::INFOMAP  PixelShader::CACHE::s_InfoMap;
  ThreadMutex  PixelShader::CACHE::s_Mutex;

  void PixelShader::Create( const String& ShaderCode )
  {
    IPixelShader::Clear();
    if( ShaderCode.empty() ) { return ; }
    m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_PIXELSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
  }

  bool PixelShader::IsCompiling()const
  {
    //  ポインタが存在してるならすでに作ってる
    if( IPixelShader::Get().get()!=NULL ) { return false; }

    //  処理が終わるまでは忙しい
    if( m_Compiler.IsCompiling() ) { return true; }

    //  ここまできたらコンパイルが終わっている
    const std::vector<unt08>& code = m_Compiler.GetByteCode();
    if( code.empty() ) { return false; }

    //  次はシェーダー作成処理
    if( m_Cache.IsEmpty() )
    { //  m_Cache が動いてない==まだ作ってないなので、作成する
      const String& key = m_Compiler.GetShaderLanguage();
      const std::vector<unt08>& byte = m_Compiler.GetByteCode();
      const_cast<PixelShader*>(this)->m_Cache.Start( KEEPOUT::psInput(key, byte, GlobalPointer<GraphicsCore>::Get()->GetDevice() ) );
    }

    if( m_Cache.IsExecuting() ) { return true; }

    const_cast<PixelShader*>(this)->IPixelShader::Set( m_Cache.GetOutput().pShader );
    return false;
  }
}

