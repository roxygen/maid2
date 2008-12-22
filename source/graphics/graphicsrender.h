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
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"

namespace Maid
{
  class GraphicsRender
  {
  public:
    void Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand );

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

		void Fill  ( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, const COLOR_R32G32B32A32F& Color );
		void FillS ( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, const COLOR_R32G32B32A32F& Color );
		void FillR ( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec, const COLOR_R32G32B32A32F& Color );
		void FillSR( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const COLOR_R32G32B32A32F& Color );

    bool IsLoading() const;

  protected:

    struct MODELINFO
    {
      VECTOR4DF Model[4];   //  座標情報
      MATRIX4DF Translate;  //  移動量をあらわした行列
      float     Acpect;
    };

    void CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& Model );
    void SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride );

    void Fill( const VECTOR4DF* Model, const MATRIX4DF& mat, const COLOR_R32G32B32A32F& Color );

  protected:
    SIZE2DI   m_VirtualScreenSize; //  仮想スクリーンサイズ
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    Vertex       m_CommonVertex;  //  一時的な書き込みで使うバッファ


    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;
    InputLayout  m_FillLayout;

    RasterizerState m_Rasterizer;

    DepthStencilState m_DepthOn;
    DepthStencilState m_DepthOff;

    std::vector<BlendState> m_BlendState;
  };	
	
}

#endif