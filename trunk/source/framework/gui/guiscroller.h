#ifndef maid2_framework_gui_guiscroller_h
#define maid2_framework_gui_guiscroller_h


#include"../../config/define.h"
#include"iguiparts.h"


namespace Maid
{
  class GUIScroller : public IGUIParts
  {
  public:
    GUIScroller();

		void	SetBarLength( int len );
		void	SetButtonLength( int len );


		void	SetValueRange( int min, int max );
		void	SetValue( int Value );
		int		GetValue() const;

    void SetRotate( float rad );
    float GetRotate() const;

    bool IsButtonDown() const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBarCollision( const POINT2DI& pos, int len, float rad ) const= 0;
    virtual bool IsButtonCollision( const POINT2DI& pos, int len, float rad ) const= 0;

    virtual void OnButtonDown( const POINT2DI& pos ){}
    virtual void OnButtonUp( const POINT2DI& pos ){}

    VECTOR2DI CalcButtonOffset() const;
    int CalcValue( const POINT2DI& pos ) const;
  private:

    int m_BarLength;
    int m_ButtonLength;

    int m_ValueMin;
    int m_ValueMax;
    int m_Value;
    float m_Rotat;

    bool m_IsButtonDown;//!<	スライダを動かしている最中か？
  };


}
#endif