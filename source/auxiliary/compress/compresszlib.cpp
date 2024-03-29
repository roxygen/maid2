﻿#include"compresszlib.h"
#include"../debug/warning.h"

#include<zlib.h>
#include<zconf.h>

namespace Maid
{

/*!
    @class	CCompressZlib CCompressZlib.h
    @brief	Zlib による圧縮・展開アルゴリズムルーチン
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! データを圧縮する
/*!
 	引数などは ICompress::Encode() を参照すること
 */
size_t CompressZLIB::Encode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const
{
	z_stream	Zstream;
	ZERO( &Zstream, sizeof(Zstream) ); 

	if( deflateInit(&Zstream, Z_DEFAULT_COMPRESSION)!=Z_OK ) 
  {
    MAID_WARNING( "deflateInit()" );
    return FUNCERROR;
  }


	Zstream.next_in   = (unt08*)pSrc;
	Zstream.avail_in  = SrcLenght;
	Zstream.next_out  = (unt08*)pDst;
	Zstream.avail_out = DstLength;

	const int ret = ::deflate(&Zstream, Z_FINISH);
	if( ret!=Z_STREAM_END )
	{
    MAID_WARNING( "deflate()" );
		deflateEnd(&Zstream);
		return FUNCERROR;
	}


	const unt EncSize = Zstream.total_out;	//圧縮後のサイズ

	deflateEnd(&Zstream);

	return EncSize;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! データを展開する
/*!
 	引数などは ICompress::Decode() を参照すること
 */
size_t CompressZLIB::Decode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const
{
	z_stream	Zstream;
	ZERO( &Zstream, sizeof(Zstream) ); 

	if( inflateInit(&Zstream)!=Z_OK )
  {
    MAID_WARNING( "inflateInit()" );
    return FUNCERROR;
  }

	Zstream.next_in   = (unt08*)pSrc;
	Zstream.avail_in  = SrcLenght;
	Zstream.next_out  = (unt08*)pDst;
	Zstream.avail_out = DstLength;

	if( ::inflate(&Zstream, Z_FINISH)!=Z_STREAM_END )
	{
    MAID_WARNING( "inflate()" );
		inflateEnd(&Zstream);
    return FUNCERROR;
	}
	const unt DecSize = Zstream.total_out;	//展開後のサイズ

	inflateEnd(&Zstream);

	return DecSize;
}

}
