#include"guiscroller.h"




namespace Maid
{


GUIScroller::GUIScroller()
:m_BarScale(1),m_ButtonScale(1)
{

}

void	GUIScroller::SetBarScale( float scale )
{
  MAID_ASSERT( scale<1, "１未満は指定できません" );
  m_BarScale = scale;
}

void	GUIScroller::SetButtonScale( float scale )
{
  MAID_ASSERT( scale<1, "１未満は指定できません" );
  m_ButtonScale = scale;
}




bool GUIScroller::IsBarCollision( const POINT2DI& pos, float rad ) const
{
  return IsBarCollision( pos, m_BarScale, GetRotate() );
}

bool GUIScroller::IsButtonCollision( const POINT2DI& pos, float rad ) const
{
  return IsButtonCollision( pos, m_ButtonScale, GetRotate() );
}


VECTOR2DI GUIScroller::CalcButtonOffset() const
{
  const int p = GetValue()-GetValueMin();
  const int vec = GetValueMax()-GetValueMin();
  const float scale = (m_BarScale - m_ButtonScale) * (float)GetBarLength();

  const float pos = (p * scale / vec);

  const float rot = GetRotate();
  const float x = float(pos) * Math<float>::cos(rot) + 0.5f;
  const float y = float(pos) * Math<float>::sin(rot) + 0.5f;

  return VECTOR2DI(int(x),int(y));
}

int GUIScroller::CalcValue( const POINT2DI& pos ) const
{
  //  pos はスライダが配置してあるところからどれだけ離れているかが入っているので
  //  m_Rotat を使って逆算すれば、直線の位置になる。

  int line_pos; //  直線に戻したときの位置

  const float rot = GetRotate();

  if( Math<float>::abs(rot) < DEGtoRAD(45.0f) )
  {
    line_pos = int( float(pos.x) / Math<float>::cos(rot) +0.5f);
  }else
  {
    line_pos = int( float(pos.y) / Math<float>::sin(rot) +0.5f);
  }
  
  int NewValue;
  {
    const int vec = int((m_BarScale - m_ButtonScale) * (float)GetBarLength());
    const int p   = line_pos - vec/2;
    const int scale   = GetValueMax()-GetValueMin();

    NewValue = p * scale / vec + GetValueMin();
  }

  {
    //  クリップする
    if( NewValue < GetValueMin() ) { NewValue = GetValueMin(); }
    if( GetValueMax() < NewValue ) { NewValue = GetValueMax(); }
  }

  return NewValue;
}
/*

バー　|----i----|  
ボタン   |-i-|

最小  0         m_BarLength
バー　|----i----|  
ボタン|-i-|
      0   m_ButtonLength

最大  0         m_BarLength
バー　|----i----|  
ボタン      |-i-|
            0   m_ButtonLength

    int m_BarLength;
    int m_ButtonLength;

    int m_ValueMin;
    int m_ValueMax;
    int m_Value;

*/
/*
GUIScroller::MESSAGERETURN GUIScroller::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_SLIDER_CHANGEVALUE:
    {
      const GUIMESSAGE_SLIDER_CHANGEVALUE& m = static_cast<const GUIMESSAGE_SLIDER_CHANGEVALUE&>(*pParam);

      //  変化ないなら破棄だよね。
      if( m_Value==m.Value ) { return MESSAGERETURN_NONE; }

      m_Value = m.Value;
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );
}
*/


}
