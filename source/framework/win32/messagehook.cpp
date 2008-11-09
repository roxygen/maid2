#include"messagehook.h"
#include"messagehookmanager.h"


namespace Maid
{

/*!
  @class	MessageHook messagehook.h
  @brief	Win32 �p���b�Z�[�W�v���V�[�W��
          ���b�Z�[�W�����͂��ׂĂ��̃N���X���p�����Ď������܂�
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 */
MessageHook::MessageHook()
{
	m_IsHook = false;
	m_hWnd   = NULL;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
MessageHook::~MessageHook()
{
	DelProc();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �v���V�[�W���ɃR�[���o�b�N���Ă��炤�悤�ɂ���
/*!
    @param	hWnd	[i ]	�o�^����ꏊ
 */
void MessageHook::AddProc( HWND hWnd )
{
  GlobalPointer<MessageHookManager>::Get()->AddHook( this, hWnd );
  m_hWnd   = hWnd;
  m_IsHook = true;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �v���V�[�W���ɃR�[���o�b�N����߂Ă��炤
/*!
 */
void MessageHook::DelProc()
{
  if( m_IsHook )
  {
    GlobalPointer<MessageHookManager>::Get()->DelHook( this, m_hWnd );
    m_hWnd   = NULL;
    m_IsHook = false;
  }
}

void MessageHook::EnterInterruptMode()
{
  GlobalPointer<MessageHookManager>::Get()->SetInterruptHook( this );
}

void MessageHook::LeaveInterruptMode()
{
  GlobalPointer<MessageHookManager>::Get()->SetInterruptHook( NULL );
}


}