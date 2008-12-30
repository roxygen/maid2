#include"scenetext.h"

#include"../../app.h"

using namespace Maid;

void SceneText::Initialize( const SPSCENEINPUT& pInput )
{
  App* pApp = GlobalPointer<App>::Get();

  m_pFont = pApp->GetRender().CreateFont( SIZE2DI(16,32) );

}

bool SceneText::IsInitializing() const
{
  return false;
}

void SceneText::Finalize( SPSCENEOUTPUT& pOutput )
{

}

void SceneText::UpdateFrame()
{

}

void SceneText::UpdateDraw()
{
  GraphicsRender& Render = GlobalPointer<App>::Get()->GetRender();

  Render.Begin();
  Render.BltText( POINT2DI(0,0), m_pFont, MAIDTEXT("テキストを表示するシーン"), COLOR_R32G32B32A32F(1,1,1,1) );
  Render.End();
}

void SceneText::LastDraw( IRenderTarget& target, IDepthStencil& depth )
{

}
