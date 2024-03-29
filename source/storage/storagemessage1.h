﻿#ifndef maid2_storage_storagemessage1_h
#define maid2_storage_storagemessage1_h


#include"storagemessagebase.h"



namespace Maid
{
  //  描画スレッドに投げる　メッセージクラス

  namespace StorageMessage
  {
    struct FileWCreate : public Base
    {
      FileWCreate() : Base(FILE_W_CREATE){}

      String  FileName; //!<  作成するファイル名
    };

    struct FileWOpen : public Base
    {
      FileWOpen() : Base(FILE_W_OPEN){}

      String  FileName; //!<  作成するファイル名
    };

    struct FileWAdd : public Base
    {
      FileWAdd() : Base(FILE_W_ADD){}

      String  FileName; //!<  作成するファイル名
    };

    struct FileWWrite : public Base
    {
      FileWWrite() : Base(FILE_W_WRITE){}

      std::vector<unt08>  Buffer; //  書き込むデータ
    };

    struct FileWSeek : public Base
    {
      FileWSeek() : Base(FILE_W_SEEK){}

      enum POSITION
      {
        POSITION_BEGIN,   //!<  ファイルの先頭から
        POSITION_END,     //!<  ファイルの終端から
        POSITION_CURRENT, //!<  現在の位置から
      };

      POSITION  Pos;
      int       Size;
    };

    struct FileWClose : public Base
    {
      FileWClose() : Base(FILE_W_CLOSE){}
    };
  }
}



#endif
