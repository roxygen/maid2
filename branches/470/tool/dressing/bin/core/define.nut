enum FUNCTIONRESULT {
  SUCCESS = 0, // �֐��͐�������
  FAILE   = 1, // �֐��͎��s����(���s�s��)
  WARNING = 2, // �֐��͎��s�������Ǒ����Ă�����
}


enum KEYBOARD
{
// a-z�� 'A' - 'Z' �ł�

  TAB   = 0x09, // tab
  ENTER = 0x0D, // ���s
  SHIFT = 0x10, // �V�t�g
  CTRL  = 0x11, // Ctrl
  ESC   = 0x1B, // �G�X�P�[�v

  SPACE    = 0x20, // �X�y�[�X
  PAGEUP   = 0x21, // �y�[�W�A�b�v
  PAGEDOWN = 0x22, // �y�[�W�_�E��
  END      = 0x23, // �Ō�
  HOME     = 0x24, // �ŏ�

  LEFT   = 0x25, // ��
  UP     = 0x26, // ��
  RIGHT  = 0x27, // ��
  DOWN   = 0x28, // ��

  INSERT = 0x2D, // �}��
  DELETE = 0x2E, // �폜

  NUM0   = 0x30, // �L�[�{�[�h�� '0'
  NUM1   = 0x31, // �L�[�{�[�h�� '1'
  NUM2   = 0x32, // �L�[�{�[�h�� '2'
  NUM3   = 0x33, // �L�[�{�[�h�� '3'
  NUM4   = 0x34, // �L�[�{�[�h�� '4'
  NUM5   = 0x35, // �L�[�{�[�h�� '5'
  NUM6   = 0x36, // �L�[�{�[�h�� '6'
  NUM7   = 0x37, // �L�[�{�[�h�� '7'
  NUM8   = 0x38, // �L�[�{�[�h�� '8'
  NUM9   = 0x39, // �L�[�{�[�h�� '9'

  TEN0   = 0x60, // �e���L�[�� '0'
  TEN1   = 0x61, // �e���L�[�� '1'
  TEN2   = 0x62, // �e���L�[�� '2'
  TEN3   = 0x63, // �e���L�[�� '3'
  TEN4   = 0x64, // �e���L�[�� '4'
  TEN5   = 0x65, // �e���L�[�� '5'
  TEN6   = 0x66, // �e���L�[�� '6'
  TEN7   = 0x67, // �e���L�[�� '7'
  TEN8   = 0x68, // �e���L�[�� '8'
  TEN9   = 0x69, // �e���L�[�� '9'

  F01   = 0x70, // F01
  F02   = 0x71, // F02
  F03   = 0x72, // F03
  F04   = 0x73, // F04
  F05   = 0x74, // F05
  F06   = 0x75, // F06
  F07   = 0x76, // F07
  F08   = 0x77, // F08
  F09   = 0x78, // F09
}

enum MOUSE
{
  X = 0,
  Y = 1,
  WHEEL = 2,

  L = 0x0100,
  R = 0x0101,
  M = 0x0102,
}