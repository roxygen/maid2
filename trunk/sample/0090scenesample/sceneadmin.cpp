#include"sceneadmin.h"
#include"scene/text/scenetext.h"
#include"scene/image/sceneimage.h"
#include"app.h"

using namespace Maid;


SPSCENE SceneAdmin::CreateFirstScene()
{
  SPSCENEINPUT pInput;
  SPSCENE pScene( new SceneText );

  pScene->Initialize( pInput );

  return pScene;
}

SPSCENE SceneAdmin::CreateNextScene( const Maid::SPSCENEOUTPUT& pOut )
{
  //  こんなとこにstatic あるけど、本当は App にゲームルーチンなクラスを持たせて
  //  そいつが行うべき

  static int count;

  ++count;

  SPSCENEINPUT pInput;
  SPSCENE pScene;

  switch( count%2 )
  {
  case 0: { pScene.reset( new SceneText ); }break;
  case 1: { pScene.reset( new SceneImage ); }break;
  }

  pScene->Initialize( pInput );

  return pScene;
}

void SceneAdmin::BeginFade()
{
  m_FadeAlpha.Set( 1.0f, 0.0f, 120 );
}

bool SceneAdmin::IsFadeEnd()
{
  return m_FadeAlpha.IsEnd();
}

void SceneAdmin::UpdateFadeFrame()
{
  ++m_FadeAlpha;
}


void SceneAdmin::UpdateFadeDraw( const Texture2DBase& target, const RECT2DI& rc )
{
  GraphicsRender& Render = GlobalPointer<App>::Get()->GetRender();
  const RenderTargetBase& rt = GlobalPointer<App>::Get()->GetGraphicsCore().GetBackBuffer();

  Render.SetRenderTarget( rt );

  Render.Begin();
  const RECT2DI rect( POINT2DI(0,0), target.GetSize() );
  const float alpha = m_FadeAlpha;
  Render.Blt( POINT2DI(0,0), target, rc, POINT2DI(0,0), alpha );
  Render.End();

/*
  Render.Begin();
  const SIZE2DI size = rc.GetSize();
  const POINT2DI pos(size.w/2,size.h/2);
  const POINT2DI center(size.w/2,size.h/2);
  const float alpha = m_FadeAlpha;
  Render.Blt( pos, target, rc, center, alpha );
  Render.End();
*/

}
