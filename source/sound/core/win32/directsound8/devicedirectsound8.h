﻿#ifndef maid2_sound_core_win32_directsound8_devicedirectsound8_h
#define maid2_sound_core_win32_directsound8_devicedirectsound8_h

#include"../../../../config/define.h"
#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"

#include<boost/smart_ptr.hpp>
#include"../../idevice.h"
#include"common.h"

namespace Maid { namespace Sound {


  class DeviceDirectSound8 : public IDevice
	{
	public:
    DeviceDirectSound8( const boost::shared_ptr<GUID>& guid, const DllWrapper& dsounddll, const Window& window );

		void Initialize();
		void Finalize();
    void SetFormat( const PCMFORMAT& format );

    SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param );
    SPBUFFER DuplicateBuffer( const SPBUFFER& pSrc );

  private:
    void  PlayDummySound();
    bool  IsPriorityMode();

  private:
    const boost::shared_ptr<GUID>      m_pGUID;
    const DllWrapper& m_dsounddll;
    const Window&   m_Window;
    SPDIRECTSOUND8  m_pDevice;

    SPDIRECTSOUNDBUFFER m_pPrimary;
    SPDIRECTSOUNDBUFFER m_pDummy;
  };

}}


#endif
