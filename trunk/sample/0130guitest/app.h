#ifndef sample_0130guitest_app_h
#define sample_0130guitest_app_h

#include"../../source/config/define.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/gui.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"


#include"buttonsample.h"
#include"checkboxsample.h"
#include"radiobuttonsample.h"
#include"slidersample.h"


class MyApp : public Maid::IGameThread
{
protected:
  bool SelectDevice( const std::vector<Maid::Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();


private:
  void ReciveMessage( Maid::IGUIParts::ID id, const Maid::IGUIParam& param );


private:
  Maid::GraphicsRender  m_Render;

  Maid::Font m_hFont;
  std::list<Maid::String> m_MessageLog;

  int m_Frame;
private:
  ButtonSample      m_Button;
  CheckBoxSample    m_CheckBox;
  RadioButtonSample m_RadioButton[3];
  SliderSample      m_Slider;
  SliderSample      m_Slider20;
  SliderSample      m_Slider20b;
  Maid::GUIRadioGroup     m_RadioGroup;
  Maid::GUIManager      m_Manager;
};

#endif