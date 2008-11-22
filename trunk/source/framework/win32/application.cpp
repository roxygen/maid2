#include"application.h"


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
  m_OSDevice.SetParam( hInstance,hPrevInstance,lpCmdLine,nCmdShow );

  return IApplication::Run();
}

IOSDevice& Application::GetOSDevice() { return m_OSDevice; }

}