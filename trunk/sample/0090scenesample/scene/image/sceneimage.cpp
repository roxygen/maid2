#include"sceneimage.h"

#include"../../app.h"

using namespace Maid;

void SceneImage::Initialize( const SPSCENEINPUT& pInput )
{
  m_Texture.LoadFile( MAIDTEXT("mo_005.bmp") );
  m_FrameCount = 0;
}

bool SceneImage::IsInitializing() const
{
  return m_Texture.IsLoading();
}

void SceneImage::Finalize( SPSCENEOUTPUT& pOutput )
{

}

void SceneImage::UpdateFrame()
{
  if( 300 < m_FrameCount ) { ExitScene(); }
  ++m_FrameCount;

}

void SceneImage::UpdateDraw( const Maid::RenderTargetBase& target, const Maid::IDepthStencil& depth )
{
  Graphics2DRender& Render = GlobalPointer<App>::Get()->GetRender();


  Render.SetRenderTarget(target);

  Render.Begin();
  const RECT2DI  rc( POINT2DI(0,0), m_Texture.GetImageSize() );
  const POINT2DI center(rc.w/2,rc.h/2);
	Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );
  Render.End();
}

void SceneImage::LastDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  Graphics2DRender& Render = GlobalPointer<App>::Get()->GetRender();
  Render.SetRenderTarget(target);
  Render.ClearRenderTarget( COLOR_A32B32G32R32F(0,0,0,0) );
  UpdateDraw(target,depth);
}
