#ifndef maid2_graphics_color_color_a04r04g04b04i_h
#define maid2_graphics_color_color_a04r04g04b04i_h

/*!
    @file
    @brief	�P�s�N�Z��������킷�N���X
\n          A04R04G04B04 intger �t�H�[�}�b�g Ver.
 */

#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A08R08G08B08I color_a04r04g04b04i.h
	 */
	class COLOR_A04R04G04B04I
	{
	public:
		COLOR_A04R04G04B04I(){}	//!<	�R���X�g���N�^
		COLOR_A04R04G04B04I( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	�R���X�g���N�^
		template <class COLOR>
		COLOR_A04R04G04B04I( const COLOR& Col ){ SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() ); }	//!<	�R���X�g���N�^

		unt16 GetA() const	{ return BITCONVERT04Ito16I((m_ARGB&0xF000)>>12);	}	//!<	�A���t�@�����̎擾
		unt16 GetR() const	{ return BITCONVERT04Ito16I((m_ARGB&0x0F00)>> 8);	}	//!<	�Ԑ����̎擾
		unt16 GetG() const	{ return BITCONVERT04Ito16I((m_ARGB&0x00F0)>> 4);	}	//!<	�ΐ����̎擾
		unt16 GetB() const	{ return BITCONVERT04Ito16I((m_ARGB&0x000F)>> 0);	}	//!<	�����̎擾

		operator unt16() const { return m_ARGB; }	//!<	�ꊇ�ϊ�

		void SetA( unt16 a )	{ m_ARGB = (m_ARGB&~0xF000)|(BITCONVERT16Ito04I(a)<<12);	}	//!<	�A���t�@�����̐ݒ�
		void SetR( unt16 r )	{ m_ARGB = (m_ARGB&~0x0F00)|(BITCONVERT16Ito04I(r)<< 8);	}	//!<	�Ԑ����̐ݒ�
		void SetG( unt16 g )	{ m_ARGB = (m_ARGB&~0x00F0)|(BITCONVERT16Ito04I(g)<< 4);	}	//!<	�ΐ����̐ݒ�
		void SetB( unt16 b )	{ m_ARGB = (m_ARGB&~0x000F)|(BITCONVERT16Ito04I(b)<< 0);	}	//!<	�����̐ݒ�

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	�������ɐݒ�
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	�Ⴄ�s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		template <class PIXEL>
		COLOR_A04R04G04B04I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(), src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	�����s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		COLOR_A04R04G04B04I& operator = (const COLOR_A04R04G04B04I& src)
		{
			m_ARGB = src.m_ARGB;
			return *this;
		}
	private:
		unt16 m_ARGB;
	};
}

#endif
