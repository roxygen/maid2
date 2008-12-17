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
  struct VERTEXSHADEROPTION
  {
    Graphics::hVertexShader hObject;
  };

  class VertexShader 
      :public IVertexShader
      ,public GlobalPointer<Graphics::Core>
      ,public StorageObjectTemplate<String,VERTEXSHADEROPTION>
  {
  public:
    VertexShader();


	protected:
		virtual void DeleteFirst( const String& id  );
		virtual void LoadEndFirst( const IStorageFunction* param, VERTEXSHADEROPTION& option  );
		virtual void LoadEnd( const VERTEXSHADEROPTION& option  );
    virtual IStorageFunction* CreateFunction( const String& id );
  };	
	
}

#endif