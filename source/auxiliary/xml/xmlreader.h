/*!
 	@file 
 	@brief	ＸＭＬ読み込みクラス
 */

#ifndef maid2_auxiliary_xml_xmlreader_h
#define maid2_auxiliary_xml_xmlreader_h

#include"../config/define.h"
#include"xmldocument.h"
#include"../functionresult.h"

#include<stack>

namespace Maid
{
	class XMLReader
	{
	public:
		FUCTIONRESULT Load( const std::string& FileName );

		void NextNode();
		bool IsEndNode();
		void AscendNode();
		void DescendNode();

		bool IsTopNode();

		enum NODETYPE
		{
			NODETYPE_EMPTY,		//	改行のみとか空っぽ
			NODETYPE_ELEMENT,	//	データがある<a>cccc</a> 等
			NODETYPE_TAG,		//	子持ちデータ<a>・・・(中にいろいろある)・・・</a> 等
		};

		NODETYPE GetNodeType()const;


		String GetNodeName()const;
		String GetString()const;
		int GetInteger()const;
		float GetFloat()const;
		XMLText GetAttribute( const String& Name )const;

	private:
		XMLNode& GetCurrentNode()const;
		XMLNode& GetTextNode( XMLNode& parent )const;

	private:
		XMLDocument	m_Docment;

		struct NODESTACK
		{
			NODESTACK( XMLNode& n )
				:ParentNode(n)
			{

				ChildPos = 0;
			}

			XMLNode& ParentNode;
			int	ChildPos;
		};

		std::stack<NODESTACK>	m_ParentStack;
	};

	class XMLReaderAutoDescend_base
	{
	public:
		XMLReaderAutoDescend_base( XMLReader& r )
			:m_Reader(r)
		{
			m_Reader.AscendNode();
		}

		~XMLReaderAutoDescend_base()
		{
			m_Reader.DescendNode();
		}
	private:
		XMLReader& m_Reader;
	};

	#define XMLReaderAutoDescend2( a, writer ) XMLReaderAutoDescend_base p##a(writer)
	#define XMLReaderAutoDescend( writer )  XMLReaderAutoDescend2(__LINE__, writer)

}
#endif
