#include"rendertargetbase.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"

namespace Maid
{

  /*!
      @class  RenderTargetBase rendertargetbase.h
      @brief  描画対象となる平面をあらわす基本クラス
    \n        実際の大きさと異なるように振舞ってほしい場合、このクラスを継承して使うとベンリ
  */
RenderTargetBase::RenderTargetBase()
  :m_Size(0,0),m_TextureSize(0,0)
{

}

//! 作っていたものを開放する
/*!
 */
void RenderTargetBase::Clear()
{
  IRenderTarget::Clear();

  m_Size = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
}


//! すでに作成されているか？
/*!
    @return 作成されているなら true
 */
bool RenderTargetBase::IsSetupped() const
{
  return IRenderTarget::Get().get()!=NULL;
}

//! 各種設定しておく
/*!
    @param  pTarget [i ]  要求があったときに差し出すターゲット
    @param  create  [i ]  扱って欲しい平面の大きさ
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& create )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  IRenderTarget::Set(pTarget);

  const Graphics::CREATERETEXTURE2DPARAM param = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam();

  m_Size = create;
  m_TextureSize = param.Size;
  m_Format = param.Format;
}

//! 各種設定しておく
/*!
    @param  pTarget [i ]  要求があったときに差し出すターゲット
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  const SIZE2DI size = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam().Size;

  Setup( pTarget, size );
}

}

