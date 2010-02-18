﻿#include"textinput.h"



namespace Maid
{
  /*!
      @class  TextInput textinput.h
      @brief  ユーザーが入力した文字とかＩＭとかを受け取るクラス
  \n          なんとなくで用意してるので、もう少し拡張が必要
  */

TextInput::TextInput()
:m_IMCompPosition(0)
,m_IsEvent(false)
,m_IsIMOpen(false)
{

}

TextInput::~TextInput()
{
  Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
 	  @param	pChara	[i ]	新しく設定するドライバ
 	  @param	pInput	[i ]	新しく設定するドライバ
 */
void TextInput::Initialize( const Input::SPCHARCODEDEVICE& pChara, const Input::SPINPUTMETHODDEVICE& pInput )
{
  m_pChara = pChara;
  m_pInput = pInput;

  m_pChara->Initialize();
  m_pInput->Initialize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 解放
/*!
 */
void TextInput::Finalize()
{
  if( m_pInput.get()!=NULL )
  {
    m_pInput->Finalize();
    m_pInput.reset();
  }

  if( m_pChara.get()!=NULL )
  {
    m_pChara->Finalize();
    m_pChara.reset();
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 状態を更新する
/*!
    フレーム毎に１回呼び出すこと
 */
void TextInput::Update()
{
  //  コールバックにするほうがいいかも。
  //  ポーリングだといろいろめんどい

  String chara, comp;
  int pos = -1;
  bool IsOpen = false;

  m_pChara->Flush(chara);
  if( m_pInput->IsOpen() )
  {
    IsOpen = true;
    comp = m_pInput->GetCompString();
    pos = m_pInput->GetCursorPos();
  }

  m_IsEvent =     chara!=m_CharaCode
              ||  comp!=m_IMComp
              ||  pos!=m_IMCompPosition
              ||  IsOpen!=m_IsIMOpen
              ;

  m_CharaCode = chara;
  m_IMComp = comp;
  m_IMCompPosition = pos;
  m_IsIMOpen = IsOpen;

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 前フレームと比べて、なにか起こっているか
/*!
    @return なにかおこっているなら true
 */
bool TextInput::IsEvent() const
{
  return m_IsEvent;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 今回のフレームで入力されたキーコードを取得する
/*!
    @return キーコード
 */
const String& TextInput::GetCharaCode() const
{
  return m_CharaCode;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 今回のフレームで入力された変換候補を取得する
/*!
    @return 候補
 */
const String& TextInput::GetIMCompString() const
{
  return m_IMComp;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 今回のフレームで入力された変換カーソルを取得する
/*!
    @return カーソル位置（文字数）
 */
int TextInput::GetIMCompPosition() const
{
  return m_IMCompPosition;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! IMがひらいているか？
/*!
    @return キーコード
 */
bool TextInput::IsIMOpen() const
{
  return m_IsIMOpen;
}



const Input::SPCHARCODEDEVICE& TextInput::GetCharaCodeDevice() const
{
  return m_pChara;
}

const Input::SPINPUTMETHODDEVICE& TextInput::GetInputMethodDevice() const
{
  return m_pInput;
}


}

