#ifndef maid2_framework_gui_guimanager_h
#define maid2_framework_gui_guimanager_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../input/mouse.h"
#include"../../input/keybord.h"
#include"../../auxiliary/mathematics.h"
#include"iguiparts.h"
#include"guimessage.h"
#include"guimessageque.h"
#include"guicallback.h"
#include<list>

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
    void DeleteParts( IGUIParts& Parts );
    void DeletePartsAll();

    void UpdateFrame( const Mouse* pMouse, const Keybord* pKey );
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth );

  private:
    IGUIParts* FindParts( const POINT2DI& pos );

    RECIVEFUNCTION  m_ReciveFunction;

    GUICallback  m_Root;
    GUIMessageQue m_MessageQue;

  private:  //  マウスがらみで使うパラメータ
    void UpdateMouseMessage( const Mouse& m, GUIMessageQue& Que );
    IGUIParts*  m_pMouseCapturingParts; //  マウスがキャプチャされているとき（Ｌボタン押してるとき等）のポインタ
    IGUIParts*  m_pMousePrevParts;      //  １フレーム前にマウスが乗っていたパーツ
  };

  #define MakeReciveMessageFunction(pFunc,pObject) boost::bind( pFunc, pObject, _1, _2 )

}




#endif
