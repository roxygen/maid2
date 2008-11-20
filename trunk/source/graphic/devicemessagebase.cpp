#include"devicemessagebase.h"
//#include"CRenderMessageImpl1.h"
//#include"CRenderMessageImpl2.h"
//#include"CRenderMessageImpl3.h"


namespace Maid
{
	using namespace DeviceMessage;
	String Base::GetMessage() const
	{
		switch( Message )
		{
		case NONE: { return MAIDTEXT("NONE"); }break;
/*
		case RENDERTARGET_CLEAR_COLOR: { return MAIDTEXT("RENDERTARGET_CLEAR_COLOR"); }break;
		case RENDERTARGET_CLEAR_Z: { return MAIDTEXT("RENDERTARGET_CLEAR_Z"); }break;
		case SETTEXTURE: { return MAIDTEXT("SETTEXTURE"); }break;
		case SETVERTEX:
			{
				SetVertex* p = (SetVertex*)this;
				return MAIDTEXT("SETVERTEX:") + p->ID; 
			}break;
		case SETINDEX: { return MAIDTEXT("SETINDEX"); }break;
		case SETVERTEXSHADER: { return MAIDTEXT("SETVERTEXSHADER"); }break;
		case SETPIXELSHADER: { return MAIDTEXT("SETPIXELSHADER"); }break;
		case DRAWPRIMITIVE: { return MAIDTEXT("DRAWPRIMITIVE"); }break;
		case DRAWPRIMITIVEUP: { return MAIDTEXT("DRAWPRIMITIVEUP"); }break;
*/
		case CREATERENDERTARGETSURFACE: { return MAIDTEXT("CREATERENDERTARGETSURFACE"); }break;
		case CREATEDEPTHSTENCILSURFACE: { return MAIDTEXT("CREATEDEPTHSTENCILSURFACE"); }break;
		case CREATETEXTURE: { return MAIDTEXT("CREATETEXTURE"); }break;
		case CREATERENDERTARGETTEXTURE: { return MAIDTEXT("CREATERENDERTARGETTEXTURE"); }break;
		case CREATEVERTEX:
			{
			//	RegistVertex* p = (RegistVertex*)(this);
			//	return MAIDTEXT("CREATEVERTEX") + p->ResistID; 
			}break;

		case CREATEINDEX: { return MAIDTEXT("CREATEINDEX"); }break;
		case CREATEVERTEXSHADER:
			{
			//	RegistVertexShader* p = (RegistVertexShader*)(this);
			//	return MAIDTEXT("CREATEVERTEXSHADER") + p->ResistID;
			}break;
		case CREATEPIXELSHADER:
			{
			//	RegistPixelShader* p = (RegistPixelShader*)(this);
			//	return MAIDTEXT("CREATEPIXELSHADER") + p->ResistID;
			}break;
		case DELETESURFACE: { return MAIDTEXT("DELETESURFACE"); }break;
		case DELETETEXTURE: { return MAIDTEXT("DELETETEXTURE"); }break;
		case DELETEVERTEX:
			{
			//	DeleteVertex* p = (DeleteVertex*)(this);
			//	return MAIDTEXT("DELETEVERTEX") + p->DeleteID; 
			}break;
		case DELETEINDEX: { return MAIDTEXT("DELETEINDEX"); }break;
		case DELETEVERTEXSHADER: { return MAIDTEXT("DELETEVERTEXSHADER"); }break;
		case DELETEPIXELSHADER: { return MAIDTEXT("DELETEPIXELSHADER"); }break;
		}

		return String();
	}


}


