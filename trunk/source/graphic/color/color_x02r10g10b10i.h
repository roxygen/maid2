#ifndef maid2_graphic_color_color_x02r10g10b10i_h
#define maid2_graphic_color_color_x02r10g10b10i_h

/*!
 	  @file
 	  @brief	�P�s�N�Z��������킷�N���X
\n          X02R10G10B10 �t�H�[�}�b�g Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_X02R10G10B10I color_x02r10g10b10i.h
	 */
	class COLOR_X02R10G10B10I
	{
	public:
		COLOR_X02R10G10B10I(){}	//!<	�R���X�g���N�^
		COLOR_X02R10G10B10I( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	�R���X�g���N�^
		template <class COLOR>
		COLOR_X02R10G10B10I( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	�R���X�g���N�^


		unt16 GetA() const	{ return 0xFFFF;	}	//!<	�A���t�@�����̎擾
		unt16 GetR() const	{ return BITCONVERT10Ito16I((m_XRGB&0x3FF00000)>>20);	}	//!<	�Ԑ����̎擾
		unt16 GetG() const	{ return BITCONVERT10Ito16I((m_XRGB&0x000FFC00)>>10);	}	//!<	�ΐ����̎擾
		unt16 GetB() const	{ return BITCONVERT10Ito16I((m_XRGB&0x000003FF)>> 0);	}	//!<	�����̎擾

		operator unt32() const { return *((unt32*)&m_Blue); }	//!<	�ꊇ�ϊ�

		void SetR( unt16 r )	{ m_ARGB = (m_ARGB&~0x3FF00000)|(BITCONVERT16Ito10I(a)<<20);	}	//!<	�Ԑ����̐ݒ�
		void SetG( unt16 g )	{ m_ARGB = (m_ARGB&~0x000FFC00)|(BITCONVERT16Ito10I(a)<<10);	}	//!<	�ΐ����̐ݒ�
		void SetB( unt16 b )	{ m_ARGB = (m_ARGB&~0x000003FF)|(BITCONVERT16Ito10I(a)<< 0);	}	//!<	�����̐ݒ�

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	�������ɐݒ�
		{
      m_XRGB = ~0;
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	�Ⴄ�s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		template <class PIXEL>
		COLOR_A02R10G10B10I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(),src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	�����s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		COLOR_A02R10G10B10I& operator = (const COLOR_A02R10G10B10I& src)
		{
			m_XRGB = src.m_XRGB;
			return *this;
		}
	private:
		unt32 m_XRGB;
	};
}

#endif
