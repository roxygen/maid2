#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/vertexshader.h"
#include"../../source/graphics/vertex.h"
#include"../../source/graphics/inputlayout.h"
#include"../../source/graphics/customvertextemplate.h"
#include"../../source/graphics/rasterizerstate.h"
#include"../../source/graphics/blendstate.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
    //  ここでは引数で渡されたデバイスリストをユーザーに選択させる処理を書く
    //  ユーザーの選択結果は conf に設定するんだけど、設定しなくても自動的になんとかしてくれる

    //  ここでアプリケーションを終了したいときは return false です。
    return true;
  }

  void LocalInitialize( const BEGINPARAM& param )
  {
    //  LocalSelectDevice() で指定した設定で、各種デバイスデータが param に入ってます
    //  なので、もらったデータを元に、初期化する

    m_pCommand = param.DrawCommand[0];
    m_Render.Initialize( param.DrawCommand[0] );

    {
      const char* vs =
        "vs.1.1\n"
        "dcl_position v0\n"
        "dcl_color v1\n"
        "mov oPos, v0\n"
        "mov oD0, v1\n"
        ;

      m_Shader.Load( String::ConvertSJIStoMAID(vs) );
    }

    {
      CUSTOMVERTEX_COLOR v[] =
      {
        CUSTOMVERTEX_COLOR( POINT3DF(0, 0,0), COLOR_R32G32B32A32F(1,0,0,1) ),
        CUSTOMVERTEX_COLOR( POINT3DF(0,-1,0), COLOR_R32G32B32A32F(0,0,0,0.5f) ),
        CUSTOMVERTEX_COLOR( POINT3DF(1, 1,0), COLOR_R32G32B32A32F(0,0,1,1) ),
      };

      m_Vertex.Create( v, sizeof(v) );
    }

    {
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_Layout.Create( element, NUMELEMENTS(element), NULL, 0 );
    }

    {
       Graphics::RASTERIZERSTATEPARAM param;

       param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
       param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_NONE;
       param.MultiSample = false;

       m_RasterizerState.Create( param );
    }

    {
       Graphics::BLENDSTATEPARAM param;

       param.Target[0].Enable = true;
       param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
       param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA;
       m_BlendState.Create( param );
    }
  }

  void LocalUpdateFrame( float now, float delta )
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void LocalUpdateDraw( const IRenderTarget& target,float now, float delta )
  {
    //  定期的に描画するとこ
    //  描画先は target 

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0.5f,0,0) );

    if( m_Shader.IsLoading() ) { return ; }

    IVertex& vertex = m_Vertex;
    IInputLayout& layout = m_Layout;
    IVertexShader& shader = m_Shader;
    IRasterizerState& raster = m_RasterizerState;
    IBlendState& blend = m_BlendState;

    m_pCommand->Begin();
    m_pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    m_pCommand->SetVertex( 0, vertex.Get(), 0, 12+16 );
    m_pCommand->SetInputLayout( layout.Get() );
    m_pCommand->SetRasterizerState( raster.Get() );
    m_pCommand->SetBlendState( blend.Get() );
    m_pCommand->VSSetShader( shader.Get() );
    m_pCommand->Draw( 3, 0 );
    m_pCommand->End();

  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  Graphics::SPDRAWCOMMAND m_pCommand;
  GraphicsRender  m_Render;
  VertexShader    m_Shader;
  Vertex          m_Vertex;
  InputLayout     m_Layout;
  RasterizerState m_RasterizerState;
  BlendState      m_BlendState;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

