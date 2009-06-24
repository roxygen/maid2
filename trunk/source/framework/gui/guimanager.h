﻿#ifndef maid2_framework_gui_guimanager_h
#define maid2_framework_gui_guimanager_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../input/mouse.h"
#include"../../input/keybord.h"
#include"../../input/textinput.h"
#include"../../auxiliary/mathematics.h"
#include"iguiparts.h"
#include"guimessage.h"
#include"guimessageque.h"
#include"guicallback.h"
#include"iguicontextmenu.h"
#include<list>
#include<boost/bind.hpp>

namespace Maid
{

  class GUIManager
  {
  public:
    GUIManager();
    virtual ~GUIManager();


    typedef GUICallback::RECIVEFUNCTION	RECIVEFUNCTION;
    void SetReviceFunction( const RECIVEFUNCTION& Func );

    void RegisterParts( IGUIParts::ID id, IGUIParts& Parts );
    void DeleteParts( IGUIParts::ID id );
    void DeletePartsAll();

    void UpdateFrame( const Mouse* pMouse, const Keybord* pKey, const TextInput* pText );
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth );

    void SetFocus( IGUIParts::ID id );

    IGUIParts::ID GetFocus() const;

    void BeginInterruptParts( const POINT2DI& pos, IGUIParts::ID id, IGUIParts& parts );

  private:
    void ReciveMessage( IGUIParts::ID src, const IGUIParam& Param );
    IGUIParts* FindParts( const POINT2DI& pos );
    IGUIParts* FindChildParts( const POINT2DI& pos, IGUIParts& parts );
    IGUIParts* FindChildParts( IGUIParts::ID id, IGUIParts& parts );

    void MakeMessageSETKEYBORDFOCUS( IGUIParts* pParts, GUIMessageQue& Que );
    void MakeMessageMOUSEDOWN( IGUIParts* pTarget, const POINT2DI& pos, IGUIParam::BUTTONTYPE type, GUIMessageQue& Que );
    void MakeMessageMOUSEUP( IGUIParts* pTarget, const POINT2DI& pos, IGUIParam::BUTTONTYPE type, GUIMessageQue& Que );


    RECIVEFUNCTION  m_ReciveFunction;

    GUICallback  m_Root;
    GUIMessageQue m_MessageQue;
    RECIVEFUNCTION  m_ReceiveFunction;

  private:  //  マウスがらみで使うパラメータ
    void UpdateMouseMessage( const Mouse& m, GUIMessageQue& Que );
    void UpdateKeybordMessage( const Keybord& k, GUIMessageQue& Que );
    void UpdateTextMessage( const TextInput& t, GUIMessageQue& Que );
    IGUIParts*  m_pMouseCapturingParts; //  マウスがキャプチャされているとき（Ｌボタン押してるとき等）のポインタ
    IGUIParts*  m_pMousePrevParts;      //  １フレーム前にマウスが乗っていたパーツ
    IGUIParts*  m_pKeybordFocus;        //  キーボードフォーカス
    IGUIParts*  m_pInterruptParts;      //  設定されている割り込みパーツ
  };

  #define MakeReciveMessageFunction(pFunc,pObject) boost::bind( pFunc, pObject, _1, _2 )

}




#endif
