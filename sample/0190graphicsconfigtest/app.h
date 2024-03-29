﻿#ifndef sample_0190graphicsconfigtest_app_h
#define sample_0190graphicsconfigtest_app_h
/*
  ゲーム用アプリケーションの雛形サンプル
*/


#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/framework/gui.h"
#include"../../source/graphics/texture2d.h"
#include"../../source/graphics/font.h"

#include"configwindow.h"
#include"textbutton.h"
#include"qualityslider.h"
#include"checkbox.h"
#include"textradiobutton.h"

class MyApp : public Maid::IGameThread
{
public:
  MyApp();

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();

  void ReciveMessage( Maid::IGUIParts::ID id, const Maid::IGUIParam& param );

private:

  void ReloadImage();
  bool IsImageLoading();

private:
  enum STATE
  {
    STATE_LOADING,
    STATE_EXECUTING
  };

  STATE            m_State;
  Maid::Texture2D  m_BG;
  Maid::Texture2D  m_PixivTan;
  Maid::Texture2D  m_ITChan;

  int   m_PixivTanAngle;

  Maid::Graphics2DRender::TEXTUREFILTER m_TextureFilter;
  bool  m_IsShowPanel;

private:
  Maid::GraphicsCommandControl  m_Command;
  Maid::Graphics2DRender  m_Render;

  Maid::Font          m_FontM;
  int m_FrameCount;
  int m_DrawCount;
  float m_FPS;


  CheckBox m_MipMap;

  QualitySlider m_TextureQuality;
  TextButton    m_ReloadButton;
  TextButton    m_Screen640x480;
  TextButton    m_Screen800x600;
  TextButton    m_Screen1024x768;
  TextRadioButton    m_FilterPoint;
  TextRadioButton    m_FilterBilinear;
  Maid::GUIRadioGroup     m_FilterGroup;
  ConfigWindow  m_Window;

  Maid::GUIManager      m_Manager;

};

#endif

