#ifndef oggcontainer_h
#define oggcontainer_h


#include"../../source/config/define.h"
#include"../../source/storage/fileio/filereadnormal.h"

#include<ogg/ogg.h>

#include"oggpage.h"

class OggContainer
{
public:

  OggContainer();
  ~OggContainer();

  void Initialize( const Maid::String& FileName );
  void Finalize();

  const OggPage& GetCurrentPage()const;
  void NextPage();
  void Reset();

  bool IsEnd() const;

private:
  void UpdateBuffer();

private:
  Maid::FileReadNormal m_hFile;
  ogg_sync_state  m_State;

  OggPage m_CurrentPage;
};


#endif
