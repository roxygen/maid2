#include"iguiparts.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE), m_pParent(NULL)
{

}

void IGUIParts::SetParent( IGUIParts* pParent )
{
  m_pParent = pParent;
}

void IGUIParts::AddChild( const SPGUIPARTS& pChild )
{
  const ID id = pChild->GetID();

  MAID_ASSERT( m_Child.find(id)!=m_Child.end(), "すでに登録されています" );
  m_Child[id] = pChild;
}

void IGUIParts::DelChild( const SPGUIPARTS& pChild )
{
  const ID id = pChild->GetID();

  MAID_ASSERT( m_Child.find(id)==m_Child.end(), "パーツが見つかりません" );
  m_Child.erase( m_Child.find(id) );
}

void IGUIParts::SetID( ID id )
{
  MAID_ASSERT( id==PARTSID_NONE,   "この値は使えません" );
  MAID_ASSERT( id==PARTSID_PARENT, "この値は使えません" );

  m_ID = id;
}

IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}

void IGUIParts::SendMessageParent( const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return ; }
  m_pParent->ReturnMessage( m_ID, Param );
}


}

}