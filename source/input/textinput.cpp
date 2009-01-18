#include"textinput.h"



namespace Maid
{

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

void TextInput::Initialize( const Input::SPCHARCODEDEVICE& pChara, const Input::SPINPUTMETHODDEVICE& pInput )
{
  m_pChara = pChara;
  m_pInput = pInput;

  m_pChara->Initialize();
  m_pInput->Initialize();

}

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

bool TextInput::IsEvent() const
{
  return m_IsEvent;
}

const String& TextInput::GetCharaCode() const
{
  return m_CharaCode;
}

const String& TextInput::GetIMCompString() const
{
  return m_IMComp;
}

int TextInput::GetIMCompPosition() const
{
  return m_IMCompPosition;
}

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

