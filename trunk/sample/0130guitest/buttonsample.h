#ifndef sample_0130guitest_buttonsample_h
#define sample_0130guitest_buttonsample_h

#include"../../source/config/define.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/framework/gui.h"


class ButtonSample : public Maid::IGUIButton
{
public:
    void Initialize( Maid::GraphicsRender* r );

protected:
    virtual void OnInitialize( ID id, const IGUIParts& Parent );
    virtual void OnFinalize();
    virtual bool LocalIsCollision( const Maid::POINT2DI& pos )const;
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );
    virtual void OnMouseMove( const Maid::POINT2DI& pos );
    virtual void OnMouseIn( const Maid::POINT2DI& pos );
    virtual void OnMouseOut( const Maid::POINT2DI& pos );

private:
  Maid::GraphicsRender*  m_pRender;
  Maid::SIZE2DI m_Size;
  Maid::Font m_hFont;
};


#endif