/*!
	@file
	@brief	�^�C�}�N���X timeGetTime() Ver.
*/

#include"timer.h"

//WIN32_LEAN_AND_MEAN ���w�肵�Ă邽�� �w�b�_��include����Ȃ�
#include<mmsystem.h>

#pragma comment (lib,"winmm.lib" )


namespace Maid
{

int Timer::s_RefarenceCount;

Timer::Timer()
{
	if( s_RefarenceCount==0 )
	{
		::TIMECAPS TimeCaps;
		::timeGetDevCaps(&TimeCaps,sizeof(::TIMECAPS));       
		::timeBeginPeriod(TimeCaps.wPeriodMin);
	}

	++s_RefarenceCount;
}

Timer::Timer( const Timer& rha )
{
	++s_RefarenceCount;
}

Timer::~Timer()
{
	--s_RefarenceCount;

	if( s_RefarenceCount==0 )
	{
		::TIMECAPS TimeCaps;
		::timeGetDevCaps(&TimeCaps,sizeof(::TIMECAPS));       
		::timeEndPeriod(TimeCaps.wPeriodMin);
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���Ԃ̎擾
/*!
    @return	�n�r���N�����Ă���̎���(�~���b)
 */
unt Timer::Get() const
{
	return ::timeGetTime();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �X���b�h��Q������
/*!
    ����܂萳�m�ł͂Ȃ��炵�����ǁA���ɂȂ�Ȃ����ȁB

    @param	time [i ] �Q�����鎞��(�~���b)
 */
void Timer::Sleep( unt time )
{
	::Sleep( time );
}


Timer& Timer::operator = ( const Timer& rha )
{
	++s_RefarenceCount;

	return *this;
}

}


