#include"fpstimer.h"


namespace Maid
{
	/*!
	 	@class	FPSTimer fpstimer.h
	 	@brief	FPS ���Ǘ�����^�C�}�[
	 */ 


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 */
FPSTimer::FPSTimer()
{
	m_FPS = 60;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 */
FPSTimer::~FPSTimer()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ������
/*!
 */
void FPSTimer::Initialize()
{
	Reset();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �^�C�}�[�����Z�b�g����
/*!
 */
void FPSTimer::Reset()
{
	m_BeginTime = m_Timer.Get();
	m_TimeOverCount= 0;
	m_NowFrame  = 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���Ԃ܂ŃX���b�h���~�߂�
/*!
 	���̊֐��� IsTimerOver() ���X�V����̂Ŗ��t���[���Ăяo������
 */
void FPSTimer::Sleep()
{
	const unt NOW =  m_Timer.Get();
	m_NowFrame++;

	const unt TotalTime = NOW - m_BeginTime;
	const unt TotalFrame= TotalTime*m_FPS/1000;

	if( TotalFrame<m_NowFrame )
	{	//	�\��t���[����������Sleep() ��������X���b�h���~�߂�
		const unt SleepTime = m_NowFrame*1000/m_FPS-TotalTime;

		m_Timer.Sleep(SleepTime);
		
		m_TimeOverCount = 0;
	}else
	{	//	�\�莞�Ԃ��I�[�o�[������A�E�g
		m_TimeOverCount++;

		//	�]��ɂ��I�[�o�[���Ă��烊�Z�b�g���Ă���
		if( m_TimeOverCount>60 )
		{
			Reset();
		}
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���Ԃ��߂��Ă��邩�H
/*!
 	�߂�l�� true �̏ꍇ����ȍ~�̏������΂��� Sleep() �ɂ�������
 */
bool FPSTimer::IsTimerOver() const 
{
	return m_TimeOverCount!=0;	
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �ێ�����e�o�r��ύX����
/*!
 	@param	fps	[i ]	�V�����e�o�r
 */
void FPSTimer::SetFPS( unt fps )
{
	m_FPS = fps;
	Reset();
}


}
