#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include"../../graphics/rendertargetbase.h"
#include"../../graphics/depthstencil.h"

#include"guimessage.h"


#include<boost/smart_ptr.hpp>
#include<map>

namespace Maid
{
  class IGUIParts;
  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;
  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts(){}

    class IGUIListner
    {
    public:
      virtual ~IGUIListner(){}
      virtual void Initialize(){}
      virtual void Finalize(){}
      virtual void UpdateFrame(){}
      virtual void UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const POINT2DI& pos ){}

      virtual void SetPosition ( const POINT2DI& pos  ){}
      virtual void SetZOrder ( int Order  ){}
      virtual void SetEnable ( bool IsEnable  ){}
      virtual void SetVisible( bool IsVisible ){}

      virtual bool IsMouseIn( const POINT2DI& pos ) const{ return false; }
      virtual void MouseMove( const POINT2DI& pos ){}
      virtual void MouseIn( const POINT2DI& pos ){}
      virtual void MouseOut( const POINT2DI& pos ){}
      virtual void MouseDown( const POINT2DI& pos, int Button ){}
      virtual void MouseUp( const POINT2DI& pos, int Button ){}
    };


    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    void SetParent( IGUIParts* pParent );

    void SetID( ID id );
    ID   GetID() const;

    void SendMessage( const IGUIParam& Param );
    void UpdateFrame();
    void UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth );

    void SetZOrder( int z );
    int  GetZOrder() const;

    //  ↓の３つはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void SetPosition( const POINT2DI& pos );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    POINT2DI GetPosition() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    bool IsMouseIn( const POINT2DI& pos ) const;

  protected:
    void SendMessageParent( const IGUIParam& Param );
    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}
    virtual void LocalSendMessage( const IGUIParam& Param ){}
    virtual void LocalUpdateFrame(){}
    virtual void LocalUpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const POINT2DI& pos ){}
    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const{ return false; }

    const VECTOR2DI& GetOffset() const;
    void SetListner( const boost::shared_ptr<IGUIListner>& pListner );
  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
    int         m_ZOrder;

    bool  m_IsMouseIn;
    boost::shared_ptr<IGUIListner>  m_pListner;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

  struct GUIPARTSLESS : public std::less<SPGUIPARTS>
  {
      bool operator()(const SPGUIPARTS& lha, const SPGUIPARTS& rha ) const 
      {
        return lha->GetZOrder() < rha->GetZOrder();
      } 
  };

}


#endif
