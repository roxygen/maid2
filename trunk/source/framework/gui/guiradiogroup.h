#ifndef maid2_framework_gui_guiradiogroup_h
#define maid2_framework_gui_guiradiogroup_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guiradiobutton.h"

#include<vector>

namespace Maid
{
  class GUIRadioGroup : public IGUIParts
  {
  public:

    enum
    {
      SELECT_NONE=-1, //  選択されていない
    };

    void SetRadioButton( const std::vector<SPGUIRADIOBUTTON>& RadioButtonList );

    int GetCheckNo() const;
    void SetCheck( int pos );

  protected:
    SPGUIPARTS LocalFindChild( const POINT2DI& pos ) const;
    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const;
    virtual void OnSendMessage( const IGUIParam& Param );
    virtual void ReciveMessage( ID src, const IGUIParam& Param );
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& offset );

    virtual void OnGroupMouseMove( int no, const POINT2DI& pos ){}
    virtual void OnGroupMouseIn( int no, const POINT2DI& pos ){}
    virtual void OnGroupMouseOut( int no, const POINT2DI& pos ){}

    virtual void OnGroupMouseDown( int no, const POINT2DI& pos, int Button ){}
    virtual void OnGroupMouseUp( int no, const POINT2DI& pos, int Button ){}

  private:
    std::vector<SPGUIRADIOBUTTON> m_Button;
    int m_SelectNo;
  };


}
#endif