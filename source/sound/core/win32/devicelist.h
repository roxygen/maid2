﻿#ifndef maid2_sound_core_win32_devicelist_h
#define maid2_sound_core_win32_devicelist_h

#include"../../../config/define.h"

#include"../idevicelist.h"
#include"../idevice.h"

#include"../../../framework/win32/dllwrapper.h"
#include"../../../framework/win32/window.h"

#include"directsound8/common.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {

  class DeviceList : public IDeviceList
	{
	public:
    DeviceList( Window& window );
		void Initialize();
    void GetList( std::vector<INFO>& info );
		SPDEVICE CreateDevice( int DeviceID );

  private:
    Window&  m_Window;

    static BOOL CALLBACK DirectSoundEnumCallback( LPGUID pGUID, LPCWSTR strDesc, LPCWSTR strDrvName, LPVOID pContext );

  private:
    DllWrapper        m_dsoundDLL;

    struct DIRECTSOUND8DEVICE
    {
      boost::shared_ptr<GUID>    pGUID;
	    String  Desc;
	    String  Driver;
    };
    std::vector<DIRECTSOUND8DEVICE> m_DirectSound8Device;
  };


}}


#endif
