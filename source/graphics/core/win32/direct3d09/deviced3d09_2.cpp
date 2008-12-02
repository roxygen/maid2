#include"deviced3d09.h"


namespace Maid { namespace Graphics {

IDevice::OBJECT DeviceD3D09::CreateObject( const SPOBJECTINFO& pObj )
{
  if( (int)m_ObjectEmptyList.size() <= m_ObjectUseCount )
  { //  すでに作成されているオブジェクト個数の限界を迎えたら、増やす

    const int ADD = 100; //  増やす個数。100は適当
    const int prev = (int)m_ObjectEmptyList.size();
    const int new_count = prev + ADD;

    m_Object.resize( new_count );
    m_ObjectEmptyList.resize( new_count );
    for( int i=0; i<ADD; ++i ){ m_ObjectEmptyList[prev+i] = prev+i; }
  }


  const int no = m_ObjectEmptyList[m_ObjectUseCount];

  ++m_ObjectUseCount;

  m_Object[no] = pObj;


  return IDevice::OBJECT(no);
}

void DeviceD3D09::DeleteObject( const OBJECT& Object )
{
  //  すぐに削除せずに溜めておくのは、DrawCommandRecorder 対策
  //  Flip() 後に破棄されます
  m_DeleteObjectList.push_back( Object );
}

IDevice::RESOURCE DeviceD3D09::CreateResource( const CREATERESOURCEPARAM& Param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  IDevice::OBJECT new_obj;


  switch( Param.Type )
  {
  case CREATERESOURCEPARAM::TYPE_VERTEX:
    {
      const CREATEVERTEXPARAM& param = static_cast<const CREATEVERTEXPARAM&>(Param);
      const UINT  length = param.Length;
      const DWORD usage = 0;
      const DWORD fvf = VERTEXFORMAT2FVF( param.Format );
      const D3DPOOL pool = D3DPOOL_DEFAULT;

      SPD3D09VERTEXBUFFER pBuffer;
      {
        IDirect3DVertexBuffer9* p=NULL;
        const HRESULT ret = pDev->CreateVertexBuffer( length, usage, fvf, pool, &p, NULL );
        if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateVertexBuffer()")); }
        pBuffer.reset( p );
      }

      {
        VOID* pDst = NULL;

        {
          const HRESULT ret = pBuffer->Lock( 0, 0, &pDst, D3DLOCK_DISCARD );
          if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::Lock()")); }
        }

        ::memcpy( pDst, param.pData, param.Length );

        {
          const HRESULT ret = pBuffer->Unlock();
          if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::Unlock()")); }
        }
      }

      boost::shared_ptr<VertexBufferInfo>  pInfo(new VertexBufferInfo);

      pInfo->pVertex = pBuffer;

      new_obj = CreateObject( pInfo );

    }break;
  case CREATERESOURCEPARAM::TYPE_INDEX:
    {

    }break;
  case CREATERESOURCEPARAM::TYPE_TEXTURE2D:
    {

    }break;
  case CREATERESOURCEPARAM::TYPE_CONSTANT:
    {

    }break;
  }

  return RESOURCE(new_obj.ID);
}

/*

    virtual RENDERTARGET CreateRenderTarget( const RESOURCE& resource, const CREATERENDERTARGETPARAM& param );
    virtual DEPTHSTENCIL CreateDepthStencil( const RESOURCE& resource, const CREATEDEPTHSTENCILPARAM& param );
    virtual SHADERMATERIAL CreateShaderMaterial( const RESOURCE& resource, const CREATESHADERMATERIALPARAM& param );

    virtual VERTEXSHADER CreateVertexShader( const void* pData, size_t Length );
    virtual PIXELSHADER CreatePixelShader( const void* pData, size_t Length );

    virtual SAMPLERSTATE CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual RASTERIZERSTATE CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual BLENDSTATE& CreateBlendState( const BLENDSTATEPARAM& Option );
    virtual INPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );
*/

}}


