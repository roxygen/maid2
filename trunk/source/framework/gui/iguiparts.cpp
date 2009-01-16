#include"iguiparts.h"
#include"guimessageque.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_IsMouseIn(false)
  ,m_Position(0,0)
  ,m_pPostTarget(NULL)
  ,m_ZOrder(ZORDER_FRONT)
{

}


IGUIParts::~IGUIParts()
{
  DelChildAll();
  MAID_ASSERT( m_ID!=PARTSID_NONE, "IGUIParam::MESSAGE_FINALIZE が来ていないのに、開放されようとしてます " << m_ID );
}

void IGUIParts::UpdateFrame()
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsEnable() ) { return ; }
    OnUpdateFrame();
  }

  //  子も実行
  for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
  {
    (*ite)->UpdateFrame();
  }
}

void IGUIParts::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsVisible() ) { return ; }
    OnUpdateDraw( Target, Depth, m_Position );
  }

  //  子も実行
  for( CHILDLIST::reverse_iterator ite=m_ChildList.rbegin(); ite!=m_ChildList.rend(); ++ite )
  {
    (*ite)->UpdateDraw( Target, Depth );
  }
}


void IGUIParts::SetPosition( const POINT2DI& pos )
{
  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
  SendMessage( m );
}


void IGUIParts::MovePosition( const VECTOR2DI& vec )
{
  GUIMESSAGE_MOVEPOSITION m;
  m.Offset = vec;
  SendMessage( m );
}


void IGUIParts::SetEnable ( bool IsEnable )
{
  GUIMESSAGE_ENABLE m;
  m.IsEnable = IsEnable;
  SendMessage( m );
}

void IGUIParts::SetVisible( bool IsVisible )
{
  GUIMESSAGE_VISIBLE m;
  m.IsVisible = IsVisible;
  SendMessage( m );
}

const POINT2DI& IGUIParts::GetPosition() const
{
  return m_Position;
}


bool IGUIParts::IsEnable () const
{
  return m_IsEnable;
}

bool IGUIParts::IsVisible() const
{
  return m_IsVisible;
}

IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}


bool IGUIParts::IsCollision( const POINT2DI& pos ) const
{
  if( !IsEnable() ) { return false; }

  const POINT2DI p = CalcLocalPosition(pos);
  return LocalIsCollision( p );
}



IGUIParts* IGUIParts::FindChildParts( const POINT2DI& pos )
{
  for( CHILDLIST::iterator ite =m_ChildList.begin();
                           ite!=m_ChildList.end();
                           ++ite )
  {
    IGUIParts* pChild = (*ite)->FindChildParts( pos );
    if( pChild!=NULL ) { return pChild; }
  }

  if( IsCollision(pos) ) { return this; }

  return NULL;
}

void  IGUIParts::InsertChildList( IGUIParts* pParts )
{
  if( m_ChildList.empty() )
  {
    m_ChildList.push_back(pParts);
    return;
  }

  const ZORDER z = pParts->GetZOrder();

  switch( z )
  {
  case ZORDER_FRONTLOCK: { m_ChildList.push_front(pParts); }break;

  case ZORDER_FRONT:
    {
      bool IsFind = false;
      for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                      ite != m_ChildList.end(); 
                                      ++ite )
      {
        if( (*ite)->GetZOrder() <= z ) 
        {
          m_ChildList.insert( ite, pParts );
          IsFind = true; 
          break;
        }
      }

      //  自分より前がないなら先頭に入れる
      if( !IsFind ) { m_ChildList.push_front(pParts); }break;
    }break;

  case ZORDER_BACK:
    {
      bool IsFind = false;
      for( CHILDLIST::const_reverse_iterator ite = m_ChildList.rbegin();
                                      ite != m_ChildList.rend(); 
                                      ++ite )
      {
        if( z <= (*ite)->GetZOrder() ) 
        {
          m_ChildList.insert( ite.base(), pParts );
          IsFind = true; 
          break;
        }
      }
      //  自分より後ろがないなら後ろに入れる
      if( !IsFind ) { m_ChildList.push_back(pParts); }break;
    }break;

  case ZORDER_BACKLOCK: { m_ChildList.push_back(pParts); }break;
  }


}

void IGUIParts::EraseChildList( IGUIParts* pParts )
{
  for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==pParts->GetID() ) 
    {
      m_ChildList.erase( ite );
      return;
    }
  }

  MAID_ASSERT( true, "見つかりませんでした" );

}




void IGUIParts::AddChildParts( IGUIParts::ID id, IGUIParts& Parts )
{
  MAID_ASSERT( !(id<PARTSID_LOCAL), "この値は使えません " << id );
  LocalAddChildParts( id, Parts );
}

void IGUIParts::LocalAddChildParts( IGUIParts::ID id, IGUIParts& Parts )
{
  {
    GUIMESSAGE_INITIALIZE m;
    m.pParent = this;
    m.ID = id;

    Parts.SendMessage( m );
  }

  InsertChildList( &Parts );
}


void IGUIParts::DelChildParts( IGUIParts& Parts )
{
  EraseChildList( &Parts );
  Parts.SendMessage( GUIMESSAGE_FINALIZE() );
}

void IGUIParts::DelChildAll()
{
  const GUIMESSAGE_FINALIZE m;

  for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    (*ite)->SendMessage( m );
  }

  m_ChildList.clear();
}

void IGUIParts::ChangeChilZOrder( IGUIParts& Parts, ZORDER type )
{
  EraseChildList( &Parts );
  Parts.m_ZOrder = type;
  InsertChildList( &Parts );
}

void IGUIParts::SetZOrder( ZORDER type )
{
  if( m_pParent==NULL )
  {
    m_ZOrder = type;
  }else
  {
    m_pParent->ChangeChilZOrder( *this, type );
  }
}

IGUIParts::ZORDER IGUIParts::GetZOrder()const
{
  return m_ZOrder;
}

POINT2DI IGUIParts::CalcLocalPosition( const POINT2DI& pos ) const
{
  return POINT2DI( pos.x - m_Position.x, pos.y - m_Position.y );
}

void IGUIParts::SetMessageQue( GUIMessageQue* pQue )
{
  m_pPostTarget = pQue;

  for( CHILDLIST::iterator ite =m_ChildList.begin();
                           ite!=m_ChildList.end();
                           ++ite )
  {
    (*ite)->SetMessageQue( pQue );
  }
}

void IGUIParts::PostMessage( const IGUIParam& Param )
{
  MAID_ASSERT( m_pPostTarget==NULL, "投稿先がありません" );
  m_pPostTarget->PostMessage( this, Param );
}


void IGUIParts::SendMessage( const IGUIParam& Param )
{
  SPGUIPARAM p( Param.Clone() );

  if( MessageExecuting( p )==MESSAGERETURN_SENDPARENT )
  {
    if( m_pParent==NULL ) { return; }
    m_pParent->ReciveMessage( m_ID, *p );
  }
}

void IGUIParts::ReciveMessage( ID src, const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return; }
  m_pParent->ReciveMessage( src, Param );
}




IGUIParts::MESSAGERETURN IGUIParts::MessageExecuting( SPGUIPARAM& Param )
{
  //  ここでメッセージの処理を行います
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      const GUIMESSAGE_INITIALIZE& p = static_cast<const GUIMESSAGE_INITIALIZE&>(*Param);
      MAID_ASSERT( p.ID==PARTSID_NONE, "このIDは使用できません " << p.ID );

      if( p.pParent!=NULL )
      {
        m_pPostTarget = p.pParent->m_pPostTarget;
        m_pParent = p.pParent;
      }
      m_ID = p.ID;
      OnInitialize( p.ID, *m_pParent );
    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {
      const GUIMESSAGE_FINALIZE& p = static_cast<const GUIMESSAGE_FINALIZE&>(*Param);

      m_pParent = NULL;
      m_ID = PARTSID_NONE;

      OnFinalize();
    }break;

  case IGUIParam::MESSAGE_SETPOSITION:
    {
      const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(*Param);

      const VECTOR2DI mov(m_Position, p.Position);

      m_Position = p.Position;
      OnSetPosition( p.Position );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->MovePosition( mov );
      }

    }break;

  case IGUIParam::MESSAGE_MOVEPOSITION:
    {
      const GUIMESSAGE_MOVEPOSITION& p = static_cast<const GUIMESSAGE_MOVEPOSITION&>(*Param);
      m_Position += p.Offset;
      OnMovePosition( p.Offset );
      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->MovePosition( p.Offset );
      }
    }break;


  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(*Param);
      m_IsEnable = p.IsEnable;
      OnEnable( p.IsEnable );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->SetEnable( p.IsEnable );
      }

    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(*Param);
      m_IsVisible = p.IsVisible;
      OnVisible( p.IsVisible );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->SetVisible( p.IsVisible );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      m_IsMouseIn = true;
      OnMouseIn( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      m_IsMouseIn = false;
      OnMouseOut( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      OnMouseMove( pos );
    }break;
  }

  return MESSAGERETURN_SENDPARENT;
}


}

