#ifndef maid2_framework_gui_guiscroller_h
#define maid2_framework_gui_guiscroller_h


#include"../../config/define.h"
#include"guislider.h"


namespace Maid
{
  class GUIScroller : public GUISlider
  {
  public:
    GUIScroller();

		void	SetBarScale( float scale );
		void	SetButtonScale( float scale );

  protected:
    virtual bool IsBarCollision( const POINT2DI& pos, float rad ) const;
    virtual bool IsButtonCollision( const POINT2DI& pos, float rad ) const;

    virtual bool IsBarCollision( const POINT2DI& pos, float scale, float rad ) const=0;
    virtual bool IsButtonCollision( const POINT2DI& pos, float scale, float rad ) const=0;

    virtual VECTOR2DI CalcButtonOffset() const;
    virtual int CalcValue( const POINT2DI& pos ) const;
  private:

    float m_BarScale;
    float m_ButtonScale;
  };


}
#endif