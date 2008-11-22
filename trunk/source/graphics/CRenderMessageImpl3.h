/*!	
	@file
	@brief	割り込み命令メッセージ 定義
*/

#ifndef CRenderMessageImpl3_h
#define CRenderMessageImpl3_h

#include"../Setup/CompileMode.h"
#include"CRenderMessage.h"
#include"CCustomVertexTemplate.h"

#include"Core/Driver/CVideoCardDriver.h"
#include"Core/Driver/CTextureBufferMemory.h"


namespace Maid
{
	namespace RenderMessage
	{
		class ScreenChange : public Base
		{
		public:
			ScreenChange() : Base(SCREENCHANGE){}

			IVideoCardDriver::SCREENMODE Mode;
		};

		class RegistSurface : public Base
		{
		public:
			RegistSurface() : Base(REGISTSURFACE){}

			mstring		ResistID;	//!<	登録する名前
			SIZE2DI		Size;		//!<	サーフェスの大きさ
			PIXELFORMAT	Format;		//!<	ピクセルフォーマット
		};

		class RegistTexture : public Base
		{
		public:
			RegistTexture() : Base(REGISTTEXTURE){}

			mstring									ResistID;	//!<	登録する名前
			boost::shared_ptr<CTextureBufferMemory>	pTexture;	//!<	登録するテクスチャ
		};

		class RegistRTTexture : public Base
		{
		public:
			RegistRTTexture() : Base(REGISTRTTEXTURE){}

			mstring		ResistID;	//!<	登録する名前
			SIZE2DI		Size;		//!<	テクスチャの大きさ
			PIXELFORMAT	Format;		//!<	ピクセルフォーマット
		};

		class RegistVertex : public Base
		{
		public:
			RegistVertex() : Base(REGISTVERTEX){}

			mstring			ResistID;	//!<	登録する名前
			SPBINARY		pVertex;	//!<	登録するデータ
			VERTEXFORMAT	Format;		//!<	フォーマット
		};

		class RegistIndex : public Base
		{
		public:
			RegistIndex() : Base(REGISTINDEX){}

			mstring		ResistID;	//!<	登録する名前
			SPBINARY	pIndex;		//!<	登録するデータ
			int			Stride;		//!<	１インデックスあたりの大きさ
		};

		class RegistVertexShader : public Base
		{
		public:
			RegistVertexShader() : Base(REGISTVERTEXSHADER){}

			enum TYPE
			{
				TYPE_DEFAULT,
				TYPE_CUSTOM,
			};

			TYPE		CodeType;
			mstring		ResistID;	//!<	登録する名前
			SPBINARY	pCode;		//!<	登録するコード
		};

		class RegistVertexDeclaration : public Base
		{
		public:
			RegistVertexDeclaration() : Base(REGISTVERTEXDECLARATION){}

			enum TYPE
			{
				TYPE_DEFAULT,
				TYPE_CUSTOM,
			};

			TYPE		Type;
			mstring		ResistID;	//!<	登録する名前
			SPBINARY	pCode;		//!<	登録するコード
		};

		class RegistPixelShader : public Base
		{
		public:
			RegistPixelShader() : Base(REGISTPIXELSHADER){}

			enum TYPE
			{
				TYPE_DEFAULT,
				TYPE_CUSTOM,
			};

			TYPE		CodeType;
			mstring		ResistID;	//!<	登録する名前
			SPBINARY	pCode;		//!<	登録するコード
		};



		class DeleteSurface : public Base
		{
		public:
			DeleteSurface() : Base(DELETESURFACE){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeleteTexture : public Base
		{
		public:
			DeleteTexture() : Base(DELETETEXTURE){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeleteIndex : public Base
		{
		public:
			DeleteIndex() : Base(DELETEINDEX){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeleteVertex : public Base
		{
		public:
			DeleteVertex() : Base(DELETEVERTEX){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeleteVertexDeclaration : public Base
		{
		public:
			DeleteVertexDeclaration() : Base(DELETEVERTEXDECLARATION){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeleteVertexShader : public Base
		{
		public:
			DeleteVertexShader() : Base(DELETEVERTEXSHADER){}

			mstring					DeleteID;	//!<	削除する名前
		};

		class DeletePixelShader : public Base
		{
		public:
			DeletePixelShader() : Base(DELETEPIXELSHADER){}

			mstring					DeleteID;	//!<	削除する名前
		};

	}


}


#endif
