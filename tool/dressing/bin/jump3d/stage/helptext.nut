//  �w���v�I�u�W�F�N�g

class HelpText extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
  }
  
  function SetPlayText()
  {
    Type.Text = "���A���ňړ��BZ�ŃW�����v�B���ꂾ���B"
  }
  
  function SetGameOverText()
  {
    Type.Text = "Enter�Ń^�C�g���ɖ߂�"
  }
}


