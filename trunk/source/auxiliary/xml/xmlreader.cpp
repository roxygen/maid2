#include"xmlreader.h"
#include"../debug/assert.h"
#include"../debug/trace.h"


namespace Maid
{
	/*!
	 	@class	XMLReader xmlreader.h
	 	@brief	XMLを順番に読んでいくクラス
	 */

FUCTIONRESULT XMLReader::Load( const std::string& TextImage )
{
	m_Docment.Parse(TextImage);


	XMLNode& node = m_Docment.GetRoot();

	m_ParentStack.push( NODESTACK(node) );

  return FUCTIONRESULT_OK;
}

void XMLReader::NextNode()
{
	NODESTACK& s = m_ParentStack.top();

	if( s.ChildPos < s.ParentNode.GetChildLength() )
	{
		++s.ChildPos;
	}
}

bool XMLReader::IsEndNode()
{
	if( m_ParentStack.empty() ) { return true; }

	NODESTACK& s = m_ParentStack.top();

	return s.ChildPos == s.ParentNode.GetChildLength();
}

bool XMLReader::IsTopNode()
{
	return m_ParentStack.empty();
}

void XMLReader::AscendNode()
{
	NODETYPE t = GetNodeType();

	MAID_ASSERT( t!=NODETYPE_TAG&&t!=NODETYPE_EMPTY, "子持ちではありません" );

	XMLNode& node = GetCurrentNode();

//	MAID_TRACE( MAIDTEXT("AscendNode()") << node.GetName() );
	m_ParentStack.push( NODESTACK(node) );
}

void XMLReader::DescendNode()
{
  if( m_ParentStack.size()==1 ) { return ; }
/*
	if( !m_ParentStack.empty() )
	{
		XMLNode& node = m_ParentStack.top().ParentNode;
		MAID_TRACE( MAIDTEXT("DescendNode()") << node.GetName() );
	}else
	{
		MAID_TRACE( MAIDTEXT("DescendNode()")  );

	}
*/
	m_ParentStack.pop();
}

XMLReader::NODETYPE XMLReader::GetNodeType()const
{
	XMLNode& node = GetCurrentNode();

	if( node.GetType()!=XMLNode::TYPE_ELEMENT ) { return NODETYPE_EMPTY; }


	int text_count=0;
	int element_count=0;
	int comment_count=0;
	for( int i=0; i<node.GetChildLength(); ++i )
	{
		switch( node.GetChildNode(i).GetType() )
		{
		case XMLNode::TYPE_TEXT:	 { ++text_count;	}break;
		case XMLNode::TYPE_ELEMENT: { ++element_count; }break;
		case XMLNode::TYPE_COMMENT: { ++comment_count; }break;

		case XMLNode::TYPE_EMPTY:
		default:					{ MAID_ASSERT( true, "ここはありえない" ); }break;
		}
	}

	if( element_count==0 )
	{
		if( text_count==1 ) { return NODETYPE_ELEMENT; }
		return NODETYPE_EMPTY;
	}else
	{
		return NODETYPE_TAG;
	}

	//	そうでないならデータ
	return NODETYPE_ELEMENT;

}

XMLNode& XMLReader::GetCurrentNode()const
{
	const NODESTACK& s = m_ParentStack.top();

	return s.ParentNode.GetChildNode(s.ChildPos);
}

String XMLReader::GetNodeName()const
{
	XMLNode& node = GetCurrentNode();

	return String::ToLower(node.GetName());
}


String XMLReader::GetString()const
{
	//	テキストとして読み込む場合は空データを認める
	const NODETYPE type = GetNodeType();

	MAID_ASSERT( type!=NODETYPE_ELEMENT && type!=NODETYPE_EMPTY, "データではありません" );

	if( type==NODETYPE_EMPTY ) { return String(); }

	XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetStr();
}

int XMLReader::GetInteger()const
{
	MAID_ASSERT( GetNodeType()!=NODETYPE_ELEMENT, "データではありません" );

	XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetInt();
}

float XMLReader::GetFloat()const
{
	MAID_ASSERT( GetNodeType()!=NODETYPE_ELEMENT, "データではありません" );

	XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetFloat();
}

XMLText XMLReader::GetAttribute( const String& Name )const
{
	XMLNode& n = GetCurrentNode();

	return n.GetAttribute( Name );
}


XMLNode& XMLReader::GetTextNode( XMLNode& parent )const
{
	for( int i=0; i<parent.GetChildLength(); ++i )
	{
		XMLNode& n = parent.GetChildNode(i);
		if( n.GetType()==XMLNode::TYPE_TEXT )
		{
			return n;
		}
	}

	MAID_ASSERT( true, "テキストノードが見つかりませんでした" );

	return parent;	//	←これコンパイラ対策なので、これ自体に意味はありません
}

}