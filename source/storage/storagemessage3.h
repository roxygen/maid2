#ifndef maid2_storage_storagemessage3_h
#define maid2_storage_storagemessage3_h


#include"storagemessagebase.h"



namespace Maid
{
  //	描画スレッドに投げる　メッセージクラス

  namespace StorageMessage
  {
    struct Function : public Base
    {
	    Function() : Base(FUNCTION_EXECUTE){}
    };
  }
}



#endif
