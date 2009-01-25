#include"graphics3drender.h"


namespace Maid
{

Graphics3DRender::Graphics3DRender()
  :m_ViewMatrix(MATRIX4DF().SetIdentity())
  ,m_ProjectionMatrix(MATRIX4DF().SetIdentity())
  ,m_State(STATE_EMPTY)
{

}

Graphics3DRender::~Graphics3DRender()
{

}

//! このクラスを使えるように初期化
/*!
    設定を勝手に調べて、自動的に行います
*/
void Graphics3DRender::Initialize()
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();
  Graphics::SPDRAWCOMMAND pCom = pCore->GetDevice()->GetDrawCommandExecute();

  Initialize( pCom );
}

void Graphics3DRender::Initialize( const Graphics::SPDRAWCOMMAND& pCommand )
{
  m_pDrawCommand = pCommand;

  m_State = STATE_LOADING;
}

//! 設定されている RenderTarget を塗りつぶす
/*!
    SLI機能(Scalable Link Interface)を使うには、この呼び出しが必須らしいです。
    原理を考えれば当たり前ですね。

    @param  Col [i ]  塗りつぶす色
 */
void Graphics3DRender::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}

//! 設定されている DepthStencil のうち、Depthを塗りつぶす
/*!
    普通は 1.0f でしょうね。

    @param  Depth [i ]  設定するdepth値
 */
void Graphics3DRender::ClearDepth( float Depth )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTH, Depth, 0 );
}

//! 設定されている DepthStencil のうち、Stencilを塗りつぶす
/*!
    普通は 0xFF でしょうね。

    @param  Stencil [i ]  設定する stencil値
 */
void Graphics3DRender::ClearStencil( unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_STENCIL, 0, Stencil );
}

//! 設定されている DepthStencil を塗りつぶす
/*!
    普通は 1.0f、0xFF でしょうね。

    @param  Depth   [i ]  設定する depth値
    @param  Stencil [i ]  設定する stencil値
 */
void Graphics3DRender::ClearDepthStencil( float Depth, unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTHSTENCIL, Depth, Stencil );
}

void Graphics3DRender::Begin()
{
  m_pDrawCommand->Begin();
  SetDefaultSetting();
}

void Graphics3DRender::End()
{
  m_pDrawCommand->End();
}


void Graphics3DRender::SetCamera( const Camera& cam )
{
  m_ViewMatrix = cam.GetViewMatrix();
  m_ProjectionMatrix = cam.GetProjectionMatrix();
}

void Graphics3DRender::SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Depth.Get() );

  const RECT2DI rc = Target.CalcDrawArea();
  const Graphics::VIEWPORT view( rc, 0, 1 );

  m_pDrawCommand->SetViewPort( view );
}

//! 初期化中か？
/*!
    @return   初期化中なら true
              まだなら false
 */
bool Graphics3DRender::IsInitializing() const
{
  if( m_State!=STATE_LOADING ) { return false; }


  //  メンバが読み込み中なら当然 true
  if( IsMemberLoading() ) { return true; }

  //  読み込み終わったので、残りの処理を行う

  const_cast<Graphics3DRender*>(this)->m_State = STATE_WORKING;
  const_cast<Graphics3DRender*>(this)->SetDefaultSetting();
  return false;

}

//! 最低限の設定を行う
/*!
    このクラスの描画周りで困ったらこれを呼んでリセットする感じ
 */
void Graphics3DRender::SetDefaultSetting()
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );
/*
  {
    IDepthStencilState& state = m_DepthOff;
    m_pDrawCommand->SetDepthStencilState( state.Get() );
  }
  SetBlendState( BLENDSTATE_ALPHA );
  SetFilterState( TEXTUREFILTER_LINEAR );
  IRasterizerState& raster = m_Rasterizer;
  m_pDrawCommand->SetRasterizerState( raster.Get() );
*/
}

bool Graphics3DRender::IsMemberLoading() const
{
  return false;
}

}

