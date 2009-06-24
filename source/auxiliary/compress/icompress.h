﻿/*!
 	@file 
 	@brief	圧縮・展開アルゴリズムルーチン
 */

#ifndef maid2_auxiliary_compress_icompress_h
#define maid2_auxiliary_compress_icompress_h

#include"../../config/define.h"
#include"../../config/typedef.h"

namespace Maid
{
  /*!
    @class  ICompress icompress.h
    @brief  メモリ上にあるデータを圧縮・展開するクラス
  \n			  このクラスは基本的に圧縮・展開アルゴリズムだけが記述されているので
  \n			  オブジェクトとして持つことはありません
  \n        少しずつ圧縮・展開は今のところできてません。そうしたほうがいいのかな？
   */
  class ICompress
  {
  public:
    virtual ~ICompress(){};

    enum
    {
      FUNCERROR = ~0,   //!<  関数の処理が失敗したらこれが戻って来る
    };

    //! データを圧縮する
    /*!
      @param	pSrc      [i ]	圧縮するデータ
      @param	SrcLenght [i ]	pSrcの長さ（バイト単位）
      @param	pDst      [i ]	圧縮したデータの出力先( 適切に確保しておくこと）
      @param	DstLength	[i ]	pDstの長さ（バイト単位）

      @return	圧縮後の長さ（バイト単位）	 
     */
    virtual size_t Encode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const=0;


    //! データを展開する
    /*!
      @param	pSrc      [i ]	展開するデータ
      @param	SrcLenght	[i ]	pSrcの長さ（バイト単位）
      @param	pDst      [i ]	展開したデータの出力先( 適切に確保しておくこと）
      @param	DstLength	[i ]	pDstの長さ（バイト単位）

      @return	展開後の長さ（バイト単位）
     */
    virtual size_t Decode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const=0;
  };
}
#endif
