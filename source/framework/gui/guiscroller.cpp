#include"guiscroller.h"




namespace Maid
{


GUIScroller::GUIScroller()
:m_ValueMin(0), m_ValueMax(1), m_Value(0), m_Rotat(0), m_IsButtonDown(false)
,m_BarLength(1),m_ButtonLength(1)
{

}

void	GUIScroller::SetBarLength( int len )
{
  MAID_ASSERT( len<=0, "０以下は指定できません" );
  m_BarLength = len;
}

void	GUIScroller::SetButtonLength( int len )
{
  MAID_ASSERT( len<=0, "０以下は指定できません" );
  m_ButtonLength = len;
}


void	GUIScroller::SetValueRange( int min, int max )
{
  m_ValueMin = min;
  m_ValueMax = max;

  if( m_Value < m_ValueMin ) { SetValue(m_ValueMin); }
  ef( m_ValueMax < m_Value ) { SetValue(m_ValueMax); }
}

void	GUIScroller::SetValue( int Value )
{
  if( m_Value==Value ) { return ; } //  無駄な発行はしない

  GUIMESSAGE_SCROLLBAR_CHANGEVALUE mess;
  mess.Value = Value;
  SendMessage( mess );
}

int		GUIScroller::GetValue() const
{
  return m_Value;
}

void GUIScroller::SetRotate( float rad )
{
  if( rad<DEGtoRAD(-90.0f) || DEGtoRAD(90.0f)<rad ) { m_Rotat = fmod(rad,90.0f); }
  else { m_Rotat = rad; }
}

float GUIScroller::GetRotate() const
{
  return m_Rotat;
}

bool GUIScroller::IsButtonDown() const
{
  return m_IsButtonDown;
}










bool GUIScroller::LocalIsCollision( const POINT2DI& pos ) const
{
  if( IsBarCollision( pos, m_BarLength, m_Rotat ) ) { return true; }

  const VECTOR2DI ButtonOffset = CalcButtonOffset();

  return IsButtonCollision( pos-ButtonOffset, m_ButtonLength, m_Rotat );
}


VECTOR2DI GUIScroller::CalcButtonOffset() const
{
  const int p = m_Value-m_ValueMin;
  const int vec = m_ValueMax-m_ValueMin;
  const int scale = m_BarLength - m_ButtonLength;

  const int pos = (p * scale / vec);

  const float x = float(pos) * Math<float>::cos(m_Rotat) + 0.5f;
  const float y = float(pos) * Math<float>::sin(m_Rotat) + 0.5f;

  return VECTOR2DI(int(x),int(y));
}

int GUIScroller::CalcValue( const POINT2DI& pos ) const
{
  //  pos はスライダが配置してあるところからどれだけ離れているかが入っているので
  //  m_Rotat を使って逆算すれば、直線の位置になる。

  int line_pos; //  直線に戻したときの位置

  if( Math<float>::abs(m_Rotat) < DEGtoRAD(45.0f) )
  {
    line_pos = int( float(pos.x) / Math<float>::cos(m_Rotat) +0.5f);
  }else
  {
    line_pos = int( float(pos.y) / Math<float>::sin(m_Rotat) +0.5f);
  }
  
  int NewValue;
  {
    const int vec = m_BarLength - m_ButtonLength;
    const int p   = line_pos - vec/2;
    const int scale   = m_ValueMax-m_ValueMin;

    NewValue = p * scale / vec + m_ValueMin;
  }

  {
    //  クリップする
    if( NewValue < m_ValueMin ) { NewValue = m_ValueMin; }
    if( m_ValueMax < NewValue ) { NewValue = m_ValueMax; }
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

GUIScroller::MESSAGERETURN GUIScroller::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {     
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*pParam);
      const POINT2DI pos = m.Position-CalcTotalOffset();

      if( IsButtonCollision(pos - CalcButtonOffset(),m_BarLength, m_Rotat) )
      {
        m_IsButtonDown = true;
        OnButtonDown(pos);
      }
      ef( IsBarCollision(pos,m_ButtonLength, m_Rotat) )
      {
        //  バーをクリックしたら、そこに移動してほしいよね。
        const int NewValue = CalcValue(pos);
        if( NewValue!=m_Value ) //  無駄な発行はしない
        {
          GUIMESSAGE_SLIDER_CHANGEVALUE mess;
          mess.Value = NewValue;
          PostMessage( mess );
        }
      }

    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*pParam);

      if( m_IsButtonDown )
      {
        const POINT2DI pos = m.Position-CalcTotalOffset() - CalcButtonOffset();
        m_IsButtonDown = false;
        OnButtonUp(pos);
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
      const POINT2DI ButtonOffset = p.Position-CalcTotalOffset();

      //  ボタンをつかんだときだけボタンの再配置を行う
      if( !m_IsButtonDown ) { break; }

      const int NewValue = CalcValue( ButtonOffset );

      if( NewValue!=m_Value )//  無駄な発行はしない
      {
        GUIMESSAGE_SLIDER_CHANGEVALUE mess;
        mess.Value = NewValue;
        PostMessage( mess );
      }
    }break;

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



}
