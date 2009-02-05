#ifndef soundmessage1_h
#define soundmessage1_h

#include"soundmessagebase.h"
#include"isoundobject.h"
#include"pcmreader/ipcmreader.h"

#include"core/ibuffer.h"
#include"../auxiliary/buffer.h"

namespace Maid
{
	namespace SoundMessage
	{
		class CreatePCMStatic : public Base
		{
		public:
			CreatePCMStatic() : Base(CREATE_PCMSTATIC){}

      Sound::CREATEBUFFERPARAM Param;
			SPBUFFER	pData;	//	PCM�f�[�^
		};

		class CreatePCMStream : public Base
		{
		public:
			CreatePCMStream() : Base(CREATE_PCMSTREAM){}

      Sound::CREATEBUFFERPARAM Param;
    #pragma COMPILERMSG("TODO:pDecoder �� pData ���������莝���Ȃ������肷��΂��������H")	
			SPBUFFER    pData;	//	PCM�f�[�^
      SPPCMREADER	pDecoder;	//	�f�R�[�_
		};

		class CreateClone : public Base
		{
		public:
			CreateClone() : Base(CREATE_CLONE){}

      SPSOUNDOBJECTINFO pSrc;
		};

    class DeleteObject : public Base
		{
		public:
			DeleteObject() : Base(DELETEOBJECT){}
		};


		class Play : public Base
		{
		public:
			Play() : Base(PLAY){}
		};

		class Stop : public Base
		{
		public:
			Stop() : Base(STOP){}
		};


		class SetPosition : public Base
		{
		public:
			SetPosition() : Base(SETPOSITION){}
			float	Position;
		};


    class SetJumpPoint : public Base
		{
		public:
			SetJumpPoint() : Base(SETJUMPPOINT){}

      std::vector<JUMPPOINT>   List;
		};

		class SetVolume : public Base
		{
		public:
			SetVolume() : Base(SETVOLUME){}
			float	Volume;
      float Time;
		};

		class SetLoopState : public Base
		{
		public:
			SetLoopState() : Base(SETLOOPSTATE){}
			bool	IsLoop;
		};
  }
}

#endif
