/*!
 	@file 
 	@brief	読み込み専用のファイルインターフェース
 */

#ifndef maid2_storage_fileio_filereadinterface_h
#define maid2_storage_fileio_filereadinterface_h

#include"../../config/define.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
 	  @brief	読み込み専用のファイルインターフェース
  \n			各クラスはこのクラスを派生させて実装してあります	 
   */
  class FileRead
  {
  public:
    //! デストラクタ
    /*!
     */
    virtual ~FileRead() {}


    //! ファイルを読み込む
    /*!
      @param	pData	[ o]	読込先
      @param	Size	[i ]	読み込む長さ（バイト単位）

      @return	実際に読み込んだバイト数
     */
    virtual size_t Read( void* pData, size_t Size )=0;


    //! ファイルサイズの取得
    /*!
      @return	ファイルサイズ
     */
    virtual size_t		GetSize()    const =0;



    //! 現在のファイルポジションの取得
    /*!
      @return	ファイルポジション
     */
    virtual size_t		GetPosition()const =0;


    enum POSITION
    {
	    POSITION_BEGIN,		//!<	ファイルの先頭から
	    POSITION_END,		  //!<	ファイルの終端から
	    POSITION_CURRENT,	//!<	現在の位置から
    };

    //! ファイルポジションの移動
    /*!
      ファイル範囲を超える指定があった場合　範囲内になるようにクリップされる

      @param	Pos   [i ]	移動基準位置		 
      @param	Size	[i ]	移動量（バイト単位）
     */
    virtual void	Seek( POSITION Pos, int Size)=0;


    //! 開いているファイルを閉じる
    /*!
     */
    virtual void Close()=0;
  };

	typedef	boost::shared_ptr<FileRead>	SPFILEREAD;
}

#endif
