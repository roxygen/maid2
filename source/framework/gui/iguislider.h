﻿#ifndef maid2_framework_gui_iguislider_h
#define maid2_framework_gui_iguislider_h


#include"../../config/define.h"
#include"../../auxiliary/mathematics/line1d.h"
#include"iguiparts.h"


namespace Maid
{
  class IGUISlider : public IGUIParts
  {
  public:
    IGUISlider();

    void	SetValueMin( int min );
    void	SetValueMax( int max );
		void	SetValue( int Value );

    int GetValueMin() const;
    int GetValueMax() const;
		int	GetValue() const;

    void SetRotate( float rad );
    float GetRotate() const;

    void SetBarLength( int len );
    int GetBarLength() const;

    void SetButtonLength( int len );
    int GetButtonLength() const;

    bool IsButtonDown() const;
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBarCollision( const POINT2DI& pos ) const;
    virtual bool IsButtonCollision( const POINT2DI& pos ) const;

    virtual void OnButtonDown( const POINT2DI& pos );
    virtual void OnButtonUp( const POINT2DI& pos );


    VECTOR2DI CalcButtonOffset() const;
    int CalcValue( const POINT2DI& pos ) const;


  private:

    int m_ValueMin;
    int m_ValueMax;
    int m_Value;
    float m_Rotat;

    bool  m_IsButtonDown;//!<	スライダを動かしている最中か？
    int   m_BarLength;
    int   m_ButtonLength;
  };


}
#endif