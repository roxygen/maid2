/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_core_graphicscore_h
#define maid2_graphics_core_graphicscore_h

#include"../../config/define.h"



#include"igraphicsdevice.h"
#include"isurfacebuffer.h"
#include"itexturebuffer.h"
#include"ivertexbuffer.h"
#include"iindexbuffer.h"
#include"ivertexshaderbuffer.h"
#include"ipixelshaderbuffer.h"
#include"ivertexdeclarationbuffer.h"


#include"../graphicssendmessagebase.h"
#include"../graphicsreturnmessagebase.h"

#include<vector>
#include<map>

namespace Maid
{
	class GraphicsCore
	{
	public:
		GraphicsCore();
		virtual ~GraphicsCore();

		void Initialize( const SPGRAPHICSEVICE& pDevice );
		void Finalize();

		void SwapQue( GRAPHICSRENDERMESSAGEQUE& Send, GRAPHICSRETURNMESSAGEQUE& Return );
		void DrawFrame();

	private:
		void ExecuteMessage( const SPRENDERMESSAGE& mess );

    GRAPHICSRENDERMESSAGEQUE  m_RenderQue;
    GRAPHICSRETURNMESSAGEQUE  m_ReturnQue;

	private:
		SPVIDEODEVICE	m_pDevice;

    typedef std::map<GraphicsSendMessage::OBJECTID,SPSURFACEBUFFER>			  SURFACEMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPTEXTUREBUFFER>			  TEXTUREMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPINDEXBUFFER>			    INDEXMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPVERTEXBUFFER>		    	VERTEXMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPVERTEXSHADERBUFFER>	  VERTEXSHADERMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPPIXELSHADERBUFFER>		PIXELSHADERMAP;
		typedef std::map<GraphicsSendMessage::OBJECTID,SPVERTEXDECLARATIONBUFFER>		DECLARATIONMAP;
		SURFACEMAP		  m_SurfaceMap;
		TEXTUREMAP		  m_TextureMap;
		INDEXMAP	    	m_IndexMap;
		VERTEXMAP	    	m_VertexMap;
		VERTEXSHADERMAP	m_VertexShaderMap;
		PIXELSHADERMAP	m_PixelShaderMap;
		DECLARATIONMAP	m_DeclarationMap;
	};
}


#endif
