#include"iguicontextmenu.h"
#include"../../auxiliary/collision.h"

namespace Maid
{
  IGUIContextMenu::IChild::IChild()
    :m_IsSelect(false)
  {
  }

  void IGUIContextMenu::IChild::Insert( int id, IChild& parts )
  {
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

    return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),size) );
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

  void IGUIContextMenu::IChild::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
  {
    if( IsSelect() )
    {
      for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
      {
        const INFO& info = *ite;
        info.pChild->UpdateDraw( Target, Depth );
      }
    }

    LocalUpdateDraw( Target, Depth, m_Position );
  }

  bool IGUIContextMenu::IChild::IsSelect() const
  {
    return m_IsSelect;
  }

  void IGUIContextMenu::IChild::SetSelectState( bool on )
  {
    m_IsSelect = on;
    UpdateChildPosition();
  }

  void IGUIContextMenu::IChild::SetPosition( const POINT2DI& pos )
  {
    m_Position = pos;
    UpdateChildPosition();
  }

  void IGUIContextMenu::UpdateChildPosition()
  {
    POINT2DI now = m_Position;
    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      info.pChild->UpdateDraw( Target, Depth );
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
    const SIZE2DI size = m_Top.CalcChildSize();

    return Collision<int>::IsPointToRect( pos, RECT2DI(0,0,size.w,size.h) );
  }

  void IGUIContextMenu::OnUpdateFrame()
  {
    m_Top.UpdateFrame();
  }

  void IGUIContextMenu::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
  {
    m_Top.UpdateDraw(Target,Depth );
  }


  IGUIContextMenu::MESSAGERETURN IGUIContextMenu::MessageExecuting( SPGUIPARAM& pParam )
  {
    switch( pParam->Message )
    {
    case IGUIParam::MESSAGE_INITIALIZE:
      {
        m_SelectList.push_back( &m_Top );
        m_Top.SetSelectState( true );
      }break;

    case IGUIParam::MESSAGE_MOUSEMOVE:
      {
        const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
        const POINT2DI pos = CalcLocalPosition(p.Position);
/*
        IChild* pTop = m_SelectList.back();
        IChild* pChild = pTop->FindChild(pos);

        if( pChild!=NULL )
        {
          m_SelectList.push_back(pChild);
          pChild->SetSelectState(true);
        }else
        {
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

              p->SetSelectState(false);
              pChild->SetSelectState(true);
              m_SelectList.push_back(pChild);
            }
            break;
          }
        }
*/
      }break;
    case IGUIParam::MESSAGE_SETPOSITION:
      {
        const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(*pParam);
        m_Top.SetPosition( p.Position );
      }
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