/*!
 	@file 
 	@brief	ＸＭＬドキュメント
 */

#ifndef CXMLDocument_h
#define maid2_auxiliary_xml_xmldocument_h

#include"../config/define.h"
#include"xmlnode.h"
#include<string>

namespace Maid
{
	class XMLDocument
	{
	public:
    void Parse( const std::string& TextImage );


		XMLNode& GetRoot();

	private:

		struct ATTRIBUTE
		{
			String Name;
			String Value;
		};

		void DivAttribute( const String& Element, String& Name, std::vector<ATTRIBUTE>& Attribute );
		SPXMLNODE ReadChildNode( const String& token );


	private:
		SPXMLNODE	m_pRootNode;
	};

}
#endif
