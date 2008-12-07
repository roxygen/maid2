/*!
    @file
    @brief Win32 �p���b�Z�[�W�v���V�[�W��
 */


#ifndef framework_win32_messagehook_h
#define framework_win32_messagehook_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/globalpointer.h"



namespace Maid
{
  /*!
 	  @class	WindowsMessage WindowsMessage.h
 	  @brief	�v���V�[�W���p�ҏW�N���X
   */
  class WindowsMessage
  {
  public:
    //!	�R���X�g���N�^
    WindowsMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
      : m_hWnd(hWnd),m_Msg(msg),m_wParam(wParam),m_lParam(lParam)
    {

    }


    HWND	GetHWND() const { return m_hWnd; }		//!<	���b�Z�[�W�̑��M��n���h��
    UINT	GetMSG() const { return m_Msg; }		//!<	������ꂽ���b�Z�[�W�̎擾

    enum PARAM
    {
      PARAM_LO,
      PARAM_HI,
      PARAM_ALL
    };


    WPARAM	GetWPARAM(PARAM pos=PARAM_ALL) const 
    {
      if( pos==PARAM_LO ) { return LOWORD(m_wParam); }
      if( pos==PARAM_HI ) { return HIWORD(m_wParam); }
      return m_wParam; 
    }

    LPARAM	GetLPARAM(PARAM pos=PARAM_ALL) const
    {
      if( pos==PARAM_LO ) { return LOWORD(m_lParam); }
      if( pos==PARAM_HI ) { return HIWORD(m_lParam); }
      return m_lParam; 
    }

    void SetHWND( HWND hWnd ) { m_hWnd = hWnd; }
    void SetMSG( UINT msg ) { m_Msg = msg; }
    void SetWPARAM( WPARAM wparam ) { m_wParam = wparam; }
    void SetLPARAM( LPARAM lparam ) { m_lParam = lparam; }

  private:
    HWND	  m_hWnd;
    UINT	  m_Msg;
    WPARAM	m_wParam;
    LPARAM	m_lParam;
  };


  class MessageHookManager;
  class MessageHook: public GlobalPointer<MessageHookManager>
  {
  public:
    MessageHook();
    virtual ~MessageHook();

    /*!
        WindowProc() �Ŗ߂��Ă���R�[�h
     */
    enum RETURNCODE
    {
      RETURNCODE_RETURN=0,   //!<	���b�Z�[�W�͏������ꂽ�̂ł���ȍ~�͌Ă΂Ȃ�
      RETURNCODE_DEFAULT,    //!<	���b�Z�[�W��OS�ɖ߂�
    };

    void AddProc( HWND hWnd );
    void DelProc();

    void EnterInterruptMode();
    void LeaveInterruptMode();

    //! �v���V�[�W���ɃR�[���o�b�N���Ă��炤�֐�
    /*!
        ���̊֐����p����Ŏ������܂�

        @param	mess	[i ]	�����Ă������b�Z�[�W���

        @retval	 RETURNCODE_EXIT		����ȍ~���b�Z�[�W�������s���܂���
        @retval	 RETURNCODE_DEFAULT	�܂������܂�
     */
	  virtual RETURNCODE OnMessage( WindowsMessage& mess )=0;

  private:
    bool	m_IsHook;	//!<	���݃t�b�N����Ă��邩
    HWND	m_hWnd;		//!<	���݂̃n���h��
  };
}

#endif
