﻿#ifndef maid2_graphics_color_color_r05g06b05i_h
#define maid2_graphics_color_color_r05g06b05i_h

/*!
    @file
    @brief	１ピクセルをあらわすクラス郡
\n          RGB565 フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
	 	@class	COLOR_R05G06B05I color_r05g06b05i.h
	 */
	class COLOR_R05G06B05I
	{
	public:
		COLOR_R05G06B05I(){}	//!<	コンストラクタ
		COLOR_R05G06B05I( unt16 r, unt16 g, unt16 b ){ SetRGB( r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_R05G06B05I( const COLOR& Col ){  SetRGB( Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ


		unt16 GetA() const	{ return 0xFFFF;		}								//!<	アルファ成分の取得
		unt16 GetR() const	{ return BITCONVERT05Ito16I((m_RGB&0xF800)>>11);	}	//!<	赤成分の取得
		unt16 GetG() const	{ return BITCONVERT06Ito16I((m_RGB&0x07E0)>> 5);	}	//!<	緑成分の取得
		unt16 GetB() const	{ return BITCONVERT05Ito16I((m_RGB&0x001F)>> 0);	}	//!<	青成分の取得

		operator unt16() const { return m_RGB; }	//!<	一括変換

		void SetA( unt16 a )	{ /*何もしない*/	}
		void SetR( unt16 r )	{ m_RGB = (m_RGB&~0xF800) | (BITCONVERT16Ito05I(r)<<11);	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_RGB = (m_RGB&~0x07E0) | (BITCONVERT16Ito06I(g)<< 5);	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_RGB = (m_RGB&~0x001F) | (BITCONVERT16Ito05I(b)<< 0);	}	//!<	青成分の設定

		void SetRGB( unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetR( r );
			SetG( g );
			SetB( b );
		}

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_R05G06B05I& operator = (const PIXEL &src)
		{
			SetRGB(src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_R05G06B05I& operator = (const COLOR_R05G06B05I& src)
		{
			m_RGB = src.m_RGB;
			return *this;
		}
	private:
		unt16 m_RGB;
	};
}

#endif
