#include"pixelshaderd3d09fixed.h"

namespace Maid { namespace Graphics {



PS_FIXED::PS_FIXED( int no )
 :m_No(no)
{
}

void PS_FIXED::Setup( const SPD3D09DEVICE& pDevice )
{
  switch( m_No )
  {
  case 200:
    { //	���_�F�����̂܂ܗ���
	    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );
	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
    }break;


  case 210:
    {
      //	�e�N�X�`���ƒ��_�F�̍���
	    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	    pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
    }break;

  case 211:
    {
      //	�e�N�X�`���ƒ��_�F�̍���
      //  ���A��ɃA���t�@�������Ă��܂�
	    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	    pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_MODULATE );
	    pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT|D3DTA_ALPHAREPLICATE );
	    pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );

	    pDevice->SetTextureStageState( 2, D3DTSS_COLOROP,	D3DTOP_DISABLE );

	    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
	    pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
    }break;
  }
}


}}

