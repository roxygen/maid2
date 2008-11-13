#include"application.h"

#include"shell.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include"../../storage/storage.h"
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
  
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
Application::~Application()
{
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
//! ���s
/*!
 	  @param	hInstance		[i ]	WinMain() ����n���ꂽ����
 	  @param	hPrevInstance	[i ]	WinMain() ����n���ꂽ����
 	  @param	lpCmdLine		[i ]	WinMain() ����n���ꂽ����
 	  @param	nCmdShow		[i ]	WinMain() ����n���ꂽ����
 */
int Application::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	m_Instance = hInstance;
	m_PrevInstance = hPrevInstance;
	m_CommandShow = nCmdShow;

  return ApplicationLib::Run();
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
		if( Msg.message == WM_QUIT ){ return false;	}
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
void Application::LocalExit()
{
	::PostQuitMessage(0);
}

void Application::OnSetup()
{
  Set();
  Shell::CoInitialize();
	Shell::InitCommonControls();

	{	//	�����ݒ�
		String ExePath = Shell::GetExeFileName();
		String ExeDir  = String::GetDirectory(ExePath);

		Shell::SetCurrentDirectory( ExeDir );
    m_HookManager.Initialize();
	//	CRand::SetSeed(::GetTickCount());

	}

  m_CommandLine = String::ConvertUNICODEtoMAID(GetCommandLine());

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

  LocalSetup();
}


void Application::OnLoop()
{
  LocalLoop();
}

void Application::OnCleanup()
{
  LocalCleanup();
	Shell::CoUninitialize();
}


LRESULT CALLBACK Application::ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WindowsMessage mess(hWnd,msg,wParam,lParam);
  return GlobalPointer<Application>::Get()->m_HookManager.OnMessage( mess );
}

}