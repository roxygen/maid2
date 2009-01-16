#ifndef sample_0130guitest_slidersample_h
#define sample_0130guitest_slidersample_h

#include"../../source/config/define.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphicsrender.h"


#include"../../source/framework/gui/guislider.h"

class SliderSample : public Maid::GUISlider
{
public:
  void Initialize( Maid::GraphicsRender* r );

protected:
    virtual void OnInitialize( ID id, const IGUIParts& Parent );
    virtual void OnFinalize();
    virtual bool IsBarCollision( const Maid::POINT2DI& pos, float rad ) const;
    virtual bool IsButtonCollision( const Maid::POINT2DI& pos, float rad ) const;

    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );
    virtual void OnMouseMove( const Maid::POINT2DI& pos );
    virtual void OnMouseIn( const Maid::POINT2DI& pos );
    virtual void OnMouseOut( const Maid::POINT2DI& pos );
    virtual void OnButtonDown( const Maid::POINT2DI& pos );
    virtual void OnButtonUp( const Maid::POINT2DI& pos );

private:
  Maid::GraphicsRender*  m_pRender;
  Maid::Font m_hFont;
};


#endif