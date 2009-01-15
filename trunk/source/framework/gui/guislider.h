#ifndef maid2_framework_gui_guislider_h
#define maid2_framework_gui_guislider_h


#include"../../config/define.h"
#include"../../auxiliary/mathematics/line1d.h"
#include"iguiparts.h"


namespace Maid
{
  class GUISlider : public IGUIParts
  {
  public:
    GUISlider();

    void	SetValueMin( int min );
    void	SetValueMax( int max );

    int GetValueMin() const;
    int GetValueMax() const;


		void	SetValue( int Value );
		int		GetValue() const;

    void SetRotate( float rad );
    float GetRotate() const;

    bool IsButtonDown() const;
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBarCollision( const POINT2DI& pos, float rad ) const= 0;
    virtual bool IsButtonCollision( const POINT2DI& pos, float rad ) const= 0;

    virtual void OnButtonDown( const POINT2DI& pos ){}
    virtual void OnButtonUp( const POINT2DI& pos ){}

    virtual int GetBarLength() const=0;

    virtual VECTOR2DI CalcButtonOffset() const;
    virtual int CalcValue( const POINT2DI& pos ) const;


  private:

    int m_ValueMin;
    int m_ValueMax;
    int m_Value;
    float m_Rotat;

    bool m_IsButtonDown;//!<	スライダを動かしている最中か？
  };


}
#endif