#include"iguicontextmenu.h"
#include"../../auxiliary/collision.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid
{
  IGUIContextMenu::IChild::IChild()
    :m_IsSelect(false)
    ,m_pParent(NULL)
    ,m_Offset(0,0)
  {
  }

  void IGUIContextMenu::IChild::Insert( int id, IChild& parts )
  {
    parts.m_pParent = this;
    m_Child.insert( INFO( id, &parts ) );
  }

  void IGUIContextMenu::IChild::Delete( int id )
  {
    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      if( info.ID==id )
      {
        m_Child.erase( ite );
        break;
      }
    }
  }

  void IGUIContextMenu::IChild::DeleteAll()
  {
    m_Child.clear();
  }

  SIZE2DI IGUIContextMenu::IChild::CalcChildSize() const
  {
    int w = 0;
    int h = 0;

    for( CHILDLIST::const_iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      const SIZE2DI size = info.pChild->GetSize();
      w = std::max( w, size.w );
      h += size.h;
    }

    return SIZE2DI(w,h);
  }

  IGUIContextMenu::IChild* IGUIContextMenu::IChild::FindChild( const POINT2DI& pos )
  {
    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO&   info = *ite;
      if( info.pChild->IsCollision(pos) )
      {
        return info.pChild;
      }
    }

    return NULL;
  }

  bool IGUIContextMenu::IChild::IsCollision( const POINT2DI& pos ) const
  {
    const SIZE2DI size = GetSize();
    const POINT2DI off(m_Offset.x,m_Offset.y);

    return Collision<int>::IsPointToRect( pos, RECT2DI(off,size) );
  }


  void IGUIContextMenu::IChild::UpdateFrame()
  {
    if( IsSelect() )
    {
      for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
      {
        const INFO& info = *ite;
        info.pChild->UpdateFrame();
      }
    }

    LocalUpdateFrame();
  }

  void IGUIContextMenu::IChild::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
  {
    if( IsSelect() )
    {
      for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
      {
        const INFO& info = *ite;
        info.pChild->UpdateDraw( Target, Depth, pos );
      }
    }

    LocalUpdateDraw( Target, Depth, pos + m_Offset );
  }

  bool IGUIContextMenu::IChild::IsSelect() const
  {
    return m_IsSelect;
  }

  void IGUIContextMenu::IChild::SetSelectState( bool on )
  {
    m_IsSelect = on;
//    UpdateChildPosition();
  }

  void IGUIContextMenu::IChild::SetOffset( const VECTOR2DI& off )
  {
    m_Offset = off;

//    MAID_WARNING( "x:" << off.x << " y:" << off.y );
    UpdateChildPosition();
  }

  const VECTOR2DI& IGUIContextMenu::IChild::GetOffset() const
  {
    return m_Offset;
  }


  void IGUIContextMenu::IChild::UpdateChildPosition()
  {
    VECTOR2DI now = m_Offset;

    if( m_pParent!=NULL )
    {
      const SIZE2DI s = m_pParent->CalcChildSize();
      now.x += s.w;
    }

    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      const SIZE2DI size = info.pChild->GetSize();
      info.pChild->SetOffset( now );

      now.y += size.h;
    }
  }












  IGUIContextMenu::IGUIContextMenu()
  {
  }



  void IGUIContextMenu::Insert( int id, IChild& parts )
  {
    m_Top.Insert( id, parts );
  }

  void IGUIContextMenu::Delete( int id )
  {
    m_Top.Delete( id );
  }

  void IGUIContextMenu::DeleteAll()
  {
    m_Top.DeleteAll();
  }
 
  bool IGUIContextMenu::LocalIsCollision( const POINT2DI& pos ) const
  {
/*
    for( SELECTLIST::const_iterator ite =m_SelectList.begin(); ite!=m_SelectList.end(); ++ite )
    {
      const IChild* p = *ite;

      const SIZE2DI size = p->CalcChildSize();
      const POINT2DI off = POINT2DI(p->GetOffset().x,p->GetOffset().y);

      if( Collision<int>::IsPointToRect( pos, RECT2DI(off,size) ) )
      {
        return true;
      }
    }
*/
    return false;
  }

  void IGUIContextMenu::OnUpdateFrame()
  {
    m_Top.UpdateFrame();
  }

  void IGUIContextMenu::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
  {
    m_Top.UpdateDraw(Target,Depth,pos );
  }


  IGUIContextMenu::MESSAGERETURN IGUIContextMenu::MessageExecuting( SPGUIPARAM& pParam )
  {
    switch( pParam->Message )
    {
    case IGUIParam::MESSAGE_INITIALIZE:
      {
        m_Top.SetOffset( VECTOR2DI(0,0) );
        m_Top.SetSelectState( true );
        m_SelectList.push_back( &m_Top );
      }break;

    case IGUIParam::MESSAGE_MOUSEMOVE:
      {
        const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
        const POINT2DI pos = CalcLocalPosition(p.Position);

        IChild* pTop = m_SelectList.back();
        IChild* pChild = pTop->FindChild(pos);

        if( pChild!=NULL )
        { //  カーソルが子の上に載っているならフォーカスを移す
          m_SelectList.push_back(pChild);
          pChild->SetSelectState(true);
        }
        else
        {
          //  乗っていない場合は、別の子の上にいることがあるので
          //  それを探し出して、設定する
          for( SELECTLIST::reverse_iterator ite = m_SelectList.rbegin(); ite!=m_SelectList.rend(); ++ite )
          {
            IChild* p = *ite;

            IChild* pChild = p->FindChild(pos);

            if( pChild!=NULL )
            {
              while(true)
              {
                if( p==m_SelectList.back() ) { break; }
                m_SelectList.pop_back();
              }

              pTop->SetSelectState(false);
              pChild->SetSelectState(true);
              m_SelectList.push_back(pChild);
              break;
            }
          }
        }

      }break;
    }
/*
      MESSAGE_MOUSEMOVE,//  マウスが移動した
      MESSAGE_MOUSEIN,  //  マウスがパーツの範囲内に入った
      MESSAGE_MOUSEOUT, //  マウスがパーツの範囲内から出た
      MESSAGE_MOUSEDOWN,//  マウスボタンが押された
      MESSAGE_MOUSEUP,  //  マウスボタンが離された
*/
    return IGUIParts::MessageExecuting( pParam );
  }
}