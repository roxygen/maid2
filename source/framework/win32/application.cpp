#include"application.h"
#include"osdriverwin32.h"


namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���s
/*!
 	  @param	hInstance		  [i ]	WinMain() ����n���ꂽ����
 	  @param	hPrevInstance	[i ]	WinMain() ����n���ꂽ����
 	  @param	lpCmdLine		  [i ]	WinMain() ����n���ꂽ����
 	  @param	nCmdShow		  [i ]	WinMain() ����n���ꂽ����
 */
int Application::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  SetOSDriver( SPOSDRIVER(new OSDriverWin32(hInstance,hPrevInstance,lpCmdLine,nCmdShow)) );

  return ApplicationBase::Run();
}

}