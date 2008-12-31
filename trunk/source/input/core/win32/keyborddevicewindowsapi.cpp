#include"keyborddevicewindowsapi.h"
#include"../../../auxiliary/macro.h"

// ���z�L�[�R�[�h�́����Q��
// http://yokohama.cool.ne.jp/chokuto/urawaza/prm/virtual_key_code.html


namespace Maid{ namespace Input {

	/*!
	 	@class	KeybordDeviceWindowsAPI keyborddevicewindowsapi.h
	 	@brief	�L�[�{�[�h�h���C�o
	 */ 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 */
KeybordDeviceWindowsAPI::KeybordDeviceWindowsAPI()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
KeybordDeviceWindowsAPI::~KeybordDeviceWindowsAPI()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �L�[�{�[�h�h���C�o�̏�����
/*!
 	�����Ȃǂ� IKeybordDevice::Initialize ���Q�Ƃ��邱��
 */
void	KeybordDeviceWindowsAPI::Initialize()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �L�[�{�[�h�h���C�o�̉��
/*!
 	�����Ȃǂ� IKeybordDevice::Finalize ���Q�Ƃ��邱��
 */
void  KeybordDeviceWindowsAPI::Finalize()
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �e�{�^����������Ă��邩���ׂ�
/*!
 	�����Ȃǂ� IKeybordDevice::IsDown ���Q�Ƃ��邱��
 */
void KeybordDeviceWindowsAPI::IsDown( std::vector<bool>& KeyList )	    const
{
  for( int i=0; i<256; ++i )
  {
    KeyList[i] = IsFlag(::GetAsyncKeyState(i),0x8000);
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���̃h���C�o�Ŏ��ʂł���{�^���̐����擾����
/*!
 *	�����Ȃǂ� IKeybordDevice::GetButtonnMax ���Q�Ƃ��邱��
 */
int	KeybordDeviceWindowsAPI::GetButtonnMax()	const
{
	return 256;
}



}}