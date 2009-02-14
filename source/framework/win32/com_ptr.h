/*!
    @file
    @brief	COM �p�X�}�[�g�|�C���^
 */

#ifndef maid2_framework_win32_com_ptr_h
#define maid2_framework_win32_com_ptr_h

#include"../../config/define.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include<objbase.h>

namespace Maid
{ 
	/*!
      @brief	COM �p�X�}�[�g�|�C���^
  \n			�Q�Ƃ��Ȃ��Ȃ����Ƃ��� Release() ����܂�
	 */
  template <class ComObject>
  class com_ptr
  {
  public:
    com_ptr() : m_pInterface(NULL) {}								//!<	�R���X�g���N�^���̂P
    explicit com_ptr( ComObject* pInterface ) : m_pInterface(pInterface){}	//!<	�R���X�g���N�^���̂Q
    com_ptr(const com_ptr<ComObject>& pObject )						//!<	�R���X�g���N�^���̂R
    {
      m_pInterface = pObject.m_pInterface;
      if( !IsNull() )
      {
	      m_pInterface->AddRef();
      }
    }

    ~com_ptr()	//!<	�f�X�g���N�^
    {
      reset();
    }

    //! ���̃N���X�̏�����
    /*!
        @param	pInterface	[i ]	�V�����Ǘ�����b�n�l�i�f�t�H���g�łm�t�k�k�j
     */
    void reset( ComObject* pInterface=NULL )	//!<	�f�X�g���N�^
    {
      if( m_pInterface==pInterface ) { return ; }

      if( !IsNull() )
      {
	      m_pInterface->Release();
      }
      m_pInterface = pInterface;
    }

    //! �R�s�[
    /*!
        @param	NewCom	[i ]	���L����b�n�l
     */
    com_ptr<ComObject>& operator=( com_ptr<ComObject> const & NewCom )
    {
      if( m_pInterface==NewCom.get() ) { return *this; }
      reset();
      m_pInterface = NewCom.get();
      m_pInterface->AddRef();
      return *this;
    }

    bool IsNull() const { return m_pInterface==NULL; }	//!<	�����Ǘ����Ă��Ȃ����H

    ComObject* get() const 	{ return m_pInterface;	}	//!<	�b�n�l�擾
    ComObject* operator->() const //!<	�ێ����Ă���b�n�l�𓧉ߓI�Ɉ���
    {
      MAID_ASSERT( get()==NULL, "�܂��ݒ肳��Ă��܂���" );
      return get();  
    }		

    //! COM�I�u�W�F�N�g�̍쐬
    /*!
        @param  ClassID       [i ]	CoCreateInstance() �̃w���v���Q��
        @param	pUnknown      [i ]	CoCreateInstance() �̃w���v���Q��
        @param	ClassContext  [i ]	CoCreateInstance() �̃w���v���Q��
        @param	riid          [i ]	CoCreateInstance() �̃w���v���Q��

        @exception Exception COM�I�u�W�F�N�g�̍쐬�Ɏ��s�����ꍇ
     */
    void CoCreateInstance( REFCLSID ClassID, LPUNKNOWN pUnknown, DWORD ClassContext, REFIID riid )
    {
      reset();
      const HRESULT ret = ::CoCreateInstance( ClassID, pUnknown, ClassContext, riid, (void **)&m_pInterface );

      if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("CoCreateInstance() �Ɏ��s") ); }
    }

    //! COM�I�u�W�F�N�g�̍쐬
    /*!
      @param	riid		[i ]	QueryInterface() �̃w���v���Q��
      @param	pObj		[ o]	�쐬���ꂽCOM�I�u�W�F�N�g

      @exception Exception COM�I�u�W�F�N�g�̍쐬�Ɏ��s�����ꍇ
     */
    template <class QueryObject>
    void QueryInterface( REFIID riid, com_ptr<QueryObject>& pObj )
    {
      MAID_ASSERT( get()==NULL, "�܂��ݒ肳��Ă��܂���" );

      QueryObject* p;
      const HRESULT ret = m_pInterface->QueryInterface( riid, (void **)&p );
      if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("QueryInterface() �Ɏ��s") ); }

      pObj.reset(p);
    }

  private:
	  ComObject*	m_pInterface;
  };

  template<class ComObject> 
  inline bool operator==(const com_ptr<ComObject>  & lhs, const com_ptr<ComObject>  & rhs)
  {
    return lhs.get() == rhs.get();
  }

  template<class ComObject> 
  inline bool operator!=(const com_ptr<ComObject>  & lhs, const com_ptr<ComObject>  & rhs)
  {
    return !(lhs == rhs);
  }
}
#endif
