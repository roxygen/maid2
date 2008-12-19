#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/vertexshader.h"
#include"../../source/graphics/vertex.h"
#include"../../source/graphics/inputlayout.h"
#include"../../source/graphics/customvertextemplate.h"
#include"../../source/graphics/rasterizerstate.h"

using namespace Maid;

  struct APP_CUSTOMVERTEX_COLOR : public CUSTOMVERTEX_TEMPLATE<APP_CUSTOMVERTEX_COLOR,
    VERTEXFORMAT_XYZ
   |VERTEXFORMAT_DIFFUSE
  >
  {
    //!	コンストラクタ
    APP_CUSTOMVERTEX_COLOR(){};
    APP_CUSTOMVERTEX_COLOR( const POINT3DF& pos, const COLOR_A32B32G32R32F& color )
      : Position(pos), Color(color){}
    POINT3DF			Position;	    //!< 頂点座標
    COLOR_A32B32G32R32F	Color;	//!< 頂点色
  };

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
      APP_CUSTOMVERTEX_COLOR v[] =
      {
        APP_CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), COLOR_A32B32G32R32F(1,1,1,1) ),
        APP_CUSTOMVERTEX_COLOR( POINT3DF(0,-1,0), COLOR_A32B32G32R32F(1,1,1,1) ),
        APP_CUSTOMVERTEX_COLOR( POINT3DF(1,1,0), COLOR_A32B32G32R32F(1,1,1,1) ),
      };

      m_Vertex.Create( v, sizeof(v), 0 );
    }

    {
      Graphics::IDevice::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::IDevice::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::IDevice::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::IDevice::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::IDevice::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_Layout.Create( element, NUMELEMENTS(element), NULL, 0 );
    }

    {
       Graphics::IDevice::RASTERIZERSTATEPARAM param;

       param.Fill = Graphics::IDevice::RASTERIZERSTATEPARAM::FILL_SOLID;
       param.Culling = Graphics::IDevice::RASTERIZERSTATEPARAM::CULLING_NONE;
       param.MultiSample = false;

       m_RasterizerState.Create( param );
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

    m_pCommand->Begin();
    m_pCommand->SetVertex( 0, vertex.Get(), 0, 12+16 );
    m_pCommand->SetInputLayout( layout.Get() );
    m_pCommand->SetRasterizerState( raster.Get() );
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
  RasterizerState     m_RasterizerState;
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

