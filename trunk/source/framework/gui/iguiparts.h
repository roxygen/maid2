#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"


#include"guimessage.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParts;
  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts(){}

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    ID   GetID() const;

    void SendMessage( const IGUIParam& Param );
    bool IsMouseIn( const POINT2DI& pos ) const;

    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );

    //  ↓のはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void SetOffset( const VECTOR2DI& offset );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    const VECTOR2DI& GetOffset() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    SPGUIPARTS FindChild( const POINT2DI& pos ) const;

  protected:
    void SendMessageParent( const IGUIParam& Param );

    virtual SPGUIPARTS LocalFindChild( const POINT2DI& pos ) const { return SPGUIPARTS(); }
    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const{ return false; }

    virtual void OnSendMessage( const IGUIParam& Param ){}
    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}

    virtual void OnInitialize(){}
    virtual void OnFinalize(){}
    virtual void OnUpdateFrame(){}
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}

    const IGUIParts* GetParent() const { return m_pParent; }
  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
