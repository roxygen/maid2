#include"guicallback.h"



namespace Maid
{
void GUICallback::SetReviceFunction( const RECIVEFUNCTION& Func )
{
  m_Func = Func;
}

void GUICallback::ReciveMessage( ID src, const IGUIParam& Param )
{
  if( !m_Func ) { return ; }

  m_Func( src, Param );
}


}
