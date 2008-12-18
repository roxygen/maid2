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

#include"hObject.h"
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

    struct DISPLAYMODE
    {
      SIZE2DI     Size;         //!<  ��ʉ𑜓x
      PIXELFORMAT Format;       //!<  �X�N���[�����[�h
      int         RefreshRate;  //!<  ���t���b�V�����[�g
    };

    struct SCREENMODE
    {
      DISPLAYMODE DisplayMode;  //!<  �𑜓x
      bool        IsFullScreen; //!<  �t���X�N���[���ɂ��邩�H
      bool        IsWaitVSync;  //!<  VSync�������邩�H
    };

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


    struct ENABLEFORMAT
    {
      std::vector<PIXELFORMAT>  Texture;              //!<  �쐬�\�ȃe�N�X�`���t�H�[�}�b�g
      std::vector<PIXELFORMAT>  RenderTargetTexture;  //!<  �쐬�\�ȃ����_�[�e�N�X�`���t�H�[�}�b�g
      std::vector<PIXELFORMAT>  RenderTargetSurface;  //!<  �쐬�\�ȃ����_�[�T�[�t�F�X�t�H�[�}�b�g
      std::vector<DEPTHSTENCILFORMAT>  DepthStencil;  //!<  �쐬�\��DepthStencil�t�H�[�}�b�g
      std::vector<int>          IndexBit;             //!<  �쐬�\�ȂP�C���f�b�N�X������̃r�b�g��
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂̃f�B�X�v���C���[�h�ō쐬�\�Ȋe��t�H�[�}�b�g�𒲂ׂ�
    /*!
        @param	caps [ o]	�����������\�̐ݒ��
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �X�N���[�����[�h�̕ύX
    /*!
        �f�o�C�X���X�g����̕��A�ɂ��Ă�ŉ�����

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
        @param Object [i ] �폜����I�u�W�F�N�g
     */
    virtual void DeleteObject( const hObject& Object )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �w�肵���I�u�W�F�N�g�̏����擾����
    /*!
        @param Object [i ] �擾����I�u�W�F�N�g
        @param Desc   [ o] �ݒ肷�����
     */
    virtual void GetObjectDesc( const hObject& Object, ObjectDesc& desc )=0;


    struct CREATEVERTEXPARAM
    {
      const void* pData;  //  ���_�f�[�^
      size_t Length;      //  �f�[�^�̒���(byte�P��)
      unt32 Format;       //  ���_�t�H�[�}�b�g
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hVertex CreateVertex( const CREATEVERTEXPARAM& param )=0;


    struct CREATEREINDEXPARAM
    {
      const void* pData;  //  �C���f�b�N�X�f�[�^
      size_t  Length;     //  �f�[�^�̒���(byte�P��)
      unt32   Format;     //  �P�C���f�b�N�X������̃r�b�g��
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �C���f�b�N�X�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hIndex CreateIndex( const CREATEREINDEXPARAM& param )=0;


    struct CREATERETEXTURE2DPARAM
    {
      const void* pData;  //  �s�N�Z���f�[�^
      PIXELFORMAT Format; //  �s�N�Z���t�H�[�}�b�g
      int   ArraySize;    //  �������邩�H
      int   Pitch;        //  �P���C��������̑傫��(byte�P��)
      int   Slice;        //  �P���ʂ�����̑傫��(byte�P��)
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�c�e�N�X�`���̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hTexture2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param )=0;


    struct CREATERECONSTANTPARAM
    {
      const void* pData;  //  �o�b�t�@�f�[�^
      size_t  Length;     //  �f�[�^�̒���(byte�P��)
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �萔�o�b�t�@�̍쐬
    /*!
        @param	param   [i ]	�쐬���������\�[�X

        @return	�쐬���ꂽ���\�[�X
     */
    virtual hConstant CreateConstant( const CREATERECONSTANTPARAM& param )=0;


    struct CREATERENDERTARGETPARAM
    {
      enum DIMENSION
      {
        DIMENSION_BUFFER,
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
        DIMENSION_TEXTURE3D,
      };

      PIXELFORMAT Format; //  �z�肳����t�H�[�}�b�g
      DIMENSION   Dimension;
      unt32       Param[4];
    };

    virtual hRenderTarget CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )=0;


    struct CREATEDEPTHSTENCILPARAM
    {
      enum DIMENSION
      {
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
      };
      enum FLAG
      {
        FLAG_READ_ONLY_DEPTH   = 0x01,
        FLAG_READ_ONLY_STENCIL = 0x02,
      };

      DEPTHSTENCILFORMAT Format; //  �z�肳����t�H�[�}�b�g
      DIMENSION          Dimension;
      unt32       ReadOnlyUsage;
      unt32       Param[4];
    };

    virtual hDepthStencil CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )=0;

   struct CREATESHADERMATERIALPARAM
    {
      enum DIMENSION
      {
        DIMENSION_BUFFER,
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
        DIMENSION_TEXTURE3D,
        DIMENSION_TEXTURECUBE,
        DIMENSION_TEXTURECUBEARRAY,
        DIMENSION_BUFFEREX,
      };
      enum FLAG
      {
        FLAG_READ_ONLY_DEPTH   = 0x01,
        FLAG_READ_ONLY_STENCIL = 0x02,
      };

      DEPTHSTENCILFORMAT Format; //  �z�肳����t�H�[�}�b�g
      DIMENSION          Dimension;
      unt32       Param[4];
    };
    virtual hMaterial CreateShaderMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )=0;


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
    \n        ���s������ false
     */
    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )=0;

    enum COMPARISON
    {
      COMPARISON_NEVER        = 1,	//!<	��ɒʂ�
      COMPARISON_LESS         = 2,	//!<	o >  n
      COMPARISON_EQUAL		    = 3,	//!<	o == n
      COMPARISON_LESSEQUAL	  = 4,	//!<	o >= n
      COMPARISON_GREATER		  = 5,	//!<	o <  n
      COMPARISON_NOTEQUAL     = 6,	//!<	o != n
      COMPARISON_GREATEREQUAL = 7,	//!<	o <= n
      COMPARISON_ALWAYS       = 8,	//!<	��Ƀ_��
    };

    struct SAMPLERSTATEPARAM
    {
      enum FILTER
      {
        FILTER_POINT,
        FILTER_LINEAR,
        FILTER_ANISOTROPIC,
      };

      enum ADDRESS
      {
        ADDRESS_WRAP,
        ADDRESS_MIRROR,
        ADDRESS_CLAMP,
        ADDRESS_BORDER,
        ADDRESS_MIRROR_ONCE,
      };



      FILTER  MinFilter;
      FILTER  MagFilter;

      int   MaxAnisotropy;

      ADDRESS AddressU;
      ADDRESS AddressV;
      ADDRESS AddressW;

      float MipLODBias;
      COMPARISON   ComparisonFunc;
      float BorderColor[4];
      float MinLOD;
      float MaxLOD;
    };
    virtual hSamplerState CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;

    struct RASTERIZERSTATEPARAM
    {
      enum CULLING
      {
        CULLING_NONE=0,
        CULLING_LEFT,	//!<	�����v���ɃJ�����O
        CULLING_RIGHT	//!<	���v���ɃJ�����O
      };

      CULLING Culling;
      bool MultiSample;
    };
    virtual hRasterizerState CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;

    struct BLENDSTATEPARAM
    {
      enum BLEND
      {
        BLEND_ZERO,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DEST_ALPHA,
        BLEND_INV_DEST_ALPHA,
        BLEND_DEST_COLOR,
        BLEND_INV_DEST_COLOR,
        BLEND_SRC_ALPHA_SAT,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
        BLEND_SRC1_COLOR,
        BLEND_INV_SRC1_COLOR,
        BLEND_SRC1_ALPHA,
        BLEND_INV_SRC1_ALPHA,
      };

      enum OPERATION
      {
        OPERATION_ADD,
        OPERATION_SUBTRACT,
        OPERATION_REV_SUBTRACT,
        OPERATION_MIN,
        OPERATION_MAX,
      };

      enum MASK
      {
        MASK_RED  =0x01,
        MASK_GREEN=0x02,
        MASK_BLUE =0x04,
        MASK_ALPHA=0x08,
        MASK_ALL = MASK_RED|MASK_GREEN|MASK_BLUE|MASK_ALPHA
      };

      bool  AlphaToCoverageEnable;
      bool  BlendEnable[8];

      BLEND SrcBlend;
      BLEND DestBlend;
      OPERATION BlendOp;
      BLEND SrcBlendAlpha;
      BLEND DestBlendAlpha;
      OPERATION BlendOpAlpha;
      unt08 RenderTargetWriteMask[8];
    };

    virtual hBlendState CreateBlendState( const BLENDSTATEPARAM& Option )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o���ŗp�ӂ���Ă��钸�_��`�̍쐬
    /*!
      @param	Name	[i ]	�f�t�H���g��

      @return	�쐬���ꂽ���_��`
     */
    struct INPUT_ELEMENT
    {
      enum TYPE
      {
        TYPE_FLOAT1,
        TYPE_FLOAT2,
        TYPE_FLOAT3,
        TYPE_FLOAT4,
        TYPE_COLOR,
      };
      enum METHOD {
          METHOD_DEFAULT = 0,
          METHOD_PARTIALU = 1,
          METHOD_PARTIALV = 2,
          METHOD_CROSSUV = 3,
          METHOD_UV = 4,
          METHOD_LOOKUP,
          METHOD_LOOKUPPRESAMPLED
      };

      char*  SemanticName;
      int    SemanticIndex;
      TYPE   Type;
      int    SlotNo;
      int    Offset;
      METHOD Method;
    };
    virtual hInputLayout CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;


    virtual IDrawCommandExecute* CreateDrawCommandExecute()=0;
    virtual IDrawCommandCapture* CreateDrawCommandCapture()=0;

    virtual hRenderTarget GetDefaultRenderTarget()=0;
    virtual hDepthStencil GetDefaultDepthStencil()=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif