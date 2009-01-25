#include"igamethread.h"

#include"igameframework.h"

namespace Maid
{

using namespace OSMessage;

IGameThread::IGameThread()
  :m_IsExitGame(false),m_pFrameWork(NULL)
{

}

IGameThread::~IGameThread()
{

}


bool	IGameThread::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  return true;
}

void	IGameThread::Initialize()
{
}


void	IGameThread::Finalize()
{
}



bool IGameThread::IsExitGame()
{
	return m_IsExitGame;
}

void IGameThread::OnExitGame()
{
	m_IsExitGame = true;
}

GraphicsCore& IGameThread::GetGraphicsCore()
{
  return GetFrameWork()->GetGraphicsCore();
}

SoundCore& IGameThread::GetSoundCore()
{
  return GetFrameWork()->GetSoundCore();
}

Mouse& IGameThread::GetMouse()
{
  return GetFrameWork()->GetMouse();
}

Keybord& IGameThread::GetKeybord()
{
  return GetFrameWork()->GetKeybord();
}

TextInput& IGameThread::GetTextInput()
{
  return GetFrameWork()->GetTextInput();
}

const IGameThread::DEFAULTCONFIG& IGameThread::GetDefaultConfig() const
{
  return const_cast<IGameThread*>(this)->GetFrameWork()->GetDefaultConfig();
}


IGameThread::OSMESSAGESTATE IGameThread::OnOSMessage( const OSMessage::Base& Message )
{
  switch( Message.Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  }

  return OSMESSAGESTATE_DEFAULT;
}

IGameFrameWork* IGameThread::GetFrameWork()
{
  return m_pFrameWork;
}



}