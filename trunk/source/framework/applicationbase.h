
/*!
	  @file
	  @brief アプリケーションの雛形
 */

#ifndef framework_applicationbase_h
#define framework_applicationbase_h


#include"../config/define.h"
#include"osdriver.h"

namespace Maid
{
  class ApplicationBase
	{
	public:
    ApplicationBase();
    virtual ~ApplicationBase();

    int Run();

		void OnExit( int ExitCode );

  protected:
		virtual void OnSetup()=0;
		virtual void OnLoop()=0;
		virtual void OnCleanup()=0;

    void SetOSDriver( const SPOSDRIVER& pDriver );

  private:
    SPOSDRIVER  m_pOSDriver;
    int m_ExitCode;
	};
}

#endif
