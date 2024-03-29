﻿#include"compressrle.h"
#include"../debug/warning.h"
#include"../debug/assert.h"

#include<memory.h>

namespace Maid
{

/*!
 	@class	CompressRLE compressrle.h
 	@brief	Run Length Encording による圧縮・展開アルゴリズムルーチン
\n        8bit単位で圧縮してます
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! データを圧縮する
/*!
 	引数などは ICompress::Encode() を参照すること
 */
size_t CompressRLE::Encode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const
{
	MAID_ASSERT( true, "未実装です" );

	return 0;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! データを展開する
/*!
 	引数などは ICompress::Decode() を参照すること
 */
size_t CompressRLE::Decode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const
{
	unt DecSize = 0;

	const unt08* s = (const unt08*)pSrc;
	unt08* d = (unt08*)pDst;

	while( true )
	{
		if( SrcLenght==0 ) { break; }

		unt08 flag = *s; ++s; --SrcLenght;
		
		if( flag==0x80 ) 
		{
			continue; 
		}

		if( IsBitOn(flag,7) )
		{
			const unt08 loop = (~flag+1) + 1;

			::memset( d, *(s), loop );
			d += loop;
			s += 1;
			SrcLenght -= 1;
		}else
		{
			::memcpy( d, s, flag+1 );
			d += flag+1;
			s += flag+1;
			SrcLenght -= flag+1;
		}
	}

	return DecSize;
}

}
