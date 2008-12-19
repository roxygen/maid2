#ifndef maid2_graphics_vertexshader_h
#define maid2_graphics_vertexshader_h

/*!
    @file
    @brief  vertexshader
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"../storage/storageobjecttemplate.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{

  class VertexShaderBase
      :public IVertexShader
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    struct OPTION
    {
      Graphics::hVertexShader hObject;
    };

  protected:
		void CreateOption( const IStorageFunction* param, OPTION& option  );
		void LocalSetup( const OPTION& option  );
		void LocalClear();
		void DeleteOption( const OPTION& option );
    IStorageFunction* CreateFunction( const String& id );
    String ConvertID( const String& id );
  };	
  
  typedef StorageObjectTemplate<String,VertexShaderBase::OPTION,VertexShaderBase> VertexShader;
	
}

#endif