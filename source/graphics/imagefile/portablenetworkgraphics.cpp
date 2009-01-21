#include"portablenetworkgraphics.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../storage/fileio/filereadmemory.h"

#include<png.h>
#include<vector>


//!	PNG �ǂݍ��ݗp�\����
struct TPngFileBuffer
{
  Maid::SPFILEREAD		pFile;
};

static void PngReadFunc( png_struct *Png, png_bytep buf, png_size_t size )
{
  TPngFileBuffer* PngFileBuffer = (TPngFileBuffer*)png_get_io_ptr(Png);

  PngFileBuffer->pFile->Read( buf, size );
}

namespace Maid
{
	namespace PNG
	{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �w�肵���f�[�^��PNG�����ׂ�
/*!
    @param	FileImage	[i ]  �f�[�^

    @return �r�b�g�}�b�v�Ȃ�true
            �Ⴄ�Ȃ�false
*/
bool  Check( const std::vector<unt08>& FileImage )
{
  const unt08 head[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, };

  if( FileImage.size()<NUMELEMENTS(head) ) { return false; }

  return ::memcmp( &(FileImage[0]), head, NUMELEMENTS(head) )==0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! PNG�t�@�C����W�J����
/*!
	@param	FileImage	[i ]	�t�@�C���C���[�W
	@param	dst 			[ o]	�s�N�Z���f�[�^
 */
void  Load( const std::vector<unt08>& FileImage, Surface& dst )
{
	png_struct*	pStruct = NULL;
	png_info*	pInfo   = NULL;

	pStruct = ::png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	pInfo	= ::png_create_info_struct(pStruct);

  boost::shared_ptr<FileReadMemory> pRead( new FileReadMemory );

	pRead->OpenNoOwner( &(FileImage[0]), FileImage.size() );

	TPngFileBuffer	buf;
	buf.pFile = pRead;

	::png_set_read_fn( pStruct, (png_voidp)&buf, (png_rw_ptr)PngReadFunc );

	::png_read_info( pStruct, pInfo );
	png_uint_32 Width,Height;
	int BitDepth,ColorType;
	int InterlaceType;
	int CompressionType;
	int FilterType;
	::png_get_IHDR(pStruct,pInfo,&Width,&Height,&BitDepth,&ColorType,&InterlaceType,&CompressionType,&FilterType);

	if( BitDepth!=8 )	// <- ����P�J���[�i�ԂȂǁj�̃r�b�g���ł�
  {
    MAID_WARNING( MAIDTEXT("BitDepth!=8") );
    return ;
  }
	if( InterlaceType!=PNG_INTERLACE_NONE ) //	�C���^�[���[�X���m���
  {
    MAID_WARNING( MAIDTEXT("PNG_INTERLACE_NONE") );
    return ;
  }

	const bool UsePalette = IsFlag(ColorType,0x01);
	const bool UseColor   = IsFlag(ColorType,0x02);
	const bool UseAlpha   = IsFlag(ColorType,0x04);

	if( !UseColor ) 	//	�O���[�X�P�[���Ȃ�Ēm���
  {
    MAID_WARNING( MAIDTEXT("!UseColor") );
  }

	PIXELFORMAT fmt;

//	if( UsePalette ){ fmt = PIXELFORMAT_P08X08R08G08B08I; }	//	�p���b�g������Ȃ�W�a�h�s
	if( UsePalette ){ MAID_ASSERT( true, "�ł����Ή�" );  }
	ef( UseAlpha   ){ fmt = PIXELFORMAT_A08R08G08B08I;	  }	//	�p���b�g�����{�A���t�@����Ȃ�R�Q�a�h�s
	else			{ fmt = PIXELFORMAT_R08G08B08I;		  }	//	����ȊO�͂Q�S�a�h�s

	{
		::png_set_bgr( pStruct );

		png_colorp Palette;
		int PaletteCount=0;
		if( UsePalette ) { png_get_PLTE( pStruct, pInfo, &Palette, &PaletteCount ); }

		dst.Create( SIZE2DI(Width,Height), fmt);

		const int number_passes = png_set_interlace_handling(pStruct);

		std::vector<png_bytep> LineHeadPtr(Height);

		for( int y=0; y<(int)LineHeadPtr.size(); ++y )
		{
			LineHeadPtr[y] = (png_bytep)dst.GetLinePTR(y);
		}

		for (int pass = 0; pass < number_passes; ++pass )
		{
			for (png_uint_32 y = 0; y < Height; ++y)
			{
				png_read_rows(pStruct, &LineHeadPtr[y], png_bytepp_NULL, 1);
			}
		}

		::png_destroy_read_struct( &pStruct, &pInfo, NULL );
	}
}


	}
}