/*!
    @file
    @brief Windows�p�t���[�����[�N�x�[�X�N���X
 */

#ifndef framework_win32_application32_h
#define framework_win32_application32_h

#include"../../config/define.h"
#include"../../config/Win32.h"
#include"../../auxiliary/globalpointer.h"
#include"../../auxiliary/string.h"

#include"../application.h"

#include"messagehookmanager.h"
#include<vector>

namespace Maid
{
  class Application : public _Application,  public GlobalPointer<Application>
	{
	public:
		Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
		virtual ~Application();


		HINSTANCE		GetHINSTANCE()	const;
		const String	GetClassName()	const;
		const String&	GetCmdLineALL()	const;
		const std::vector<String>&	GetCmdLine()const;
		String	GetCmdLine( int no )const;


	protected:
		virtual void LocalExit();
		virtual void OnSetup();
		virtual void OnLoop();
		virtual void OnCleanup();
		virtual bool IsValid();

	private:
		static LRESULT CALLBACK ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		HINSTANCE	m_Instance;
		HINSTANCE	m_PrevInstance;
		String		m_CommandLine;
		int			  m_CommandShow;

		std::vector<String>	m_DivComdList;	//	�������ꂽ�R�}���h���C��

		MessageHookManager	m_HookManager;
	};
}
#endif
