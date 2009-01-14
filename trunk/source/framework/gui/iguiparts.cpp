#include"iguiparts.h"
#include"guimessageque.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Offset(0,0)
  ,m_pPostTarget(NULL)
  ,m_ZOrder(ZORDER_FRONT)
{

}


IGUIParts::~IGUIParts()
{
  MAID_ASSERT( m_ID!=PARTSID_NONE, "IGUIParam::MESSAGE_FINALIZE が来ていないのに、開放されようとしてます " );
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

void IGUIParts::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsVisible() ) { return ; }
    OnUpdateDraw( Target, Depth, pos + m_Offset );
  }

  //  子も実行
  for( CHILDLIST::reverse_iterator ite=m_ChildList.rbegin(); ite!=m_ChildList.rend(); ++ite )
  {
    (*ite)->UpdateDraw( Target, Depth, pos + m_Offset );
  }
}


void IGUIParts::SetOffset( const VECTOR2DI& offset )
{
  GUIMESSAGE_SETOFFSET m;
  m.Offset = offset;
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

const VECTOR2DI& IGUIParts::GetOffset() const
{
  return m_Offset;
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

  const POINT2DI p = pos-m_Offset;
  return LocalIsCollision( p );
}



IGUIParts* IGUIParts::FindChildParts( const POINT2DI& pos )
{
  const POINT2DI p = pos-m_Offset;

  for( CHILDLIST::iterator ite =m_ChildList.begin();
                           ite!=m_ChildList.end();
                           ++ite )
  {
    IGUIParts* pChild = (*ite)->FindChildParts( p );
    if( pChild!=NULL ) { return pChild; }
  }

  if( LocalIsCollision(p) ) { return this; }

  return NULL;
}



void IGUIParts::AddChildParts( IGUIParts::ID id, IGUIParts& Parts )
{
  {
    GUIMESSAGE_INITIALIZE m;
    m.pParent = this;
    m.ID = id;

    Parts.SendMessage( m );
  }

  m_ChildList.push_back( &Parts );
}

void IGUIParts::DelChildParts( IGUIParts& Parts )
{
  for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==Parts.GetID() ) 
    {
      (*ite)->SendMessage( GUIMESSAGE_FINALIZE() );
      m_ChildList.erase( ite );
      break; 
    }
  }
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
  IGUIParts* p = NULL;

  for( CHILDLIST::iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {

    if( (*ite)->GetID()==Parts.GetID() ) 
    {
      p = (*ite);
      m_ChildList.erase( ite );
      break; 
    }
  }

  for( CHILDLIST::iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetZOrder() < type ) { continue; }

    m_ChildList.insert( ite, p );

    p->m_ZOrder = type;
  }
}

void IGUIParts::SetZOrder( ZORDER type )
{
  if( m_pParent==NULL ) { return ; }

  m_pParent->ChangeChilZOrder( *this, type );
}

IGUIParts::ZORDER IGUIParts::GetZOrder()const
{
  return m_ZOrder;
}

VECTOR2DI IGUIParts::CalcTotalOffset() const
{
  VECTOR2DI ret(0,0);
  const IGUIParts* p = this;

  while( true )
  {
    if( p==NULL ){ break; }

    ret += p->GetOffset();
    p = p->GetParent();
  }

  return ret;
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

  case IGUIParam::MESSAGE_SETOFFSET:
    {
      const GUIMESSAGE_SETOFFSET& p = static_cast<const GUIMESSAGE_SETOFFSET&>(*Param);
      m_Offset = p.Offset;
      OnSetOffset( p.Offset );
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(*Param);
      m_IsEnable = p.IsEnable;
      OnEnable( p.IsEnable );
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(*Param);
      m_IsVisible = p.IsVisible;
      OnVisible( p.IsVisible );
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = true;
      OnMouseIn( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = false;
      OnMouseOut( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      OnMouseMove( pos );
    }break;
  }

  return MESSAGERETURN_SENDPARENT;
}


}

