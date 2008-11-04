#ifndef maid2_storage_storagemessage1_h
#define maid2_storage_storagemessage1_h


#include"storagemessage.h"



namespace Maid
{
  //	描画スレッドに投げる　メッセージクラス

  namespace StorageMessage
  {
		class FileWCreate : public Base
		{
		public:
			FileWCreate() : Base(FILE_W_CREATE){}

			String	FileName;	//!<	作成するファイル名
		};

		class FileWOpen : public Base
		{
		public:
			FileWOpen() : Base(FILE_W_OPEN){}

			String	FileName;	//!<	作成するファイル名
		};

		class FileWAdd : public Base
		{
		public:
			FileWAdd() : Base(FILE_W_ADD){}

			String	FileName;	//!<	作成するファイル名
		};

		class FileWWrite : public Base
		{
		public:
			FileWWrite() : Base(FILE_W_WRITE){}

      std::vector<unt08>  Buffer; //  書き込むデータ
		};

		class FileWSeek : public Base
		{
		public:
			FileWSeek() : Base(FILE_W_SEEK){}

      enum POSITION
      {
	      POSITION_BEGIN,		//!<	ファイルの先頭から
	      POSITION_END,		  //!<	ファイルの終端から
	      POSITION_CURRENT,	//!<	現在の位置から
      };

      POSITION  Pos;
      int       Size;      
		};

		class FileWClose : public Base
		{
		public:
			FileWClose() : Base(FILE_W_CLOSE){}
		};
  }
}



#endif
