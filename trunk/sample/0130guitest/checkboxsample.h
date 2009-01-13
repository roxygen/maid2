#ifndef sample_0130guitest_checkboxsample_h
#define sample_0130guitest_checkboxsample_h

#include"../../source/config/define.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphicsrender.h"



class CheckBoxSample : public Maid::GUICheckBox
{
public:
  void Initialize( Maid::GraphicsRender* r );

protected:
    virtual MESSAGERETURN OnInitialize();
    virtual MESSAGERETURN OnFinalize();
    virtual bool LocalIsCollision( const Maid::POINT2DI& pos )const;
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );
    virtual MESSAGERETURN OnMouseIn( const Maid::POINT2DI& pos );
    virtual MESSAGERETURN OnMouseOut( const Maid::POINT2DI& pos );
    virtual MESSAGERETURN OnMouseDown( const Maid::POINT2DI& pos, int Button );
    virtual MESSAGERETURN OnMouseUp( const Maid::POINT2DI& pos, int Button );
    virtual MESSAGERETURN OnStateChange( bool IsCheck );

private:
  Maid::GraphicsRender*  m_pRender;
  Maid::SIZE2DI m_Size;
  Maid::Font m_hFont;
};


#endif