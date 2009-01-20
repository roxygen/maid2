#ifndef maid2_sound_decoder_ipcmdecoder_h
#define maid2_sound_decoder_ipcmdecoder_h 


#include"../../config/define.h"
#include"../common.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
      @class  IPCMDecoder ipcmdecoder.h
      @brief  各種データからＰＣＭデータに変換させる基本クラス
  */
  class IPCMDecoder
  {
  public:
    virtual ~IPCMDecoder(){};

    enum OPENRESULT
    {
      OPENRESULT_OK =0,
      OPENRESULT_ERROR,
    };

    //! データを開く
    /*!
 	      @param	pSrc  [i ]	データの先頭アドレス
 	      @param	size	[i ]	ロックする長さ（バイト単位）
     */
    virtual OPENRESULT Open( const void* pSrc, size_t size )=0;

    //! データを閉じる
    /*!
     */
    virtual void Close()=0;

    //! 指定した分だけデコードする
    /*!
 	      @param	pDst  [i ]	書き込み先
 	      @param	size	[i ]	書き込む長さ
     */
    virtual size_t  Read( void* pDst, size_t size )=0;


    //! デコード開始位置を変更する
    /*!
        @param  Offset  [i ]  開始位置（バイト単位）
     */
    virtual void    SetPosition( size_t Offset )=0;

    //! 現在の位置を取得する
    /*!
        @return  位置（バイト単位）
     */
    virtual size_t  GetPosition()		const=0;

    //! デコードできる長さを取得する
    /*!
        @return  長さ（バイト単位）
     */
    virtual size_t  GetLength()			const=0;

    //! 設定されているデータフォーマットを調べる
    /*!
        @return  フォーマット
     */
    virtual PCMFORMAT GetFormat() const=0;
  };

  typedef	boost::shared_ptr<IPCMDecoder>	SPPCMDECODER;

}

#endif