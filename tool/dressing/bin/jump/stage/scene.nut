import( "hero.nut" );
import( "helptext.nut" );
import( "background.nut" );
import( "block.nut" );
import( "score.nut" );
import( "gameover.nut" );
import( "redygo.nut" );

class SceneStage extends IScene
{
  _bg = null
  _makecount = 0  //  �n�ʂ������^�C�~���O���Ǘ�����J�E���^�B�O�ɂȂ�ƍ쐬����
  _Block = null   //  �n�ʃf�[�^���������z��
  _Hero = null  //  ���@
  _HelpText = null  //  �L�[�w���v

  _Score = null //  �X�R�A�\��

  static STATE_LEDY = 0  //  �Q�[���J�n�O
  static STATE_GAME = 1  //  �Q�[���v���C��
  static STATE_GAMEOVER = 2 // �Q�[���I�[�o�[
  static STATE_END = 3 // �I���҂�

  _State = 0  //  ���݂̃Q�[����ԁiSTATE_LEDY,STATE_GAME,STATE_GAMEOVER�̂ǂꂩ�j

  _RedyGo = null

  function SetState( s )
  {
    _State = s
  }

  function Initialize()
  {
    IScene.Initialize();

    _makecount = 100;
    _Block = []
    _State = STATE_LEDY

    {
      _bg = BackGround();
      _bg.z = 100.0

      GetDrawObjectManager().InsertDrawObject( _bg );
    }

    {
      // �ŏ��̈�̓Q�[�����n�܂�܂ł̈��
      local tmp = Block( 1000, 40, 1 )
      tmp.x = 0;
      tmp.y = 600-40
      GetDrawObjectManager().InsertDrawObject( tmp );
      _Block.push(tmp);
    }
    
    {
      _Hero = Hero();
      GetDrawObjectManager().InsertDrawObject( _Hero );
    }
    {
      _HelpText = HelpText()
      _HelpText.z = -100
      _HelpText.SetPlayText()
      GetDrawObjectManager().InsertDrawObject( _HelpText );
    }
    {
      _Score = Score( _Hero )
      GetDrawObjectManager().InsertDrawObject( _Score );
    }
    {
      _RedyGo = RedyGo()
      GetDrawObjectManager().InsertDrawObject( _RedyGo );
    }
  }

  function Finalize()
  {
    IScene.Finalize();
  }

  function IsExit()
  {
    return _State==STATE_END;
  }
  
  //  rect �ƂԂ����Ă���u���b�N��T��
  //  @return �Ԃ����Ă��� Block  �C���X�^���X
  //          �Ԃ����ĂȂ���� null
  function FindHitBlock( rect )
  {
    foreach( block in _Block )
    {
      local b = block.GetCollision()
      if( COLLISION.RectRect( rect, b ) )
      {
        return block
      }
    }  
    return null
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();

    UpdateGameRule()
    switch( _State )
    {
    case STATE_LEDY:    { UpdateFrameLedy()     }break;
    case STATE_GAME:    { UpdateFrameGame()     }break;
    case STATE_GAMEOVER:{ UpdateFrameGameOver() }break;
    case STATE_END:{ }break;
    }
  }
  

  function UpdateFrameLedy()
  {
    if( _RedyGo.IsEnd() )
    {
      _RedyGo.Kill();
      _Score.Start()
      SetState(STATE_GAME)
    }
  }
  
  function UpdateFrameGame()
  {
    if( GetInput().IsDown(KEYBOARD.LEFT ) ) { _Hero.WalkL() }
    if( GetInput().IsDown(KEYBOARD.RIGHT) ) { _Hero.WalkR() }
    if( GetInput().IsIn('Z')&& _Hero.GetState()==Hero.STATE_STAND) { _Hero.Jump() }
    
    
    //  ���@����ʉ��ɏ�������Q�[���I�[�o�[
    if( 700 < _Hero.y )
    {
      local tmp = GameOver()
      GetDrawObjectManager().InsertDrawObject( tmp )
      
      _Score.Stop()
      _HelpText.SetGameOverText()
      SetState(STATE_GAMEOVER)

    }
  }
  
  function UpdateFrameGameOver()
  {
    if( GetInput().IsIn(KEYBOARD.ENTER) ) { SetState(STATE_END) }
  
  }
  
  function UpdateGameRule()
  {
    {
      // ���Ƃ̓����蔻��B
      local block = FindHitBlock( _Hero.GetBottomCollision() );
      
      // �W�����v���ƒ��n���ŋ������Ⴄ�m�F
      if( _Hero.GetState()==Hero.STATE_STAND )
      {
        // ���Ƃ̓����蔻��B���ɂԂ����ĂȂ��Ȃ痎������
        // �Ԃ����Ă���Ȃ�u���b�N�̃X�s�[�h���������ֈړ�����i����Ă镵�͋C�j
        if( block==null )
        {
          _Hero.Fall()
        }
      }
      else if( _Hero.GetState()==Hero.STATE_JUMP )
      {
        if( block != null )
        {
          _Hero.Land( block )
        }
      }
    }
    
    {
      // �V��Ƃ̓����蔻��
      local block = FindHitBlock( _Hero.GetTopCollision() );
      if( block!=null )
      {
        _Hero.HitTop( block )
      }    
    }

    {
      // �E�ǂƂ̓����蔻��
      local block = FindHitBlock( _Hero.GetRightCollision() );
      if( block!=null )
      {
        _Hero.HitRight( block )
      }
    }
    {
      // ���ǂƂ̓����蔻��
      local block = FindHitBlock( _Hero.GetLeftCollision() );
      if( block!=null )
      {
        _Hero.HitLeft( block )
      }
    }

    //  ����Ȃ��Ȃ����ǂ̍폜
    //  ���ɋ߂����Ƀ\�[�g����Ă�͂��Ȃ̂ŁA(0,y)���߂��������폜����
    while( true )
    {
      if( _Block.len()==0 ) { break; }

      local w = _Block[0].x + _Block[0].Type.Width
      if( w < 0 ) { _Block.remove(0); }
      else { break; }
    }

    GameLevelCheck();  
  }

  function GameLevelCheck()
  {
    // �V�����ǂ����
    if( _makecount==0 )
    {
      local w = RandI(100,200)
      local tmp = Block( w, 40, 1 )
      tmp.x = 800;
      tmp.y = 600-100 - RandI(0,100)

      GetDrawObjectManager().InsertDrawObject( tmp );
      _Block.push(tmp);

      _makecount = RandI(200,500)
   }else
   {
      _makecount--
   }
  }

}


