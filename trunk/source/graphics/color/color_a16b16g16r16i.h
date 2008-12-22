#ifndef maid2_graphics_color_color_a16b16g16r16i_h
#define maid2_graphics_color_color_a16b16g16r16i_h

/*!
 	  @file
 	  @brief	�P�s�N�Z��������킷�N���X
\n          A16B16G16R16 �t�H�[�}�b�g Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A16B16G16R16I color_a16b16g16r16i.h
	 */
	class COLOR_A16B16G16R16I
	{
	public:
		COLOR_A16B16G16R16I(){}	//!<	�R���X�g���N�^
		COLOR_A16B16G16R16I( unt16 a, unt16 b, unt16 g, unt16 r ){ SetARGB( a, r, g, b ); }	//!<	�R���X�g���N�^
		template <class COLOR>
		COLOR_A16B16G16R16I( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	�R���X�g���N�^


		unt16 GetA() const	{ return m_Alpha;	}	//!<	�A���t�@�����̎擾
		unt16 GetR() const	{ return m_Red;		}	//!<	�Ԑ����̎擾
		unt16 GetG() const	{ return m_Green;	}	//!<	�ΐ����̎擾
		unt16 GetB() const	{ return m_Blue;	}	//!<	�����̎擾

		operator unt64() const { return *((unt64*)&m_Blue); }	//!<	�ꊇ�ϊ�

		void SetA( unt16 a )	{ m_Alpha = a;	}	//!<	�A���t�@�����̐ݒ�
		void SetR( unt16 r )	{ m_Red   = r;	}	//!<	�Ԑ����̐ݒ�
		void SetG( unt16 g )	{ m_Green = g;	}	//!<	�ΐ����̐ݒ�
		void SetB( unt16 b )	{ m_Blue  = b;	}	//!<	�����̐ݒ�

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	�������ɐݒ�
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	�Ⴄ�s�N�Z���t�H�[�}�b�g�Ƃ̃R�s�[
		template <class PIXEL>
		COLOR_A16B16G16R16I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(),src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

	private:
		unt16 m_Red;
		unt16 m_Green;
		unt16 m_Blue;
		unt16 m_Alpha;
	};
}

#endif
