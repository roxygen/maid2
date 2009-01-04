/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_graphicscore_h
#define maid2_graphics_graphicscore_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/mathematics/size.h"


#include"pixelformat.h"


#include"core/idevice.h"
#include"core/ifontdevice.h"

#include<set>
#include<vector>

namespace Maid {

  class GraphicsCore : public GlobalPointer<GraphicsCore>
	{
	public:
		GraphicsCore();
		virtual ~GraphicsCore();

		void Initialize( const Graphics::SPDEVICE& pDevice, const Graphics::SPFONTDEVICE& pFont );
		void Finalize();

    const Graphics::SPDEVICE& GetDevice();
    const Graphics::SPFONTDEVICE& GetFontDevice();

    PIXELFORMAT FindFormatTexture2D( PIXELFORMAT fmt, bool IsRenderTarget )const;
    PIXELFORMAT FindFormatRenderTarget( PIXELFORMAT fmt )const;
    PIXELFORMAT FindFormatDepthStencil( PIXELFORMAT fmt )const;

    SIZE2DI GetMaxTextureSize() const;

    SIZE2DI CalcTextureSize( const SIZE2DI& size ) const;

	private:
    typedef std::set<PIXELFORMAT> PIXELFORMATLIST;
    typedef std::set<int>         INDEXBITLIST;

    void UpdateMember();
    bool IsExist( const PIXELFORMATLIST& list, PIXELFORMAT fmt )const;


	private:
    Graphics::SPDEVICE	    m_pDevice;
    Graphics::SPFONTDEVICE  m_pFontDevice;
    std::vector<Graphics::DISPLAYMODE>  m_EnableDisplayMode;
    Graphics::ENABLEFORMAT  m_EnableFormat;


    PIXELFORMATLIST m_TextureFormat;
    PIXELFORMATLIST m_RenderTargetTextureFormat;
    PIXELFORMATLIST m_RenderTargetFormat;
    PIXELFORMATLIST m_DepthStencilFormat;
    INDEXBITLIST    m_IndexBitFormat;

  };
}


#endif
