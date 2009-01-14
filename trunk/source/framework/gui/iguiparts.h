#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"


#include"../../graphics/rendertargetbase.h"
#include"../../graphics/depthstencil.h"
#include"guimessage.h"


#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class GUIMessageQue;
  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts();

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    ID   GetID() const;

    void SendMessage( const IGUIParam& pParam );
    bool IsCollision( const POINT2DI& pos ) const;


    //  ↓のはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );
    void SetOffset( const VECTOR2DI& offset );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    const VECTOR2DI& GetOffset() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    IGUIParts* FindChildParts( const POINT2DI& pos );

    void AddChildParts( IGUIParts::ID id, IGUIParts& p );
    void DelChildParts( IGUIParts& p );
    void DelChildAll();

    enum ZORDER
    {
      ZORDER_FRONTLOCK=0, //  常に手前
      ZORDER_FRONT,     //  手前
      ZORDER_BACK,      //  奥
    };
    void SetZOrder( ZORDER type );
    ZORDER GetZOrder()const;
    void ChangeChilZOrder( IGUIParts& p, ZORDER type );

    bool IsMouseIn() const { return m_IsMouseIn; }

    void SetMessageQue( GUIMessageQue* pQue );
  protected:

    enum MESSAGERETURN
    {
      MESSAGERETURN_NONE,       //  このパーツで起きたイベントを親に届けない
      MESSAGERETURN_SENDPARENT, //  親に届ける
    };

    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );

    virtual bool LocalIsCollision( const POINT2DI& pos ) const{ return false; }

    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}

    virtual void OnUpdateFrame(){}
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}

    virtual void OnInitialize( ID id, const IGUIParts& Parent ){}
    virtual void OnFinalize(){}
    virtual void OnAddChild( const IGUIParts& Parent ){}
    virtual void OnDelChild(){}
    virtual void OnSetOffset( const VECTOR2DI& off ){}
    virtual void OnEnable( bool IsEnable ){}
    virtual void OnVisible( bool IsVisible ){}
    virtual void OnMouseMove( const POINT2DI& pos ){}
    virtual void OnMouseIn( const POINT2DI& pos ){}
    virtual void OnMouseOut( const POINT2DI& pos ){}

    const IGUIParts* GetParent() const { return m_pParent; }
    VECTOR2DI CalcTotalOffset() const;

    void PostMessage( const IGUIParam& Param );

  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
    bool        m_IsMouseIn;
    GUIMessageQue*  m_pPostTarget;

    typedef std::list<IGUIParts*> CHILDLIST;
    CHILDLIST m_ChildList;  //  m_ChildList.begin()...最前面 .end() 最背面
    ZORDER    m_ZOrder;  //  最前面か？
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
