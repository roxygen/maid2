#ifndef maid2_input_core_charcodedevicewindows_h
#define maid2_input_core_charcodedevicewindows_h

#include"../../../config/define.h"
#include"../icharcodedevice.h"

#include"../../../frameWork/win32/messagehook.h"
#include"../../../framework/win32/window.h"


namespace Maid
{

  class CharCodeDeviceWindows 
    : public ICharCodeDevice
    ,public MessageHook
  {
  public:
		CharCodeDeviceWindows( const Window& hWnd );
    virtual void Initialize();
    virtual void Finalize();
    virtual void Flush( String& buffer );
		virtual RETURNCODE OnMessage( WindowsMessage& mess );

	private:
		std::string	m_Buffer;
    const Window& m_Window;
  };


}

#endif
