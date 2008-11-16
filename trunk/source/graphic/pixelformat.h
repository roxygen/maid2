#ifndef maid2_graphic_pixelformat_h
#define maid2_graphic_pixelformat_h

/*!
    @file
    @brief  �e��s�N�Z���t�H�[�}�b�g�Ŏg�������Ȋ֐�
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"


namespace Maid
{
  /*!
    @enum	PIXELFORMAT pixelformat.h
    @brief	�T�[�t�F�X�̃s�N�Z���t�H�[�}�b�g��\��enum
   */
  enum PIXELFORMAT
  {
    PIXELFORMAT_NONE,	//!<	�s���@��T�̓G���[


    //  unt 08bit/pixel
    PIXELFORMAT_P08X08R08G08B08I,
    PIXELFORMAT_P08A08R08G08B08I,

    //  unt 16bit/pixel
    PIXELFORMAT_R05G06B05I,
    PIXELFORMAT_X01R05G05B05I,
    PIXELFORMAT_A01R05G05B05I,
    PIXELFORMAT_A01B05G05R05I,
    PIXELFORMAT_A04R04G04B04I,

    //  unt 24bit/pixel
    PIXELFORMAT_R08G08B08I,

    //  unt 32bit/pixel
    PIXELFORMAT_A08R08G08B08I,
    PIXELFORMAT_X08R08G08B08I,
    PIXELFORMAT_A02R10G10B10I,
    PIXELFORMAT_A02B10G10R10I,

    //  unt 64bit/pixel
    PIXELFORMAT_A16B16G16R16I,

    //  float 64bit/pixel
    PIXELFORMAT_A16B16G16R16F,

    //  float 128bit/pixel
    PIXELFORMAT_A32B32G32R32F,


    // �J���[�t�H�[�}�b�g���������ꍇ�͂����ɒǉ�
    //	���� GetPixelBPP() �ɂ��ǉ����Ă�������






    PIXELFORMAT_DXT1,	//!<	DirectX Texture Compression Format 1
    PIXELFORMAT_DXT2,	//!<	DirectX Texture Compression Format 2
    PIXELFORMAT_DXT3,	//!<	DirectX Texture Compression Format 3
    PIXELFORMAT_DXT4,	//!<	DirectX Texture Compression Format 4
    PIXELFORMAT_DXT5,	//!<	DirectX Texture Compression Format 5

    // ���k�J���[�t�H�[�}�b�g���������ꍇ�͂����ɒǉ�
    //	���� GetPixelBPP() �ɂ��ǉ����Ă�������




    PIXELFORMAT_D16,      //!<	16bit/Pixel �� ZBuffer
    PIXELFORMAT_D32,      //!<	32bit/Pixel �� ZBuffer
    PIXELFORMAT_D24X8,    //!<	32bit/Pixel (�L���͈�24Bit) �� ZBuffer
    PIXELFORMAT_D24S8,    //!<	32bit/Pixel �̂����A 24Bit�� ZBuffer 8Bit �̃X�e���V���o�b�t�@

    // �[�x�t�H�[�}�b�g���������ꍇ�͂����ɒǉ�
    //	���� GetPixelBPP() �ɂ��ǉ����Ă�������


  };

  /*!
    @brief	�s�N�Z���t�H�[�}�b�g����P�s�N�Z��������Ɏg�p����
  \n			�r�b�g�������߂�
   
    @param	Format	[i ]	���߂�t�H�[�}�b�g
   
    @return	���߂��r�b�g��
   */
  inline int GetPixelBPP( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_NONE:			{ return  0; }break;

    case PIXELFORMAT_P08X08R08G08B08I:	{ return  8; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  8; }break;

    case PIXELFORMAT_R05G06B05I:      { return 16; }break;
    case PIXELFORMAT_A04R04G04B04I:		{ return 16; }break;
    case PIXELFORMAT_X01R05G05B05I:		{ return 16; }break;
    case PIXELFORMAT_A01R05G05B05I:		{ return 16; }break;
    case PIXELFORMAT_A01B05G05R05I:		{ return 16; }break;

    case PIXELFORMAT_R08G08B08I:      { return 24; }break;

    case PIXELFORMAT_A08R08G08B08I:		{ return 32; }break;
    case PIXELFORMAT_X08R08G08B08I:		{ return 32; }break;
    case PIXELFORMAT_A02R10G10B10I:		{ return 32; }break;
    case PIXELFORMAT_A02B10G10R10I:		{ return 32; }break;

    case PIXELFORMAT_A16B16G16R16I:		{ return 64; }break;

    case PIXELFORMAT_A16B16G16R16F:		{ return 64; }break;

    case PIXELFORMAT_A32B32G32R32F:		{ return 128; }break;


    case PIXELFORMAT_DXT1:			{ return  4; }break;
    case PIXELFORMAT_DXT2:			{ return  8; }break;
    case PIXELFORMAT_DXT3:			{ return  8; }break;
    case PIXELFORMAT_DXT4:			{ return  8; }break;
    case PIXELFORMAT_DXT5:			{ return  8; }break;

    case PIXELFORMAT_D16:       { return 16; }break;
    case PIXELFORMAT_D32:       { return 32; }break;
    case PIXELFORMAT_D24X8:			{ return 32; }break;
    case PIXELFORMAT_D24S8:			{ return 32; }break;

    }
    MAID_ASSERT( true, "�s���ȃt�H�[�}�b�g�ł�" );
    return 0;
  }

  /*!
    @brief	�s�N�Z���t�H�[�}�b�g����CLUT�P�����肠����Ɏg�p����
  \n        �r�b�g�������߂�
   
    @param	Format	[i ]	���߂�t�H�[�}�b�g
   
    @return	���߂��r�b�g��
   */
  inline int GetCLUTBPP( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_P08X08R08G08B08I:	{ return  32; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  32; }break;
    }
    return 0;
  }

  /*!
    @brief	�s�N�Z���t�H�[�}�b�g����p���b�g�̃C���f�b�N�X�������߂�
   
    @param	Format	[i ]	���߂�t�H�[�}�b�g
   
    @return	�C���f�b�N�X��
   */
  inline int GetCLUTLength( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_P08X08R08G08B08I:	{ return  256; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  256; }break;
    }
    return 0;
  }

  /*!
    @brief	���k�e�N�X�`�������ׂ�
   
    @param	Format	[i ]	���ׂ�t�H�[�}�b�g
   
    @return	���k�e�N�X�`���ł������� true
   */
  inline bool IsCompressSurfaceBuffer( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_DXT1:
    case PIXELFORMAT_DXT2:
    case PIXELFORMAT_DXT3:
    case PIXELFORMAT_DXT4:
    case PIXELFORMAT_DXT5:
      { return true; }break;
    }

    return false;
  }

  /*!
    @brief	���s�N�Z�����܂ނ����ׂ�
   
    @param	Format	[i ]	���ׂ�t�H�[�}�b�g
   
    @return	���s�N�Z��������Ȃ� true
   */
  inline bool IsAlphaPixel( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_A04R04G04B04I:
    case PIXELFORMAT_A01R05G05B05I:
    case PIXELFORMAT_A08R08G08B08I:
    case PIXELFORMAT_A02R10G10B10I:
    case PIXELFORMAT_A02B10G10R10I:
    case PIXELFORMAT_A16B16G16R16I:
    case PIXELFORMAT_P08A08R08G08B08I:
    case PIXELFORMAT_DXT2:
    case PIXELFORMAT_DXT3:
    case PIXELFORMAT_DXT4:
    case PIXELFORMAT_DXT5: { return true; }
    }
    return false;
  }

}


#endif
