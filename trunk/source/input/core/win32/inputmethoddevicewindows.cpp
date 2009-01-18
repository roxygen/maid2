
#include"inputmethoddevicewindows.h"

#include<imm.h>
#pragma comment(lib, "imm32.lib")


namespace Maid{ namespace Input {

/*!
 	@class	InputMethodDeviceWindows inputmethoddevicewindows.h
 	@brief	FEP�Ǘ��N���X
\n			IME �̏�Ԃ�����͂���Ă���e�L�X�g���擾�ł���N���X
 *			http://www.honet.ne.jp/~tri/program/noime.html
 */


	//	����release�N���X
	class IMCHandle
	{
	public:
		IMCHandle( const Window& hWnd )
		{
			m_hWnd = hWnd.GetHWND();
			m_hImc = ::ImmGetContext(m_hWnd);
		}
		~IMCHandle()
		{
			::ImmReleaseContext(m_hWnd,m_hImc);
		}

		HIMC Get() { return m_hImc; }
	private:
		HWND	m_hWnd;
		HIMC	m_hImc;
	};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 */
InputMethodDeviceWindows::InputMethodDeviceWindows( const Window& hWnd )
  :m_pCandidateList(NULL)
  ,m_Window(hWnd)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
InputMethodDeviceWindows::~InputMethodDeviceWindows()
{
    Finalize();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP�̊J��
/*!
*/
void InputMethodDeviceWindows::Finalize()
{
	m_pCandidateList = NULL;
	DelProc();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP �̏�����
/*!
 	@param	hWnd	[i ]	�֘A����E�B���h�E�n���h��
 */
void InputMethodDeviceWindows::Initialize()
{
  Finalize();

	AddProc( m_Window.GetHWND() );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP �̋N��
/*!
 */
void InputMethodDeviceWindows::Open()
{
	if( IsOpen() ) { return ; }

	IMCHandle hImc(m_Window);
	ImmSetOpenStatus(hImc.Get(), TRUE);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP �̏I��
/*!
 */
void InputMethodDeviceWindows::Close()
{
	if( !IsOpen() ) { return ; }

	IMCHandle hImc(m_Window);
	ImmSetOpenStatus(hImc.Get(), FALSE);
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP ���������Ă��邩�H
/*!
 *	\return �e�d�o�������Ă���Ȃ� true
\n			�������Ă��Ȃ��Ȃ� false
 */
bool InputMethodDeviceWindows::IsOpen() const
{
	IMCHandle hImc(m_Window);
	const bool IsOpen = ImmGetOpenStatus(hImc.Get())!=0;
	return IsOpen;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �e�d�o�̕ϊ���Ԃ��擾����
/*!
 	@param	pConversion	[i ]	���̓��[�h(���p�J�i�A�S�p�@���j
 	@param	pSentence	[i ]	�ϊ����[�h(��ʁE�n���D��@���j
 */
void InputMethodDeviceWindows::GetStatus( DWORD* pConversion, DWORD* pSentence) const
{
	IMCHandle hImc(m_Window);

	ImmGetConversionStatus(hImc.Get(),pConversion,pSentence);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �ϊ��ʒu�̎擾
/*!
 	@return	�ϊ��ʒu
 */
int InputMethodDeviceWindows::GetConvertPos() const
{
	IMCHandle hImc(m_Window);
    DWORD dwBuff[2];
    if(::ImmGetCompositionString(hImc.Get(),GCS_COMPCLAUSE,dwBuff,sizeof(DWORD)*2) == 0) { return 0; }
    return dwBuff[1];
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�[�\���ʒu�̎擾
/*!
 	@return	�J�[�\���ʒu
 */
int InputMethodDeviceWindows::GetCursorPos() const
{
  IMCHandle hImc(m_Window);

  DWORD count = ::ImmGetCompositionString(hImc.Get(),GCS_CURSORPOS,NULL,0);

  return count;
/*
  // ���̖߂�l�̓o�C�g���Ŗ߂��Ă���̂�
  // �������ɕϊ�����

  const String str = GetCompString();
  if( str.empty() ) { return 0; }

  int index = 0;

  while( true )
  {
    if( count==0 ) { break; }

    if( str[index]<=0xFF )	{ count -= 1; }
    else					{ count -= 2; }

    ++index;
  }
  return index;
*/
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �ϊ���⃊�X�g�̍쐬
/*!
   GetCountCandidate()
\n GetCandidateIndex()
\n GetCandidateString()
\n GetCandidateStart()
\n GetCandidateEnd()
\n	���g���O�ɌĂ�ł�������
 */
void InputMethodDeviceWindows::UpdateCandidate()
{
  IMCHandle hImc(m_Window);
  //�ϊ����̎擾
  const INT iCandidateSize = ::ImmGetCandidateList(hImc.Get(),0,NULL,0);
  if(iCandidateSize==0) { return; }

  m_pCandidateBuff.reset( new unt08[iCandidateSize] );
  m_pCandidateList = (LPCANDIDATELIST)m_pCandidateBuff.get();

  ::ImmGetCandidateList(hImc.Get(),0,m_pCandidateList,iCandidateSize);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �ϊ���␔�̎擾
/*!
 	@return �ϊ���␔
 */
int InputMethodDeviceWindows::GetCandidateCount() const
{
	if( m_pCandidateList==NULL ) { return 0; }
	return m_pCandidateList->dwCount;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���ݑI�𒆂̕ϊ����̎擾
/*!
 	@return �ϊ����ʒu
 */
int InputMethodDeviceWindows::GetCandidateSelect() const
{
	if(m_pCandidateList==NULL) { return 0; }
    return m_pCandidateList->dwSelection;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP���͕�����̎擾
/*!
 	@return	���͒��̕�����
 */
String InputMethodDeviceWindows::GetCompString() const
{
  IMCHandle hImc(m_Window);
  const LONG lByte = ::ImmGetCompositionString(hImc.Get(),GCS_COMPSTR,NULL,0);

  if( lByte==0 ) { return String(); }

  std::wstring	Buff;
  Buff.resize(lByte);

  ::ImmGetCompositionString( hImc.Get(), GCS_COMPSTR, &Buff[0], lByte);

  return String::ConvertUNICODEtoMAID(Buff);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �w�肳�ꂽ�C���f�b�N�X�̕ϊ������擾
/*!
 	@return �ϊ���╶��
 */
String InputMethodDeviceWindows::GetCandidateString(int Index) const
{
	if(m_pCandidateList==NULL) { return String(); }

  unt08* p = (unt08*)m_pCandidateList+m_pCandidateList->dwOffset[Index];

	std::wstring ret = (wchar_t*)p;

  return String::ConvertUNICODEtoMAID( ret );
}


void InputMethodDeviceWindows::Flush( String& buffer )
{
  buffer = m_EnterString;
  m_EnterString.clear();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �E�B���h�E�v���V�[�W��
/*!
 */
InputMethodDeviceWindows::RETURNCODE InputMethodDeviceWindows::OnMessage( WindowsMessage& Param )
{
	switch( Param.GetMSG() )
	{
	case WM_IME_SETCONTEXT:
		{
			Param.SetLPARAM( Param.GetLPARAM()&~ISC_SHOWUIALL );
		}break;

	case WM_IME_COMPOSITION:
		{
      if( Param.GetLPARAM()&GCS_RESULTSTR )
			{
        return RETURNCODE_DEFAULT;
/*
				IMCHandle hImc(m_Window);
				const LONG lByte = ::ImmGetCompositionString(hImc.Get(),GCS_RESULTSTR,NULL,0);
				if( lByte==0 ) { break; }

				std::wstring	str;
				str.resize(lByte);

				::ImmGetCompositionString( hImc.Get(), GCS_RESULTSTR, &str[0], lByte);

        m_EnterString += String::ConvertUNICODEtoMAID(str);
*/
			}
			return RETURNCODE_RETURN;
		}
		break;

	case WM_IME_STARTCOMPOSITION: { return RETURNCODE_RETURN;}break;
	case WM_IME_ENDCOMPOSITION:   { return RETURNCODE_RETURN;}break;

	case WM_IME_NOTIFY:
		{
			switch(Param.GetWPARAM())
			{
			case IMN_OPENSTATUSWINDOW:
			case IMN_CLOSESTATUSWINDOW:
				{
					return RETURNCODE_RETURN;
				}break;

			case IMN_OPENCANDIDATE:
			case IMN_CHANGECANDIDATE:
			case IMN_CLOSECANDIDATE:
				{
					UpdateCandidate();
					return RETURNCODE_RETURN;
				}break;
			}
		}break;

	}

	return RETURNCODE_DEFAULT;
}

}}


/*
  http://nienie.com/~masapico/api_ImmGetCompositionString.html
  ���̂ւ���Q�l

*/


/*

  http://www.geocities.co.jp/SiliconValley-Cupertino/5872/Message/WM/
  ����̔����o��

WM_IME_CHAR = &H286 
	�@�\ 
		IME���ϊ����ʂ̕������󂯎�����Ƃ��A�A�v���P�[�V�����ɑ�����
 
	wParam 
		1�o�C�g�̕����̂Ƃ�ASC�U�l
		2�o�C�g�̕����̂Ƃ��V�t�gJIS�R�[�h
 
	lParam 
		�L�[�X�g���[�N���b�Z�[�W
 


WM_IME_COMPOSITION = &H10F 
�@�\ 
�L�[�̓��͂̌��ʁA�ϊ������񂪕ω������Ƃ��ɃA�v���P�[�V�����ɑ�����
 
wParam 
�ϊ�������ɑ΂��čŌ�̕ω���\��2�o�C�g����
 
lParam 
�ϊ������񂪕ω�����������\���萔�̑g�ݍ��킹
�萔                  �l         ���e
---------------------------------------------------------------------------
GCS_COMPATTR          &H10       �ϊ�������̑������擾��X�V
GCS_COMPLAUSE         &H20       �ϊ�������̋�����擾��X�V
GCS_COMPREADATTR      &H2        ���݂̕ϊ�������̓ǂ݂̑����擾�E�X�V
GCS_COMPREADCLAUSE    &H4        ���݂̕�����̓ǂ݂̋�����擾��X�V
GCS_COMPREADSTR       &H1        ���݂̕ϊ�������̓ǂ݂��擾�E�X�V
GCS_COMPSTR           &H8        ���݂̕ϊ���������擾�E�X�V
GCS_CURSORPOS         &H80       �ϊ�������̃J�[�\���̈ʒu���擾��X�V
GCS_RESULTCLAUSE      &H1000     �ϊ��㕶����̋�����擾�E�X�V
GCS_RESULTREADCLAUSE  &H400      �ϊ��㕶����̓ǂ݂̋�����擾��X�V
GCS_RESULTREADSTR     &H200      �ϊ��㕶����̓ǂ݂̑������擾��X�V
GCS_RESULTSTR         &H800      �ϊ��㕶������擾��X�V
---------------------------------------------------------------------------
 
���l 
���̃��b�Z�[�W��SDK�̉���͕s���ȓ_������
 



�@�\ 
�ϊ��E�B���h�E�̗̈���m�ۂł��Ȃ��Ƃ��A�A�v���P�[�V�����ɑ�����
 
wParam 
���0
 
lParam 
���0
 
���l 
���̃��b�Z�[�W���󂯎�����ꍇ
IME_SETCOMPOSITIONWINDOW���g���ĕ\�����@���w�肷��
 


  WM_IME_CONTROL = &H283 
�@�\ 
�A�v���P�[�V�������쐬����IME�E�B���h�E�ɑ΂���
���s���ׂ����e��IME�E�B���h�E�ɑ��郁�b�Z�[�W
 
wParam 
�������e���w�肷��t���b�O(IMC_xxx)
 
lParam 
wParam�̐ݒ�ɉ������l
 
�߂�l 
lParam���߂�
 

WM_IME_ENDCOMPOSITION = &H10E
  �@�\ 
IME���ϊ����I������Ƃ��A�v���P�[�V�����ɑ�����
 
wParam 
���0
 
lParam 
���0
 





WM_IME_KEYDOWN = &H290 
�L�[���������Ƃ��A�v���P�[�V�����ɑ�����
 
wParam 
���z�L�[�R�[�h
 
lParam 
�L�[�X�g���[�N���b�Z�[�W
 


WM_IME_KEYUP = &H291 
�@�\ 
�L�[�𗣂����Ƃ��A�v���P�[�V�����ɑ�����
 
wParam 
���z�L�[�R�[�h
 
lParam 
�L�[�X�g���[�N���b�Z�[�W
 





WM_IME_NOTIFY = &H282 
�@�\ 
IME�E�B���h�E�ɕω����������Ƃ��A�v���P�[�V�����ɑ�����
 
wParam 
IME�E�B���h�E�̎��s���e���w�肷��t���b�O(IMN_xxx)
 
lParam 
wParam�ɉ������l
 
���l 
�A�v���P�[�V������IME�E�B���h�E�̑S�Ă̓���ɑ΂���
�ӔC�����ꍇ�Ɏg�����b�Z�[�W
 




WM_IME_REQUEST = &H288 
�@�\ 
IME�̏�ԂɊւ������v������

 
wParam 
�v��������e���w�肷��t���b�O(IMR_xxx)
 
lParam 
wParam�ɉ������l
 
���l 
�A�v���P�[�V����������IME�E�B���h�E���Ǘ�����ꍇ�Ɏg��
 


WM_IME_SELECT = &H285 
�@�\ 
�V�X�e����IME��ύX���悤�Ƃ��Ă���Ƃ��A�A�v���P�[�V�����ɑ�����
 
wParam 
���݂�IME���I������Ă���Ƃ�   1
�I������ĂȂ��Ƃ�              0
 
lParam 
IME�Ɋ֘A�t�����Ă���L�[�{�[�h���C�A�E�g�̃n���h��
 


WM_IME_SETCONTEXT = &H281 
�@�\ 
�A�v���P�[�V�����̃E�B���h�E���A�N�e�B�u�ɂȂ�Ƃ�
�A�v���P�[�V�����ɑ�����
 
wParam 
���̓R���e�L�X�g���A�N�e�B�u�ȂƂ�     1
�A�N�e�B�u�łȂ��Ƃ�                   0
 
lParam 
�\������E�B���h�E���w�肷��t���b�O(IMC_xxx)
 

WM_IME_STARTCOMPOSITION = &H10D 
�@�\ 
�L�[���͂ɂ���ĕϊ������񂪐�������钼�O�ɃA�v���P�[�V�����ɑ�����
 
wParam 
���0
 
lParam 
���0
 


























http://www.honet.ne.jp/~tri/program/noime.html
�̂��҂�

  IME�ŕ�����\�������Ȃ����@�@�`���U�@�`
��ڂ́A�����ɕ`����~�߂���@�ł�������IME�֘A�̃��b�Z�[�W���������邱�ƂŎ����ł��܂��B

switch(uMsg){
case WM_IME_SETCONTEXT:
    lParam &= ~ISC_SHOWUIALL;
    break;
case WM_IME_STARTCOMPOSITION:
case WM_IME_COMPOSITION:
case WM_IME_ENDCOMPOSITION:
    return 0;
case WM_IME_NOTIFY:
    switch(wParam){
    case IMN_OPENSTATUSWINDOW:
    case IMN_CLOSESTATUSWINDOW:
    case IMN_OPENCANDIDATE:
    case IMN_CHANGECANDIDATE:
    case IMN_CLOSECANDIDATE:
        return 0;
    default:
        return DefWindowProc(..[��]..);
    }
default
    return DefWindowProc(..[��]..);
}

�Ƃ܂�����Ȋ����B���Ԃɐ������Ă����Ɓc

WM_IME_SETCONTEXT
���̃��b�Z�[�W�͊�{�I�ɃE�B���h�E���A�N�e�B�u/��A�N�e�B�u�ɂȂ����Ƃ��ɑ����Ă��܂��B 
���̒���LPARAM��IME�֘A�̃E�B���h�E���ĕ`�悷�邩�ǂ������t���O�Ŏw�肳��Ă��܂��B
�ŁA�����ł͑S���\������K�v�͖����̂�ISC_SHOWUIALL�ŕ\���t���O��S�ď����čĕ`�悳���Ă܂���B
�ŏ���MSDN�̃h�L�������g�����āA�u���̃��b�Z�[�W���������Ă����΁A�����ƕ`�悳��Ȃ��v�Ɗ��Ⴂ���Ă܂����B�i�G�L�t���j
���ۂ͂��̃��b�Z�[�W�������������ɕ`�悪����Ȃ������ŁA���̍ĕ`��̃^�C�~���O������Ε`�悳��Ă��܂��܂��B


WM_IME_STARTCOMPOSITION/WM_IME_COMPOSITION/WM_IME_ENDCOMPOSITION
���̃��b�Z�[�W�͕ϊ��������񂪑��삳�ꂽ���ɑ����Ă��܂��B
���{��IME���ƃA���_�[���C�������Ă镶����̎��ł��B 
�����̕����񂪁u�o�Ă�����/�ύX���ꂽ��/�����Ȃ������v�ɂ��ꂼ�ꔭ�����܂��B
�ŁA���̃��b�Z�[�W��DefWindowProc()�ɓn���Ȃ���Ε`�悳��Ȃ��Ȃ�܂��B


IMN_OPENSTATUSWINDOW/IMN_CLOSESTATUSWINDOW
���������WM_IME_NOTIFY�̈ꕔ�ɂȂ�܂����A���̓�̃��b�Z�[�W�̓c�[���o�[���J�����ƕ��鎞�ɔ��ł��܂�
�Ȃ�ŁA�����DefWindowProc()�ɓn���Ȃ��ƃc�[���o�[���`�悳��Ȃ��Ȃ�܂��B
�c�Ǝv�������ԈႢ�ł����B�i�G�L�t���j
�c�[���o�[�Ɋ֘A���Ă�̂͊m���Ȃ�ł����A
�ʂɂ����DefWindowProc()�ɓn���Ȃ��Ă����ʂɕ`�悳����ł���ˁc�B(Win2000 SP4�ł�)
�Ȃ̂ŁA���̃��b�Z�[�W����������K�v������̂��ǂ���������܂��񂪁A�ꉞ����Ă����܂��傤�B


IMN_OPENCANDIDATE/IMN_CHANGECANDIDATE/IMN_CLOSECANDIDATE
�����WM_IME_COMPOSITION�n�Ɏ����悤�ȕ��ŁA�����Ă���^�C�~���O��
�� IMN_OPENCANDIDATE�͕ϊ����̃E�B���h�E���\�����ꂽ��  
�� IMN_CHANGECANDIDATE�͕ϊ����ꂽ���iMSIME���ƃX�y�[�X���������j  
�� IMN_CLOSECANDIDATE�͕ϊ����̃E�B���h�E��������  
�ƁA����Ȋ����ɂȂ��Ă܂��B���̃��b�Z�[�W��DefWindowProc()�ɓn���Ȃ���
�ϊ����E�B���h�E���`�悳��Ȃ��Ȃ�����A �ϊ������񂪕`�悳��Ȃ��Ȃ�܂��B 
�{���͂��������ꍇ����������K�v�������ł����A���{��Ɗ؍���IME���g���Ă����͂���Ŗ��Ȃ������̂Ŗ������Ă܂��B�i�P�[�P�j
�����A���m�Ȃ̂��m�肽���l��PlatformSDK�̃T���v�������܂��傤�B
���Ȃ݂ɁA�����VisualStudio�Ƃ͕ʂ̕���Platform SDK Update ����_�E�����[�h�ł��܂��B


*/