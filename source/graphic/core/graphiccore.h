/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef graphic_core_graphiccore_h
#define graphic_core_graphiccore_h

#include"../../config/define.h"
#include"videodevicelist.h"



#include"driver/ivideodevice.h"
#include"driver/isurfacebuffer.h"
#include"driver/itexturebuffer.h"
#include"driver/ivertexbuffer.h"
#include"driver/iindexbuffer.h"
#include"driver/ivertexshaderbuffer.h"
#include"driver/ipixelshaderbuffer.h"
#include"driver/ivertexdeclarationbuffer.h"


#include"../graphicsendmessagebase.h"
#include"../graphicreturnmessagebase.h"

#include<vector>
#include<map>

namespace Maid
{
	class GraphicCore
	{
	public:
		GraphicCore();
		virtual ~GraphicCore();

		void Initialize( const SPVIDEODEVICE& pDevice );
		void Finalize();

		void SwapQue( RENDERMESSAGEQUE& Send, RETURNMESSAGEQUE& Return );
		void DrawFrame();

	private:
		void ExecuteMessage( const SPRENDERMESSAGE& mess );

    RENDERMESSAGEQUE  m_RenderQue;
    RETURNMESSAGEQUE  m_ReturnQue;

	private:
		SPVIDEODEVICE	m_pDevice;

    typedef std::map<GraphicSendMessage::OBJECTID,SPSURFACEBUFFER>			  SURFACEMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPTEXTUREBUFFER>			  TEXTUREMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPINDEXBUFFER>			    INDEXMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPVERTEXBUFFER>		    	VERTEXMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPVERTEXSHADERBUFFER>	  VERTEXSHADERMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPPIXELSHADERBUFFER>		PIXELSHADERMAP;
		typedef std::map<GraphicSendMessage::OBJECTID,SPVERTEXDECLARATIONBUFFER>		DECLARATIONMAP;
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
