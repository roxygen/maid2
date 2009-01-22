#include"isceneadmin.h"
#include"../graphics/depthstencil.h"


namespace Maid
{

  //  IScene も ISceneAdmin もお試し的なコードなので
  //  ゲームによってはつくりかえたほうがいいかも


//! このクラスの初期化
/*!
    @param  FadeSize  [i ]  シーン切り替え時に使うテクスチャ・デプスの大きさ
    @param  TargetFMT [i ]  シーン切り替え時に使うテクスチャのピクセルフォーマット
    @param  DepthFMT  [i ]  シーン切り替え時に使うデプスのピクセルフォーマット
*/
void ISceneAdmin::Initialize( const SIZE2DI& FadeSize, PIXELFORMAT TargetFMT, PIXELFORMAT DepthFMT )
{
  m_FadeSize = FadeSize;
  m_TargetFormat = TargetFMT;
  m_DepthFormat  = DepthFMT;

  m_State = STATE_UPDATEFRAME;

  m_pCurrentScene = CreateFirstScene();
}


//! 初期化が終わっているか？
/*!
*/
bool ISceneAdmin::IsInitializing() const
{
  return m_pCurrentScene->IsInitializing();
}


//! フレーム毎の更新
/*!
*/
void ISceneAdmin::UpdateFrame()
{
  switch( m_State )
  {
  case STATE_UPDATEFRAME:
    {
      //  シーンが終了していたら
      //  切り替え開始
      if( m_pCurrentScene->IsExit() )
      {
        {
          DepthStencil Depth;
          //  最後の描画をフェードアウト用に保存
          m_FadeTexture.Create( m_FadeSize, m_TargetFormat );
          Depth.CreateCompatible( m_FadeTexture, m_DepthFormat );
          m_pCurrentScene->LastDraw( m_FadeTexture, Depth );
          BeginFade();
        }

        { //  終了させて、次のシーンを作る
          SPSCENEOUTPUT pOut;
          m_pCurrentScene->Finalize( pOut );
          m_pCurrentScene.reset();

          m_pCurrentScene = CreateNextScene(pOut);
        }
        m_State = STATE_CHANGING;
      }else
      {
        SceneUpdateFrame();
      }
    }break;

  case STATE_CHANGING:
    {
      if( IsFadeEnd() )
      {
        m_FadeTexture.Destroy();
        m_State = STATE_UPDATEFRAME;
      }else
      {
        UpdateFadeFrame();
      }
      SceneUpdateFrame();
    }break;
  }

}

//! フレーム毎の描画
/*!
    @param  target  [i ]  描画先
    @param  depth   [i ]  描画先
*/
void ISceneAdmin::UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  SceneUpdateDraw(target,depth);

  if( m_State==STATE_CHANGING )
  {
    const RECT2DI rc = m_FadeTexture.CalcDrawArea();
    UpdateFadeDraw( m_FadeTexture, rc );
  }
}


void ISceneAdmin::SceneUpdateFrame()
{
  if( m_pCurrentScene->IsInitializing() ) { return ; }
  m_pCurrentScene->UpdateFrame();
}

void ISceneAdmin::SceneUpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  if( m_pCurrentScene->IsInitializing() ) { return ; }
  m_pCurrentScene->UpdateDraw(target,depth);
}

}

