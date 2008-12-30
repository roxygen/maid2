#ifndef maid2_graphics_graphicsrender_h
#define maid2_graphics_graphicsrender_h

/*!
    @file
    @brief  描画を受け持つクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/size.h"

#include"core/idrawcommand.h"
#include"vertex.h"
#include"texture2dbase.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"
#include"fonttexturemanager.h"

namespace Maid
{
  class GraphicsRender
  {
  public:
    GraphicsRender();
    virtual ~GraphicsRender();

    void Initialize( const SIZE2DI& ScreenSize, const Graphics::SPDRAWCOMMAND& pDrawCommand, const SPFONTDEVICE& pFontDevice );

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );

    void SetVirtualScreenSize( const SIZE2DI& Size );

    void Begin();
    void End();


    //  2D系描画な関数
    //  左上が(0,0)で右下が(w,h)です
    void SetDefaultSetting();
    void SetDepthTest( bool Enable );

    enum BLENDSTATE
    {
      BLENDSTATE_NONE=0,  //  何もしない
      BLENDSTATE_ALPHA, //  α合成のみ
      BLENDSTATE_ADD,   //  加算
      BLENDSTATE_SUB,   //  減算
      BLENDSTATE_MUL,   //  乗算
    };

    void SetBlendState( BLENDSTATE State );

		void Fill  ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center );
		void FillS ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale );
		void FillR ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec );
		void FillSR( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );

		void Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha );
		void BltS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale );
		void BltR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec );
		void BltSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );

    SPFONT CreateFont( const SIZE2DI& size );
		void BltText( const POINT2DI& Base, const SPFONT& pFont, const String& Text, const COLOR_R32G32B32A32F& Color, size_t Len );
		void BltText( const POINT2DI& Base, const SPFONT& pFont, const String& Text, const COLOR_R32G32B32A32F& Color );

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

  protected:
    SIZE2DI   m_VirtualScreenSize; //  仮想スクリーンサイズ
    Graphics::SPDRAWCOMMAND m_pDrawCommand;
    SPFONTDEVICE      m_pFontDevice;

    FontTextureManager  m_FontManager;

    Vertex       m_CommonVertex;  //  一時的な書き込みで使うバッファ

    InputLayout  m_FillLayout;
    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;

    InputLayout  m_BltLayout;
    VertexShader m_BltVertexShader;
    PixelShader  m_BltPixelShader;

    PixelShader  m_FontPixelShader;

    RasterizerState m_Rasterizer;

    DepthStencilState m_DepthOn;
    DepthStencilState m_DepthOff;

    std::vector<BlendState> m_BlendState;

    int m_BeginCount;
  };	
	
}

#endif