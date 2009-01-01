#ifndef maid2_sound_soundmessagebase_h
#define maid2_sound_soundmessagebase_h

/*!	
	@file
	@brief	���b�Z�[�W�����֐��̃x�[�X
*/

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  class SoundCore;
	namespace SoundMessage
	{
		class Base
		{
      friend  SoundCore;
		public:
			//	�T�E���h���b�Z�[�W
			enum MESSAGE
			{
				NONE,				//!<	�������Ȃ�

				CREATE_PCMSTATIC, //!<	�I���������T�E���h�Ƃ��Đ���
				CREATE_PCMSTREAM, //!<	�X�g���[���T�E���h�Ƃ��Đ���
				CREATE_CLONE,     //!<	�N���[�����쐬

				DELETEOBJECT,     //!<	�j��

				PLAY,		//!<	�Đ�	
				STOP,		//!<	�X�g�b�v
				SETPOSITION,//!<	�Đ��ꏊ�̈ړ�
				SETVOLUME,		//!<	�{�����[���̕ύX

        SETLOOPSTATE, //!<  ���[�v�Đ����邩�H
        SETJUMPPOINT, //!<  �w�肵���|�C���g�܂ł�����߂�ʒu
			};

			Base( MESSAGE mess ) : Message(mess){}

			const MESSAGE	Message;
    private:
			int		ID;
		};
	}

	typedef	boost::shared_ptr<SoundMessage::Base>	SPSOUNDMESSAGE;
	typedef	std::vector<SPSOUNDMESSAGE>			SOUNDMESSAGEQUE;

  struct JUMPPOINT
  {
	  float	Boot;
	  float	Jump;
  };
}

#endif
