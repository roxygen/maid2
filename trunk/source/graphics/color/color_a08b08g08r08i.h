#ifndef maid2_graphics_color_color_a08b08g08r08i_h
#define maid2_graphics_color_color_a08b08g08r08i_h

/*!
 	  @file
 	  @brief	�P�s�N�Z��������킷�N���X
\n          ABGR8888 �t�H�[�}�b�g Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A08B08G08R08I color_a08b08g08r08i.h
	 */
	class COLOR_A08B08G08R08I
	{
	public:
		COLOR_A08B08G08R08I(){}	//!<	�R���X�g���N�^
		COLOR_A08B08G08R08I( unt16 a, unt16 b, unt16 g, unt16 r ){ SetARGB( a, r, g, b ); }	//!<	�R���X�g���N�^
		template <class COLOR>
		COLOR_A08B08G08R08I( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	�R���X�g���N�^


		unt16 GetA() const	{ return BITCONVERT08Ito16I(m_Alpha);	}	//!<	�A���t�@�����̎擾
		unt16 GetR() const	{ return BITCONVERT08Ito16I(m_Red);		}	//!<	�Ԑ����̎擾
		unt16 GetG() const	{ return BITCONVERT08Ito16I(m_Green);	}	//!<	�ΐ����̎擾
		unt16 GetB() const	{ return BITCONVERT08Ito16I(m_Blue);	}	//!<	�����̎擾

		operator unt32() const { return *((unt32*)&m_Blue); }	//!<	�ꊇ�ϊ�

		void SetA( unt16 a )	{ m_Alpha = BITCONVERT16Ito08I(a);	}	//!<	�A���t�@�����̐ݒ�
		void SetR( unt16 r )	{ m_Red   = BITCONVERT16Ito08I(r);	}	//!<	�Ԑ����̐ݒ�
		void SetG( unt16 g )	{ m_Green = BITCONVERT16Ito08I(g);	}	//!<	�ΐ����̐ݒ�
		void SetB( unt16 b )	{ m_Blue  = BITCONVERT16Ito08I(b);	}	//!<	�����̐ݒ�

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	�������ɐݒ�
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	�Ⴄ�s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		template <class PIXEL>
		COLOR_A08B08G08R08I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(),src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	�����s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		COLOR_A08B08G08R08I& operator = (const COLOR_A08B08G08R08I& src)
		{
			*(unt32*)(&m_Red) = *(unt32*)(&src.m_Red);
			return *this;
		}
	private:
		unt08 m_Red;
		unt08 m_Green;
		unt08 m_Blue;
		unt08 m_Alpha;
	};
}

#endif
