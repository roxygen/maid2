﻿/*
  シーン切り替え表示サンプル
  SceneText,SceneImage はそれぞれ自分のメンバだけで画面を構築してて
  SceneAdmin　がシーン間の移動を管理してます。
*/

#include"../../source/framework/gameframework.h"

#include"app.h"


using namespace Maid;

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new App) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

