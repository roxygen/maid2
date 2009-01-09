#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include"../../graphics/rendertargetbase.h"

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
      virtual void UpdateDraw( RenderTargetBase& Target, const POINT2DI& pos ){}

      virtual void SetPosition ( const POINT2DI& pos  ){}
      virtual void SetZOrder ( int Order  ){}
      virtual void SetEnable ( bool IsEnable  ){}
      virtual void SetVisible( bool IsVisible ){}
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
    void UpdateDraw( RenderTargetBase& Target );

    void SetZOrder( int z );
    int  GetZOrder() const;

    //  ↓の３つはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void SetPosition( const POINT2DI& pos );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    const POINT2DI& GetPosition() const;
    bool IsEnable () const; 
    bool IsVisible() const; 

  protected:
    void SendMessageParent( const IGUIParam& Param );
    virtual void ReturnMessage( ID src, const IGUIParam& Param ){}
    virtual void LocalSendMessage( const IGUIParam& Param )=0;
    virtual void LocalUpdateFrame()=0;
    virtual void LocalUpdateDraw( RenderTargetBase& Target, const POINT2DI& pos )=0;

    void SetDefaultListner( const boost::shared_ptr<Listner>& pListner );
  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    POINT2DI    m_Position;
    int         m_ZOrder;

    boost::shared_ptr<Listner>  m_pListner;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;
}


#endif
