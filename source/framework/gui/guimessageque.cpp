#include"guimessageque.h"


namespace Maid
{
GUIMessageQue::~GUIMessageQue()
{
  //  残ってるメッセージを処理したほうがいい予感なのでする
  Execute();
}

void GUIMessageQue::PostMessage( IGUIParts* pTarget, const IGUIParam& Param )
{
  MESSAGE m;
  m.pTarget = pTarget;
  m.pParam.reset(Param.Clone());

  m_Que.push_back( m );
}


void GUIMessageQue::Execute()
{
  while( true )
  {
    //  この関数の中でメッセージが増えることがあるので
    //  なくなるまでひたすらに処理する
    if( m_Que.empty() ) { break; }

    MESSAGEQUE dat;
    dat.swap(m_Que);

    for( size_t i=0; i<dat.size(); ++i )
    {
      MESSAGE& m = dat[i];

      m.pTarget->SendMessage( *(m.pParam) );
    }
  }

}

}