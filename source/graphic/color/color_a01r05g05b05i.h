#ifndef maid2_graphic_color_color_a01r05g05b05i_h
#define maid2_graphic_color_color_a01r05g05b05i_h

/*!
    @file
    @brief	�P�s�N�Z��������킷�N���X
\n          A01R05G05B05 integer �t�H�[�}�b�g Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A08R08G08B08I color_a01r05g05b05i.h
	 */
  class COLOR_A01R05G05B05I
	{
	public:
		COLOR_A01R05G05B05I(){}	//!<	�R���X�g���N�^
		COLOR_A01R05G05B05I( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	�R���X�g���N�^
		template <class COLOR>
		COLOR_A01R05G05B05I( const COLOR& Col ){ SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() ); }	//!<	�R���X�g���N�^

		unt16 GetA() const	{ return BITCONVERT01Ito16I((m_ARGB&0x8000)>>15);	}	//!<	�A���t�@�����̎擾
		unt16 GetR() const	{ return BITCONVERT05Ito16I((m_ARGB&0x7C00)>>10);	}	//!<	�Ԑ����̎擾
		unt16 GetG() const	{ return BITCONVERT05Ito16I((m_ARGB&0x03E0)>> 5);	}	//!<	�ΐ����̎擾
		unt16 GetB() const	{ return BITCONVERT05Ito16I((m_ARGB&0x001F)<< 3);	}	//!<	�����̎擾

		operator unt16() const { return m_ARGB; }	//!<	�ꊇ�ϊ�

		void SetA( unt16 a )	{ m_ARGB = (m_ARGB&~0x8000)|(BITCONVERT16Ito01I(a)<<15);	}	//!<	�A���t�@�����̐ݒ�
		void SetR( unt16 r )	{ m_ARGB = (m_ARGB&~0x7C00)|(BITCONVERT16Ito05I(r)<<10);	}	//!<	�Ԑ����̐ݒ�
		void SetG( unt16 g )	{ m_ARGB = (m_ARGB&~0x03E0)|(BITCONVERT16Ito05I(g)<< 5);	}	//!<	�ΐ����̐ݒ�
		void SetB( unt16 b )	{ m_ARGB = (m_ARGB&~0x001F)|(BITCONVERT16Ito05I(b)<< 0);	}	//!<	�����̐ݒ�

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	�������ɐݒ�
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	�Ⴄ�s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		template <class PIXEL>
		COLOR_A01R05G05B05I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(), src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	�����s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		COLOR_A01R05G05B05I& operator = (const COLOR_A01R05G05B05I& src)
		{
			m_ARGB = src.m_ARGB;
			return *this;
		}

	private:
		unt16 m_ARGB;
	};
}

#endif
