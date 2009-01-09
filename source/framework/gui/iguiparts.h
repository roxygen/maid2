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
    virtual ~IGUIParts(){}

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    void SetParent( IGUIParts* pParent );

    void SetID( ID id );
    ID   GetID() const;

    virtual void SendMessage( const IGUIParam& Param )=0;
    virtual void UpdateFrame()=0;
    virtual void UpdateDraw( RenderTargetBase& Target )=0;

  protected:
    void SendMessageParent( const IGUIParam& Param );
    virtual void ReturnMessage( ID src, const IGUIParam& Param ){}

  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;
}


#endif
