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
Application::Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
  :m_Device(hInstance,hPrevInstance,lpCmdLine,nCmdShow)
{
}


IOSDevice& Application::GetOSDevice() { return m_Device; }

}