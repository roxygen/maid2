#ifndef maid2_sound_soundmessagebase_h
#define maid2_sound_soundmessagebase_h

/*!	
	@file
	@brief	���b�Z�[�W�����֐��̃x�[�X
*/

#include"../config/define.h"
#include"../config/typedef.h"
#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  class SoundCore;

  class SoundObjectInfo
  {
    friend SoundCore;
  public:
    SoundObjectInfo( unt id )
      :m_ID(id)
      ,m_IsPlay(false)
      ,m_Position(0)
      ,m_MessageCount(0)
    {}

    unt  GetID()  const { return m_ID; }
    bool IsPlay() const { return m_IsPlay; }
    double GetPosition() const { return m_Position; }

    bool IsMessageExecuting() const { return m_MessageCount!=0; }

  private:
    int   m_MessageCount; //  �܂��������ĂȂ����b�Z�[�W��
    unt   m_ID;
    bool  m_IsPlay;
    double m_Position;
    double m_Volume;
  };
  typedef	boost::shared_ptr<SoundObjectInfo>	SPSOUNDOBJECTINFO;

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
				CREATE_PCMREALTIME, //!<	�������X�g���[���Ƃ��Đ���
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
      SPSOUNDOBJECTINFO pInfo;
    private:
			int		ID;
		};
	}

	typedef	boost::shared_ptr<SoundMessage::Base>	SPSOUNDMESSAGE;
	typedef	std::vector<SPSOUNDMESSAGE>			SOUNDMESSAGEQUE;

  struct JUMPPOINT
  {
	  double	Boot;
	  double	Jump;

    JUMPPOINT():Boot(-1),Jump(-1){}
  };
	typedef	std::vector<JUMPPOINT>			SOUNDJUMPPOINTLIST;
}

#endif
