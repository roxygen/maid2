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

namespace Maid
{
  class GraphicsRender
  {
  public:
    void Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand );

    void ClearRenderTarget( const COLOR_A32B32G32R32F& Col );

		void Fill( const RECT2DF&  rc, const COLOR_A32B32G32R32F& Color );

    bool IsLoading() const;

  protected:
    SIZE2DI   m_ScreenSize; //  仮想スクリーンサイズ
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    Vertex       m_FillVertex;
    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;
  };	
	
}

#endif