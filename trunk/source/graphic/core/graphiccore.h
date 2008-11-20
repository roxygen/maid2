/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef graphic_core_graphiccore_h
#define graphic_core_graphiccore_h

#include"../../config/define.h"
#include"videodevicelist.h"



#include"driver/videodevice.h"
#include"driver/surfacebuffer.h"
#include"driver/texturebuffer.h"
#include"driver/vertexbuffer.h"
#include"driver/indexbuffer.h"
#include"driver/vertexshaderbuffer.h"
#include"driver/pixelshaderbuffer.h"
#include"driver/vertexdeclarationbuffer.h"


#include"../devicemessage.h"
//#include"../CReturnMessage.h"
#include<vector>
#include<map>

namespace Maid
{
	class CGraphicCore
	{
	public:
		CGraphicCore();
		virtual ~CGraphicCore();

		void Initialize( const SPVIDEODEVICE& pDevice );
		void Finalize();

//		void SwapRenderMessage( RENDERMESSAGEQUE& mess );
//		void DrawFrame( RETURNMESSAGEQUE& Return );

	private:
//		void ExecuteRenderMessage( const SPRENDERMESSAGE& pMessage, RETURNMESSAGEQUE& Return );

	private:
		SPVIDEODEVICE	m_pDevice;

		typedef std::map<mstring,SPSURFACEBUFFER>			SURFACEMAP;
		typedef std::map<mstring,SPTEXTUREBUFFER>			TEXTUREMAP;
		typedef std::map<mstring,SPINDEXBUFFER>			  INDEXMAP;
		typedef std::map<mstring,SPVERTEXBUFFER>			VERTEXMAP;
		typedef std::map<mstring,SPVERTEXSHADERBUFFER>	VERTEXSHADERMAP;
		typedef std::map<mstring,SPPIXELSHADERBUFFER>		PIXELSHADERMAP;
		typedef std::map<mstring,SPVERTEXDECLARATIONBUFFER>		DECLARATIONMAP;
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
