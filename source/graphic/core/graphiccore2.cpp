#include"graphiccore.h"


#include"../graphicsendmessage1.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/debug/warning.h"
//#include"Driver/CPlaneTransiter.h"

namespace Maid
{
	using namespace GraphicSendMessage;

void GraphicCore::ExecuteMessage(  const SPRENDERMESSAGE& pMessage )
{
//	MAID_TRACE( MAIDTEXT("ExecuteRender:") << pMessage->GetMessage() );

	const SPVIDEODEVICE&	pDev = m_pDevice;

	const Base::MESSAGE mess = pMessage->Message;

	switch( mess )
	{
	default:
		{
			MAID_ASSERT( true, "未実装の命令です" << mess );
		}break;

	case Base::NONE:
		{

		}break;

/*
	case Base::SCREENCHANGE:
		{
			ScreenChange* pMess = static_cast<ScreenChange*>(pMessage.get());
			m_DefaultScreenMode = pMess->Mode;
			pDriver->EndScene();

			struct TEXINF
			{
				SIZE2DI		Size;		//!<	テクスチャの大きさ
				PIXELFORMAT	Format;		//!<	ピクセルフォーマット
			};

			MySTL::map<mstring,TEXINF> tmp;
			for( TEXTUREMAP::iterator ite=m_TextureMap.begin(); ite!=m_TextureMap.end(); ++ite )
			{
				if( ite->second.IsManaged ) { continue; }

				TEXINF inf;
				inf.Size   = ite->second.pBuffer->GetSize();
				inf.Format = ite->second.pBuffer->GetPixelFormat();

				tmp[ite->first] = inf;
				ite->second.pBuffer.reset();
			}

			pDriver->Reset( pMess->Mode );

			for( MySTL::map<mstring,TEXINF>::iterator ite=tmp.begin(); ite!=tmp.end(); ++ite )
			{
				TEXINF& inf = ite->second;
				SPTEXTUREBUFFER pBuffer = pDriver->CreateRTTextureBuffer( inf.Size, inf.Format );
				m_TextureMap[ite->first] = TEXTUREINFO(pBuffer,false);
			}

			pDriver->BeginScene();
		}break;
*/
#if 0
	case Base::REGISTSURFACE:
		{
			RegistSurface* pMess = static_cast<RegistSurface*>(pMessage.get());

			SPSURFACEBUFFER pBuffer = pDriver->CreateSurface( pMess->Size, pMess->Format );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_SurfaceMap[pMess->ResistID] = pBuffer;

		}break;

	case Base::REGISTTEXTURE:
		{
			RegistTexture* pMess = static_cast<RegistTexture*>(pMessage.get());

			{
				TEXTUREMAP::iterator ite = m_TextureMap.find(pMess->ResistID);
				if( ite!=m_TextureMap.end() )
				{
					const SIZE2DI DstSize = ite->second.pBuffer->GetSize();
					const SIZE2DI SrcSize = pMess->pTexture->GetSize();

					if( SrcSize.w <= DstSize.w && SrcSize.h <= DstSize.h )
					{
						const RECT2DI rc( POINT2DI(0,0), SrcSize );

						ISurfaceBuffer& dst = ite->second.pBuffer->GetSurface(0);
						ISurfaceBuffer& src = pMess->pTexture->GetSurface(0);

						CPlaneTransiter::Blt( dst, &rc, src, &rc );
						break;
					}
				}
			}

			SPTEXTUREBUFFER pBuffer = pDriver->CreateTextureBuffer( *(pMess->pTexture) );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_TextureMap[pMess->ResistID] = TEXTUREINFO(pBuffer,true);

		}break;

	case Base::REGISTRTTEXTURE:
		{
			RegistRTTexture* pMess = static_cast<RegistRTTexture*>(pMessage.get());

			SPTEXTUREBUFFER pBuffer = pDriver->CreateRTTextureBuffer( pMess->Size, pMess->Format );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_TextureMap[pMess->ResistID] = TEXTUREINFO(pBuffer,false);

		}break;

	case Base::REGISTINDEX:
		{
			RegistIndex* pMess = static_cast<RegistIndex*>(pMessage.get());
			SPINDEXBUFFER pBuffer = pDriver->CreateIndexBuffer( *(pMess->pIndex), pMess->Stride );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_IndexMap[pMess->ResistID] = pBuffer;

		}break;

	case Base::REGISTVERTEX:
		{
			RegistVertex* pMess = static_cast<RegistVertex*>(pMessage.get());
			SPVERTEXBUFFER pBuffer = pDriver->CreateVertexBuffer( *(pMess->pVertex), pMess->Format );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_VertexMap[pMess->ResistID] = pBuffer;

		}break;

	case Base::REGISTVERTEXDECLARATION:
		{
			RegistVertexDeclaration* pMess = static_cast<RegistVertexDeclaration*>(pMessage.get());
			SPVERTEXDECLARATIONBUFFER pBuffer = pDriver->CreateVertexDeclarationBuffer( pMess->ResistID );
			MAID_ASSERT( pBuffer.get()==NULL, "pBuffer==NULL" );
			m_DeclarationMap[pMess->ResistID] = pBuffer;

		}break;


	case Base::REGISTVERTEXSHADER:
		{
			RegistVertexShader* pMess = static_cast<RegistVertexShader*>(pMessage.get());

			SPVERTEXSHADERBUFFER pShader;
			if( pMess->CodeType==RegistVertexShader::TYPE_DEFAULT )
			{
				pShader = pDriver->CreateVertexShader( pMess->ResistID );
			}else
			{
/*
				IVideoCardDriver::SHADERCOMPILE type;

				switch( pMess->CodeType )
				{
				case RegistVertexShader::TYPE_COPY:			{ type = IVideoCardDriver::SHADERCOMPILE_COPY; }break;
				case RegistVertexShader::TYPE_ASSEMBLE:		{ type = IVideoCardDriver::SHADERCOMPILE_ASSEMBLE; }break;
				case RegistVertexShader::TYPE_COMPILE:		{ type = IVideoCardDriver::SHADERCOMPILE_COMPILE; }break;
				}

				pShader = pDriver->CreateVertexShader( *(pMess->pCode), type );
*/
				MAID_ASSERT( true, "未実装" );
			}
			MAID_ASSERT( pShader.get()==NULL, "pShader==NULL" );
			m_VertexShaderMap[pMess->ResistID] = pShader;
		}break;

	case Base::REGISTPIXELSHADER:
		{
			RegistPixelShader* pMess = static_cast<RegistPixelShader*>(pMessage.get());

			SPPIXELSHADERBUFFER pShader;
			if( pMess->CodeType==RegistPixelShader::TYPE_DEFAULT )
			{
				pShader = pDriver->CreatePixelShader( pMess->ResistID );
			}else
			{
/*
				IVideoCardDriver::SHADERCOMPILE type;

				switch( pMess->CodeType )
				{
				case RegistPixelShader::TYPE_COPY:			{ type = IVideoCardDriver::SHADERCOMPILE_COPY; }break;
				case RegistPixelShader::TYPE_ASSEMBLE:		{ type = IVideoCardDriver::SHADERCOMPILE_ASSEMBLE; }break;
				case RegistPixelShader::TYPE_COMPILE:		{ type = IVideoCardDriver::SHADERCOMPILE_COMPILE; }break;
				}

				pShader = pDriver->CreatePixelShader( *(pMess->pCode), type );
*/
				MAID_ASSERT( true, "未実装" );
			}
			MAID_ASSERT( pShader.get()==NULL, "pShader==NULL" );
			m_PixelShaderMap[pMess->ResistID] = pShader;
		}break;


	//	Deleteメッセージについて
	//	10F REGVERTEX
	//	11F DELVERTEX
	//	11F DELVERTEX
	//	となると、見つからないケースがあるので
	//	XXXMap にない場合は　エラーでなくスルーする

	case Base::DELETESURFACE:
		{
			DeleteSurface* pMess = static_cast<DeleteSurface*>(pMessage.get());
			SURFACEMAP::iterator ite = m_SurfaceMap.find( pMess->DeleteID );

			if( ite==m_SurfaceMap.end() ) { return ; }

			m_SurfaceMap.erase( ite );
		}break;

	case Base::DELETETEXTURE:
		{
			DeleteTexture* pMess = static_cast<DeleteTexture*>(pMessage.get());
			TEXTUREMAP::iterator ite = m_TextureMap.find( pMess->DeleteID );

			if( ite==m_TextureMap.end() ) { return ; }

			m_TextureMap.erase( ite );
		}break;

	case Base::DELETEVERTEX:
		{
			DeleteVertex* pMess = static_cast<DeleteVertex*>(pMessage.get());
			VERTEXMAP::iterator ite = m_VertexMap.find( pMess->DeleteID );

			if( ite==m_VertexMap.end() ) { return ; }

			m_VertexMap.erase( ite );
		}break;

	case Base::DELETEINDEX:
		{
			DeleteIndex* pMess = static_cast<DeleteIndex*>(pMessage.get());
			INDEXMAP::iterator ite = m_IndexMap.find( pMess->DeleteID );

			if( ite==m_IndexMap.end() ) { return ; }

			m_IndexMap.erase( ite );
		}break;

	case Base::DELETEVERTEXSHADER:
		{
			DeleteVertexShader* pMess = static_cast<DeleteVertexShader*>(pMessage.get());
			VERTEXSHADERMAP::iterator ite = m_VertexShaderMap.find( pMess->DeleteID );
			if( ite==m_VertexShaderMap.end() ) { return; }

			m_VertexShaderMap.erase( ite );
		}break;

	case Base::DELETEPIXELSHADER:
		{
			DeletePixelShader* pMess = static_cast<DeletePixelShader*>(pMessage.get());
			PIXELSHADERMAP::iterator ite = m_PixelShaderMap.find( pMess->DeleteID );
			if( ite==m_PixelShaderMap.end() ) { return ; }

			m_PixelShaderMap.erase( ite );
		}break;

	case Base::DELETEVERTEXDECLARATION:
		{
			DeleteVertexDeclaration* pMess = static_cast<DeleteVertexDeclaration*>(pMessage.get());
			DECLARATIONMAP::iterator ite = m_DeclarationMap.find( pMess->DeleteID );
			if( ite==m_DeclarationMap.end() ) { return ; }

			m_DeclarationMap.erase( ite );
		}break;
#endif
	case Base::CLEAR_RENDERTARGET:
		{
			ClearRenderTarget* pMess = static_cast<ClearRenderTarget*>(pMessage.get());
			pDev->ClearRenderTarget( pMess->Color );
		}break;

	case Base::CLEAR_DEPTHSTENCIL:
		{
			ClearDepthStencil* pMess = static_cast<ClearDepthStencil*>(pMessage.get());
			pDev->ClearDepthStencil( pMess->Depth, pMess->Stencil );
		}break;
#if 0
	case Base::SETTEXTURE:
		{
			RenderMessage::SetTexture* pMess = static_cast<RenderMessage::SetTexture*>(pMessage.get());

			TEXTUREMAP::iterator ite = m_TextureMap.find( pMess->ID );
			MAID_ASSERT( ite==m_TextureMap.end(), MAIDTEXT("Texture:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetTextureBuffer( ite->second.pBuffer, pMess->Stage );

		}break;

	case Base::SETVERTEX:
		{
			RenderMessage::SetVertex* pMess = static_cast<RenderMessage::SetVertex*>(pMessage.get());

			VERTEXMAP::iterator ite = m_VertexMap.find( pMess->ID );
			MAID_ASSERT( ite==m_VertexMap.end(), MAIDTEXT("Vertex:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetVertexBuffer( ite->second, pMess->pos );
		}break;

	case Base::SETINDEX:
		{
			RenderMessage::SetIndex* pMess = static_cast<RenderMessage::SetIndex*>(pMessage.get());

			INDEXMAP::iterator ite = m_IndexMap.find( pMess->ID );
			MAID_ASSERT( ite==m_IndexMap.end(), MAIDTEXT("Index:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetIndexBuffer( ite->second );
		}break;

	case Base::SETVERTEXDECLARATION:
		{
			RenderMessage::SetVertexDeclaration* pMess = static_cast<RenderMessage::SetVertexDeclaration*>(pMessage.get());

			DECLARATIONMAP::iterator ite = m_DeclarationMap.find( pMess->ID );
			MAID_ASSERT( ite==m_DeclarationMap.end(), MAIDTEXT("Declaration:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetVertexDeclaration( ite->second );
		}break;

	case Base::SETVERTEXSHADER:
		{
			RenderMessage::SetVertexShader* pMess = static_cast<RenderMessage::SetVertexShader*>(pMessage.get());

			VERTEXSHADERMAP::iterator ite = m_VertexShaderMap.find( pMess->ID );

			MAID_ASSERT( ite==m_VertexShaderMap.end(), MAIDTEXT("VertexShader:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetVertexShader( ite->second );

		}break;

	case Base::SETPIXELSHADER:
		{
			RenderMessage::SetPixelShader* pMess = static_cast<RenderMessage::SetPixelShader*>(pMessage.get());

			PIXELSHADERMAP::iterator ite = m_PixelShaderMap.find( pMess->ID );

			MAID_ASSERT( ite==m_PixelShaderMap.end(), MAIDTEXT("PixelShader:") << pMess->ID << MAIDTEXT("が見つかりません") );

			pDriver->SetPixelShader( ite->second );
		}break;


	case Base::SETVERTEXSHADER_CONSTF:
		{
			RenderMessage::SetVertexShaderConstF* pMess = static_cast<RenderMessage::SetVertexShaderConstF*>(pMessage.get());

			pDriver->SetVertexShaderConstF( pMess->pos, pMess->vec );

		}break;

	case Base::DRAWPRIMITIVE:
		{
			RenderMessage::DrawPrimitive* pMess = static_cast<RenderMessage::DrawPrimitive*>(pMessage.get());

			pDriver->DrawPrimitive( (IVideoCardDriver::PRIMITIVE)pMess->type, pMess->StartVertex, pMess->PrimitiveCount );

		}break;

	case Base::DRAWPRIMITIVEUP:
		{
			RenderMessage::DrawPrimitiveUP* pMess = static_cast<RenderMessage::DrawPrimitiveUP*>(pMessage.get());

			pDriver->DrawPrimitiveUP( (IVideoCardDriver::PRIMITIVE)pMess->type, 
				&(pMess->Vertex[0]),
				pMess->Stride,
				pMess->PrimitiveCount
				);
		}break;

	case Base::DRAWINDEXPRIMITIVE:
		{
			RenderMessage::DrawIndexedPrimitive* pMess = static_cast<RenderMessage::DrawIndexedPrimitive*>(pMessage.get());

			pDriver->DrawIndexedPrimitive( (IVideoCardDriver::PRIMITIVE)pMess->type, 
				pMess->VertexOffset,
				pMess->MinIndex,
				pMess->NumVertex,
				pMess->StartIndex,
				pMess->PrimitiveCount
				);
		}break;

	case Base::DRAWINDEXPRIMITIVEUP:
		{
			RenderMessage::DrawIndexedPrimitiveUP* pMess = static_cast<RenderMessage::DrawIndexedPrimitiveUP*>(pMessage.get());

			pDriver->DrawIndexPrimitiveUP( (IVideoCardDriver::PRIMITIVE)pMess->type, 
				pMess->MinIndex,
				pMess->NumVertices,
				&(pMess->Vertex[0]),
				pMess->VertexSize,
				pMess->PrimitiveCount,
				&(pMess->Index[0]),
				pMess->IndexSize
				);
		}break;

	case Base::SETRENDERSTATE:
		{
			RenderMessage::SetRenderState* pMess = static_cast<RenderMessage::SetRenderState*>(pMessage.get());

			pDriver->SetRenderState( pMess->state, pMess->Value );
		}break;

	case Base::SETSAMPLERSTATE:
		{
			RenderMessage::SetSamplerState* pMess = static_cast<RenderMessage::SetSamplerState*>(pMessage.get());

			pDriver->SetSamplerState( pMess->Stage, pMess->State, pMess->Value );
		}break;

	case Base::RESETRENDERTARGET:
		{
			RenderMessage::ResetRenderTarget* pMess = static_cast<RenderMessage::ResetRenderTarget*>(pMessage.get());

			switch( pMess->Target )
			{
			case RenderMessage::ResetRenderTarget::TARGET_COLOR:
				{
					pDriver->ResetRenderTarget();
				}break;
			case RenderMessage::ResetRenderTarget::TARGET_DEPTHSTENCIL:
				{
					pDriver->ResetDepthStencil();
				}break;
			}
		}break;

	case Base::SETRENDERTARGET:
		{
			RenderMessage::SetRenderTarget* pMess = static_cast<RenderMessage::SetRenderTarget*>(pMessage.get());

			ISurfaceBuffer* pSurface;

			switch( pMess->Type )
			{
			case RenderMessage::SetRenderTarget::TYPE_SURFACE:
				{
					SURFACEMAP::iterator ite = m_SurfaceMap.find( pMess->ID );

					MAID_ASSERT( ite==m_SurfaceMap.end(), MAIDTEXT("Surface:") << pMess->ID << MAIDTEXT("が見つかりません") );

					pSurface = ite->second.get();
				}break;
			case RenderMessage::SetRenderTarget::TYPE_TEXTURE:
				{
					TEXTUREMAP::iterator ite = m_TextureMap.find( pMess->ID );
					MAID_ASSERT( ite==m_TextureMap.end(), MAIDTEXT("Texture:") << pMess->ID << MAIDTEXT("が見つかりません") );

					pSurface = &(ite->second.pBuffer->GetSurface(0));
				}break;
			}

			switch( pMess->Target )
			{
			case RenderMessage::SetRenderTarget::TARGET_COLOR:
				{
					pDriver->SetRenderTarget(pSurface);
				}break;
			case RenderMessage::SetRenderTarget::TARGET_DEPTHSTENCIL:
				{
					pDriver->SetDepthStencil(pSurface);
				}break;
			}

		}break;

	case Base::SETVIEWPORT:
		{
			RenderMessage::SetViewPort* pMess = static_cast<RenderMessage::SetViewPort*>(pMessage.get());

			pDriver->SetViewPort( pMess->Screen, pMess->MinZ, pMess->MaxZ );
		}break;

	case Base::COPYSURFACE:
		{
			RenderMessage::CopySurface* pMess = static_cast<RenderMessage::CopySurface*>(pMessage.get());

			SPSURFACEBUFFERMEMORY pDst;
			{
				SPSURFACEBUFFER pSurface;
				{
					SURFACEMAP::iterator ite = m_SurfaceMap.find( pMess->ID );

					MAID_ASSERT( ite==m_SurfaceMap.end(), MAIDTEXT("Surface:") << pMess->ID << MAIDTEXT("が見つかりません") );

					pSurface = ite->second;
				}

				pDriver->CopySurface( pSurface, pDst );
			}

			ReturnMessage::GetSurface* pRet = new ReturnMessage::GetSurface;

			pRet->ID = pMess->ID;
			pRet->pSurface = pDst;

			Return.push_back( SPRETURNMESSAGE(pRet) );
		}break;

	case Base::GETVIDEOINFO:
		{
			ReturnMessage::DriverCaps* pRet = new ReturnMessage::DriverCaps;

			pRet->CapsText = pDriver->GetVideoInfo();

			Return.push_back( SPRETURNMESSAGE(pRet) );
		}break;
#endif
	}
}

}

