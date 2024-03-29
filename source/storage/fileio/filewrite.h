﻿/*!
 	@file
 	@brief	ファイルにたいして書き込みのみのクラス
 
 */

#ifndef maid2_storage_fileio_filewrite_h
#define maid2_storage_fileio_filewrite_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include<stdio.h>

namespace Maid
{
  class FileWrite
  {
  public:
    FileWrite();
    ~FileWrite();

    enum OPENOPTION
    {
      OPENOPTION_NEW=0, //!<  ファイルサイズを０にして作成
      OPENOPTION_OPEN,  //!<  普通にオープン
      OPENOPTION_ADD,   //!<  追加書き込み（ファイルが存在しないときは新規に作る）
    };

    enum OPENRESULT
    {
      OPENRESULT_OK =0,
      OPENRESULT_ERROR,
    };

    OPENRESULT    Open( const String& FileName, OPENOPTION eOption );
    size_t  Write( const void* pData, size_t DataSize );
    void    Close();

    enum POSITION
    {
      POSITION_BEGIN,   //!<  ファイルの先頭から
      POSITION_END,     //!<  ファイルの終端から
      POSITION_CURRENT, //!<  現在の位置から
    };

    void	Seek( POSITION ePos, int Size );

    size_t  GetSize()    const;
    size_t  GetPosition()const;

  private:
    FILE*   m_Handle; //!<  ファイルハンドル
  };
}

#endif
