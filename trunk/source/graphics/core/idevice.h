#ifndef maid2_graphics_core_idevice_h
#define maid2_graphics_core_idevice_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../auxiliary/exception.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/macro.h"

#include"../../auxiliary/string.h"
#include"../color.h"
#include"../pixelformat.h"

#include"createparam.h"


#include"ibuffer.h"
#include"itexture2d.h"
#include"iconstant.h"
#include"irendertarget.h"
#include"idepthstencil.h"
#include"imaterial.h"
#include"iinputlayout.h"
#include"irasterizerstate.h"
#include"isamplerstate.h"
#include"iblendstate.h"
#include"idepthstencilstate.h"
#include"ivertexshader.h"
#include"ipixelshader.h"
#include"idrawcommandexecute.h"
#include"idrawcommandcapture.h"


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
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode ) const= 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���̃A�_�v�^���g�p�\�ȃf�B�X�v���C���j�^�𒲂ׂ�
    /*!
        @param	info [ o] ���j�^�̗�
     */
    virtual void SerchDisplayInfo( std::vector<DISPLAYINFO>& info ) const=0;



    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂̃f�B�X�v���C���[�h�ō쐬�\�Ȋe��t�H�[�}�b�g�𒲂ׂ�
    /*!
        @param	caps [ o]	�����������\�̐ݒ��
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps )  const= 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �f�B�X�v���C�̃A�X�y�N�g����擾����
    /*!
        @return �A�X�y�N�g��i�Ƃ����Ă��c�Ɖ��̑傫�������邾���ł�)
     */
    virtual SIZE2DI GetAspectRatio() const=0;

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
    virtual SCREENMODE GetScreenMode() const=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_��`�̍쐬
    /*!
        @param	Element	  [i ]	�Z�b�g�A�b�v���
        @param	Count	    [i ]	Element �̌�
        @param	pShaderBytecodeWithInputSignature	[i ]	Element��Ή������邽�߂̃V�F�[�_�[
        @param	BytecodeLength	[i ]	pShaderBytecodeWithInputSignature �̒���

        @return	�쐬���ꂽ���_��`
     */
    virtual SPINPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�c�e�N�X�`���̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual SPTEXTURE2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )=0;

    virtual SPRENDERTARGET CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM* param )=0;
    virtual SPDEPTHSTENCIL CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM* param )=0;
    virtual SPMATERIAL     CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �o�[�e�b�N�X�V�F�[�_�[�̍쐬
    /*!
      @param	pData   [i ]	�R�[�h
      @param	Length  [i ]	�f�[�^�̒���

      @return	�쐬���ꂽ�o�[�e�b�N�X�V�F�[�_�[
     */
    virtual SPVERTEXSHADER CreateVertexShader( const void* pData, size_t Length )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�[�����Ǝ��̃s�N�Z���V�F�[�_�[�̍쐬
    /*!
      @param	pData   [i ]	�R�[�h
      @param	Length  [i ]	�f�[�^�̒���

      @return	�쐬���ꂽ�s�N�Z���V�F�[�_�[
     */
    virtual SPPIXELSHADER CreatePixelShader( const void* pData, size_t Length )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �e��V�F�[�_�[����R���p�C������
    /*!
        @param	Source  [i ]	�R�[�h
        @param	Binary  [ o]	�R���p�C�����ꂽ�f�[�^
        @param	ErrorMessage  [ o]	�R���p�C�������s�����Ƃ��̃G���[���b�Z�[�W

        @return	�R���p�C���ɐ��������� true
    \n          ���s������ false
     */

    enum CODETYPE
    {
      CODETYPE_VERTEXSHADER,
      CODETYPE_PIXELSHADER,
    };

    virtual bool CompileShaderLanguage( const String& Source, CODETYPE type, std::vector<unt08>& Binary, String& ErrorMessage )=0;


    virtual SPSAMPLERSTATE      CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;
    virtual SPRASTERIZERSTATE   CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;
    virtual SPDEPTHSTENCILSTATE CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option )=0;
    virtual SPBLENDSTATE        CreateBlendState( const BLENDSTATEPARAM& Option )=0;

    virtual SPDRAWCOMMANDEXECUTE GetDrawCommandExecute()const=0;
    virtual SPDRAWCOMMANDCAPTURE CreateDrawCommandCapture()=0;

    //! ����Present()���ɕ\�ɏo�Ă���RenderTarget���擾����
    /*!
        @return	RenderTarget
     */
    virtual SPRENDERTARGET GetCurrentRenderTarget()const=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif