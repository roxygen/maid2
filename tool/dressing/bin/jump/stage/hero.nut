class Hero extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���

  static STATE_STAND = 0
  static STATE_JUMP = 1
  static WALK_BRAKE = 0.5 // �������x�̃u���[�L
  static WALK_POW   = 1.0 // �������x
  static WALK_POWMAX= 2.0 // �������x�ő�l
  static JUMP_POW   = 1.0 // �c�����x
  static JUMP_POWMAX= 20.0 // �c�����x�ő�l
  

  _State = 0  //  ���݂̏��
  _JumpPow = 0.0    //  �W�����v��
  _Land = null      //  ���ݒ��n���Ă�n��(null�Ȃ畂���Ă�)

  _WalkPow = 0.0    // �������x
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect();//  �F�t�̎l�p�`�����܂�
    Type.Width  = 64;  //  ����pixel
    Type.Height = 64;  //  �c��
    Type.ColorR = 0;   //  �F red
    Type.ColorG = 0;   //  �F green
    Type.ColorB = 0;   //  �F blue
    Type.Alpha  = 1;   //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    Type.CenterX = Type.Width/2;   // �I�t�Z�b�g�B��]�����鎞�ȂǂɎg����ʒu
    Type.CenterY = Type.Height;

     x = 64
    _JumpPow = 0
    _WalkPow = 0
    _State = STATE_STAND
  }

  function UpdateFrame()
  {
    x += _WalkPow;

    switch( _State )
    {
    case STATE_STAND:
      {
        x -= _Land._Speed
      }break;

    case STATE_JUMP:
      {
        y += -_JumpPow  //  y���W�n�����ɐ��Ȃ̂Ŕ��]����
        _JumpPow -= JUMP_POW
      }
    }
    
    { // ��ʊO�ɂłȂ��悤�ɃN���b�v
      if( x< 50.0 ) { x = 50.0 }
      if( 750.0 < x ) { x = 750.0 }
    }
    { // �������x����
           if( _WalkPow < -WALK_BRAKE ) { _WalkPow += WALK_BRAKE; }
      else if( WALK_BRAKE < _WalkPow  ) { _WalkPow -= WALK_BRAKE; }
      else { _WalkPow = 0.0 }
    }
    { //  �c�����x����
      if( _JumpPow < -JUMP_POWMAX ) { _JumpPow = -JUMP_POWMAX }
    }
  }

  function Jump()
  {
    _JumpPow = JUMP_POWMAX
    _State = STATE_JUMP;
    _Land = null;
  }
  
  function Fall()
  {
    _State = STATE_JUMP;
    _JumpPow = 0
    _Land = null;
  }
  
  function Land( block )
  {
    _State = STATE_STAND;
    _JumpPow = 0
    _Land = block;
    
    local rc = block.GetCollision()
    y = rc.top
  }
  
  function HitTop( block )
  {
    _JumpPow = 0
    y = block.GetCollision().bottom + Type.CenterY
  }

  function HitRight( block )
  {
    _WalkPow = 0
    x = block.GetCollision().left - Type.CenterX
  }

  function HitLeft( block )
  {
    _WalkPow = 0
    x = block.GetCollision().right + Type.CenterX
  }

  function WalkR()
  {
    _WalkPow += WALK_POW
    if( _WalkPow<WALK_POWMAX ) { _WalkPow = WALK_POWMAX }
  }
  
  function WalkL()
  {
    _WalkPow -= WALK_POW
    if( -WALK_POWMAX<_WalkPow ) { _WalkPow = -WALK_POWMAX }
  }

  function GetState()
  {
    return _State
  }
  
  function GetTopCollision()
  {
    local tmp = GetCollision()
    return RECT2D(tmp.left+10, tmp.top, tmp.w-10-10, 10 );
  }

  function GetBottomCollision()
  {
    local tmp = GetCollision()
    return RECT2D(tmp.left+10, tmp.bottom-5, tmp.w-10-10, 10 );
  }

  function GetRightCollision()
  {
    local tmp = GetCollision()
    return RECT2D( tmp.right-5, tmp.top+5, 5, tmp.h-5-5 );
  }

  function GetLeftCollision()
  {
    local tmp = GetCollision()
    return RECT2D( tmp.left, tmp.top+5, 5, tmp.h-5-5 );
  }

  function GetCollision()
  {
    return RECT2D( x-Type.CenterX, y-Type.CenterY, Type.Width, Type.Height )
  
  }
}


