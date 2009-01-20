#ifndef maid2_graphics_graphicsrender_h
#define maid2_graphics_graphicsrender_h

/*!
    @file
    @brief  描画を受け持つクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/size.h"
#include"../auxiliary/globalpointer.h"

#include"graphicscore.h"
#include"vertex.h"
#include"texture2dbase.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"
#include"font.h"
#include"fonttexturemanager.h"
#include"rendertargetbase.h"
#include"samplerstate.h"

namespace Maid
{
  class GraphicsRender
    : public GlobalPointer<GraphicsCore>
  {
  public:
    GraphicsRender();
    virtual ~GraphicsRender();

    void Initialize();

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );
    void ClearDepth( float Depth );
    void ClearStencil( unt08 Stencil );
    void ClearDepthStencil( float Depth, unt08 Stencil );

    void SetVirtualScreenSize( const SIZE2DI& Size );

    void Begin();
    void End();


    //  2D系描画な関数
    //  左上が(0,0)で右下が(w,h)です
    void SetDefaultSetting();

    enum BLENDSTATE
    {
      BLENDSTATE_NONE=0,  //  何もしない
      BLENDSTATE_ALPHA, //  α合成のみ
      BLENDSTATE_ADD,   //  加算
      BLENDSTATE_SUB,   //  減算
      BLENDSTATE_MUL,   //  乗算
      BLENDSTATE_SCREEN,   //  スクリーン
      BLENDSTATE_DODGE,    //  覆い焼き
      BLENDSTATE_BURN,     //  焼き込み
      BLENDSTATE_DARKEN,   //  比較(暗)
      BLENDSTATE_LIGHTTEN, //  比較(明)
    };

    enum TEXTUREFILTER
    {
      TEXTUREFILTER_POINT,
      TEXTUREFILTER_LINEAR,
    };

    void SetFilterState( TEXTUREFILTER filter );
    void SetBlendState( BLENDSTATE State );
    void SetRenderTarget( const RenderTargetBase& Target );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

		void Fill  ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center );
		void FillS ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale );
		void FillR ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec );
		void FillSR( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );

		void Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha );
		void BltS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale );
		void BltR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec );
		void BltSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );

		void BltText( const POINT2DI& Base, const Font& f, const String& Text, const COLOR_R32G32B32A32F& Color, size_t Len );
		void BltText( const POINT2DI& Base, const Font& f, const String& Text, const COLOR_R32G32B32A32F& Color );

    bool IsInitializing() const;

  protected:

    struct MODELINFO
    {
      VECTOR4DF Model[4];   //  座標情報
      MATRIX4DF Translate;  //  移動量をあらわした行列
      float     Acpect;
    };

    void CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& Model );
    void SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride );
    void SetCommonVertex( void* pVertex, int size, int Stride );

    void Fill( const VECTOR4DF* Model, const MATRIX4DF& mat, const COLOR_R32G32B32A32F& Color );
    void Blt( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture, const RECT2DI& rc, const COLOR_R32G32B32A32F& Color );

  protected:
    bool IsMemberLoading() const;

    enum STATE
    {
      STATE_EMPTY,    //  まだ初期化されてない
      STATE_LOADING,  //  初期設定中
      STATE_WORKING,  //  稼動できる
    };

    STATE m_State;

    BLENDSTATE  m_BltState;

  protected:
    SIZE2DI   m_VirtualScreenSize; //  仮想スクリーンサイズ
    Graphics::SPDRAWCOMMAND m_pDrawCommand;
    Graphics::SPFONTDEVICE      m_pFontDevice;

    FontTextureManager  m_FontManager;

    Vertex       m_CommonVertex;  //  一時的な書き込みで使うバッファ

    InputLayout  m_FillLayout;
    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;

    InputLayout  m_BltLayout;
    VertexShader m_BltVertexShader;
    PixelShader  m_BltPixelShader210;
    PixelShader  m_BltPixelShader211;

    RasterizerState m_Rasterizer;

    SamplerState  m_SamplerPoint;
    SamplerState  m_SamplerLinar;

    DepthStencilState m_DepthOff;

    std::vector<BlendState> m_BlendState;

    int m_BeginCount;
  };	
	
}

#endif