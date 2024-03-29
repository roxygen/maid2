﻿#ifndef maid2_graphics_color_color_p08_h
#define maid2_graphics_color_color_p08_h

/*!
 	@file
 	@brief	１ピクセルをあらわすクラス郡
\n			８ビットインデックス Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"color_a08r08g08b08i.h"
#include"color_x08r08g08b08i.h"
#include<vector>

namespace Maid
{
	/*!
	 	@brief	８ビットインデックスピクセル
	\n			パレットは テンプレート化されています
	 */
	template <class CLUTCOLOR>
	class COLOR_P08
	{
	public:

		//!	パレット配列
//		typedef MySTL::vector<CLUTCOLOR> CLUT;

		COLOR_P08(){}	//!<	コンストラクタ
		COLOR_P08( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_P08( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ

		unt16 GetA() const	{ return GetSrc().GetA();	}	//!<	アルファ成分の取得
		unt16 GetR() const	{ return GetSrc().GetR();	}	//!<	赤成分の取得
		unt16 GetG() const	{ return GetSrc().GetG();	}	//!<	緑成分の取得
		unt16 GetB() const	{ return GetSrc().GetB();	}	//!<	青成分の取得

		operator unt32() const { return GetSrc(); }	//!<	一括変換

		void SetA( unt16 a )	{/*しばらくの間パス*/DEN_ASSERT( true, "未実装です" );}	//!<	アルファ成分の設定
		void SetR( unt16 r )	{/*しばらくの間パス*/DEN_ASSERT( true, "未実装です" );}	//!<	赤成分の設定
		void SetG( unt16 g )	{/*しばらくの間パス*/DEN_ASSERT( true, "未実装です" );}	//!<	緑成分の設定
		void SetB( unt16 b )	{/*しばらくの間パス*/DEN_ASSERT( true, "未実装です" );}	//!<	青成分の設定

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			/*しばらくの間パス*/
			MAID_ASSERT( true, "未実装です" );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_P08& operator = (const PIXEL &src)
		{	/*しばらくの間パス*/
			MAID_ASSERT( true, "未実装です" );
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_P08& operator = (const COLOR_P08& src)
		{	/*しばらくの間パス*/
			MAID_ASSERT( true, "未実装です" );
			return *this;
		}

		//!	転送元パレットの設定
		static void SetCLUTSrc( CLUTCOLOR* pPal ){ s_CLUTLIST_Src=pPal; }
		//!	転送先パレットの設定
		static void SetCLUTDst( CLUTCOLOR* pPal ){ s_CLUTLIST_Dst=pPal; }
	private:

		//!	転送元パレットの取得
		const CLUTCOLOR& GetSrc() const { return s_CLUTLIST_Src[m_Pixel]; }
		//!	転送先パレットの取得
		const CLUTCOLOR& GetDst() const { return s_CLUTLIST_Dst[m_Pixel]; }
		unt08 m_Pixel;

	private:
		static CLUTCOLOR* s_CLUTLIST_Src;
		static CLUTCOLOR* s_CLUTLIST_Dst;
	};

	typedef COLOR_P08<COLOR_A08R08G08B08I>	COLOR_P08A08R08G08B08I;
	typedef COLOR_P08<COLOR_X08R08G08B08I>	COLOR_P08X08R08G08B08I;
}

#endif
