#include"id3d09object.h"

namespace Maid { namespace Graphics {

std::set<ID3D09Object*>  ID3D09Object::s_ObjectList;

ID3D09Object::ID3D09Object()
{
  s_ObjectList.insert(this);
}

ID3D09Object::~ID3D09Object()
{
  s_ObjectList.erase(this);
}

}}

