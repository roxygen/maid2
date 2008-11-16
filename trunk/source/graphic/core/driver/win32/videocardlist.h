/*!
	@file
	@brief Win32用 VideoCard列挙ヘッダ
 */

#ifndef graphic_core_driver_win32_videocardlist_h
#define graphic_core_driver_win32_videocardlist_h


#include"../../../../config/define.h"
#include"../../../../config/win32.h"
#include"../../videocardlistbase.h"

namespace Maid
{
  class VideoCardList : public VideoCardListBase
  {
  public:
    int GetVideoCardCount();
    VIDEOCARDINFO GetVideoCardInfo( int no );
    SPVIDEOCARD CreateVideoCard( int No, const CREATEPARAM& Info );
  };
}



#endif
