#ifndef maid2_graphics_core_idevice_h
#define maid2_graphics_core_idevice_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../auxiliary/exception.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/macro.h"
#include"../color.h"
#include"../pixelformat.h"

#include"hobject.h"
#include"createparam.h"
#include"iDrawCommandexecute.h"
#include"iDrawCommandcapture.h"


namespace Maid { namespace Graphics {

  /*!	
      @class	IDevice idevice.h
      @brief	�r�f�I�h���C�o
  \n    			�h���C�o�ɂ���Ă͂ł��邱�ƁA�ł��Ȃ����Ƃ�����܂���
  \n		    	�ł��Ȃ��ꍇ�̓G���[���o���̂ł͂Ȃ��A�ؗ�ɃX���[����悤�ɂ��Ă�������
  \n			    �����^�C���G���[���N�������ꍇ�� Exception �𓊂��邱��
  */

  class IDevice
  {
  public:


    virtual ~IDevice(){}

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o�̏�����
    /*!
        @exception Exception �������Ɏ��s�����ꍇ
     */
    virtual void Initialize() = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o�̊J��
    /*!
        @exception Exception �J���Ɏ��s�����ꍇ
     */
    virtual void Finalize() = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �g�p�\�ȃf�B�X�v���C���[�h�𒲂ׂ�
    /*!
        @param	mode [ o]	�����������\�̐ݒ��
     */
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode ) = 0;



    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂̃f�B�X�v���C���[�h�ō쐬�\�Ȋe��t�H�[�}�b�g�𒲂ׂ�
    /*!
        @param	caps [ o]	�����������\�̐ݒ��
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �X�N���[�����[�h�̕ύX
    /*!
        @param	mode	[i ]	�����炵���𑜓x

        @exception Exception �������Ɏ��s�����ꍇ
     */
    virtual void SetScreenMode( const SCREENMODE& mode )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �t�����g�o�b�t�@�ɍX�V����
    /*!
     */
    virtual void Present()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂̃X�N���[�����[�h�̎擾
    /*!
        @return ���݂̃X�N���[�����[�h
     */
    virtual SCREENMODE GetScreenMode()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �w�肵���I�u�W�F�N�g���폜����
    /*!
        @param Object [i ]  �폜����I�u�W�F�N�g
    \n                      Object.ID==ID_NONE �̏ꍇ�͂Ȃɂ����Ȃ�
     */
    virtual void DeleteObject( const hObject& Object )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �w�肵���I�u�W�F�N�g�̏����擾����
    /*!
        @param Object [i ] �擾����I�u�W�F�N�g
        @param Desc   [ o] �ݒ肷�����
     */
    virtual void GetObjectDesc( const hObject& Object, ObjectDesc& desc )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_��`�̍쐬
    /*!
        @param	Element	  [i ]	�Z�b�g�A�b�v���
        @param	Count	    [i ]	Element �̌�
        @param	pShaderBytecodeWithInputSignature	[i ]	Element��Ή������邽�߂̃V�F�[�_�[
        @param	BytecodeLength	[i ]	pShaderBytecodeWithInputSignature �̒���

        @return	�쐬���ꂽ���_��`
     */
    virtual hInputLayout CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hVertex CreateVertex( const CREATEVERTEXPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �C���f�b�N�X�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hIndex CreateIndex( const CREATEREINDEXPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�c�e�N�X�`���̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hTexture2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �萔�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hConstant CreateConstant( const CREATERECONSTANTPARAM& param )=0;

    virtual hRenderTarget CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )=0;
    virtual hDepthStencil CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )=0;
    virtual hMaterial     CreateMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �o�[�e�b�N�X�V�F�[�_�[�̍쐬
    /*!
      @param	pData   [i ]	�R�[�h
      @param	Length  [i ]	�f�[�^�̒���

      @return	�쐬���ꂽ�o�[�e�b�N�X�V�F�[�_�[
     */
    virtual hVertexShader CreateVertexShader( const void* pData, size_t Length )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�[�����Ǝ��̃s�N�Z���V�F�[�_�[�̍쐬
    /*!
      @param	pData   [i ]	�R�[�h
      @param	Length  [i ]	�f�[�^�̒���

      @return	�쐬���ꂽ�s�N�Z���V�F�[�_�[
     */
    virtual hPixelShader CreatePixelShader( const void* pData, size_t Length )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �e��V�F�[�_�[����R���p�C������
    /*!
        @param	Source  [i ]	�R�[�h
        @param	Binary  [ o]	�R���p�C�����ꂽ�f�[�^
        @param	ErrorMessage  [ o]	�R���p�C�������s�����Ƃ��̃G���[���b�Z�[�W

        @return	�R���p�C���ɐ��������� true
    \n          ���s������ false
     */
    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )=0;


    virtual hSamplerState     CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;
    virtual hRasterizerState  CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;
    virtual hBlendState       CreateBlendState( const BLENDSTATEPARAM& Option )=0;



    virtual IDrawCommandExecute* CreateDrawCommandExecute()=0;
    virtual IDrawCommandCapture* CreateDrawCommandCapture()=0;

    virtual hRenderTarget GetDefaultRenderTarget()=0;
    virtual hDepthStencil GetDefaultDepthStencil()=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif