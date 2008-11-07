#include"application.h"

#include"shell.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include<shellapi.h>

namespace Maid
{

static const wchar_t* DEFAULT_CLASSNAME = L"Maid_application";

Application* GlobalPointer<Application>::s_pPointer;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 */
Application::Application()
{
	Shell::CoInitialize();
	Shell::InitCommonControls();
	m_ApplicationExitCode = 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
Application::~Application()
{
	Shell::CoUninitialize();
}

HINSTANCE		Application::GetHINSTANCE()	const { return m_Instance; }
const String	Application::GetClassName()	const { return String::ConvertUNICODEtoMAID(DEFAULT_CLASSNAME); }
const String&	Application::GetCmdLineALL()	const { return m_CommandLine; }
const std::vector<String>&	Application::GetCmdLine()const { return m_DivComdList; }

String	Application::GetCmdLine( int no )const
{
	if( !(no<(int)m_DivComdList.size()) ) { return String(); }

	return m_DivComdList[no];
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �A�v���������Ă��邩�H
/*!
	�ڂ����� IApplication::IsValid() ���Q��
 */
bool Application::IsValid()
{
	MSG Msg;

	while( ::PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE )!=0 )
	{
		if( Msg.message == WM_QUIT )
		{
			m_ApplicationExitCode = (int)Msg.wParam;
			return false;	
		}
		::TranslateMessage( &Msg );
		::DispatchMessage( &Msg );
	}

	return true;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �A�v���P�[�V�������I��������
/*!
	�ڂ����� IApplication::ApplicationExit() ���Q��
 */
void Application::ApplicationExit()
{
	::PostQuitMessage(0);
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �A�v���P�[�V�����̋N��
/*!
 	@param	hInstance		[i ]	WinMain() ����n���ꂽ����
 	@param	hPrevInstance	[i ]	WinMain() ����n���ꂽ����
 	@param	lpCmdLine		[i ]	WinMain() ����n���ꂽ����
 	@param	nCmdShow		[i ]	WinMain() ����n���ꂽ����

	@return �A�v���P�[�V�����̏I���R�[�h WinMain �̕Ԃ�l�Ƃ��Ďg���܂�

	@exception CException �ُ�I�������ꍇ

 */

int Application::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  Set();
	try
	{
		{	//	�����ݒ�
			String ExePath = Shell::GetExeFileName();
			String ExeDir  = String::GetDirectory(ExePath);

			Shell::SetCurrentDirectory( ExeDir );
      String::Initialize();

		//	CRand::SetSeed(::GetTickCount());
		}

		m_Instance = hInstance;
		m_PrevInstance = hPrevInstance;
    m_CommandLine = String::ConvertUNICODEtoMAID(GetCommandLine());
		m_CommandShow = nCmdShow;

    {	//	�R�}���h���C���̔���
	    int    nArgs;
	    LPTSTR *lplpszArgs;

      lplpszArgs = CommandLineToArgvW(GetCommandLine(), &nArgs);

	    for (int i = 0; i < nArgs; i++) 
      {
        m_DivComdList.push_back(String::ConvertUNICODEtoMAID(lplpszArgs[i])); 
	    }

	    LocalFree(lplpszArgs);
    }

		{	//	�v���Z�X�̓o�^
			WNDCLASSEX		wc = {0};
			wc.cbSize	    = sizeof(WNDCLASSEX);
			wc.style      = CS_VREDRAW | CS_HREDRAW; 
			wc.lpfnWndProc	= ProcessProc; 
			wc.hInstance		= m_Instance;
			wc.cbClsExtra		= 0;
			wc.cbWndExtra		= 0;
			wc.hIcon        = ::LoadIcon( m_Instance,IDI_APPLICATION); 
			wc.hCursor      = ::LoadCursor( NULL, IDC_ARROW ); 
			wc.hbrBackground	= (HBRUSH)::GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName		= NULL; 
			wc.lpszClassName	= DEFAULT_CLASSNAME;
			 
			wc.hIconSm			= NULL;

			if( ::RegisterClassEx(&wc)==0 ) 
			{
				MAID_THROWEXCEPTION(MAIDTEXT("RegisterClassEx �Ɏ��s"));
			}
		}



		OnSetup();

		while( IsValid() )
		{
			OnLoop();
		}

		OnCleanup();
	}
	catch( Exception& e )
	{
		MAID_WARNING( "���C�u������O:" << e.GetLog() );

		const String str = MAIDTEXT("�G���[���������܂����B�A�v���P�[�V�������I�����܂�\n") + String::ConvertSJIStoMAID(e.GetLog());

		::MessageBox( NULL, String::ConvertMAIDtoUNICODE(str).c_str(), L"execption", MB_OK );	
	}
	catch(...)
	{
		MAID_WARNING( "�V�X�e����O" );
		throw;
	}

	return m_ApplicationExitCode;
}

LRESULT CALLBACK Application::ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WindowsMessage mess(hWnd,msg,wParam,lParam);
  return GlobalPointer<Application>::Get()->m_HookManager.OnMessage( mess );
}

}