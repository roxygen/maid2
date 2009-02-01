#ifndef oggbuffer_h
#define oggbuffer_h


#include"../../source/config/define.h"
#include"../../source/storage/fileio/filereadnormal.h"

#include<ogg/ogg.h>

#include"oggpage.h"

class OggBuffer
{
public:

  OggBuffer();
  ~OggBuffer();

  void Initialize();
  void Finalize();

  const OggPage& GetCurrentPage()const;
  void NextPage();

  bool IsEnd() const;

private:
  void UpdateBuffer();

private:
  Maid::FileReadNormal m_hFile;
  ogg_sync_state  m_State;

  OggPage m_CurrentPage;
};


#endif
