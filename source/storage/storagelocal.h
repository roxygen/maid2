﻿/*!	
	@file
	@brief	Storageクラス内でのみ使用しているオブジェクト
*/

#ifndef maid2_storage_storagelocal_h
#define maid2_storage_storagelocal_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"fileio/filewrite.h"
#include"fileio/ifileread.h"
#include"storagemessagebase.h"

namespace Maid
{
  class StorageObjectFileWriter : public IStorageObject
  {
  public:
    FileWrite Writer;
  };

  class StorageObjectFileReader : public IStorageObject
  {
  public:
    SPFILEREAD pReader;
  };

}


#endif
