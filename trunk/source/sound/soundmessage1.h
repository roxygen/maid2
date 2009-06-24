﻿#ifndef soundmessage1_h
#define soundmessage1_h

#include"soundmessagebase.h"
#include"isoundobject.h"
#include"pcmreader/ipcmreader.h"
#include"pcmreader/custompcmreader.h"
#include"pcmreader/pcmbufferlist.h"

#include"core/ibuffer.h"
#include"../auxiliary/memorybuffer.h"

namespace Maid
{
	namespace SoundMessage
	{
		class CreatePCMStatic : public Base
		{
		public:
			CreatePCMStatic() : Base(CREATE_PCMSTATIC){}

      Sound::CREATEBUFFERPARAM Param;
			SPMEMORYBUFFER	pData;	//	PCMデータ
		};

		class CreatePCMStream : public Base
		{
		public:
			CreatePCMStream() : Base(CREATE_PCMSTREAM){}

      Sound::CREATEBUFFERPARAM Param;
    #pragma COMPILERMSG("TODO:pDecoder が pData を持ったり持たなかったりすればいいかも？")	
			SPMEMORYBUFFER    pData;	//	PCMデータ
      SPCUSTOMPCMREADER	pDecoder;	//	デコーダ
		};

		class CreatePCMRealTime : public Base
		{
		public:
			CreatePCMRealTime() : Base(CREATE_PCMREALTIME){}

      Sound::CREATEBUFFERPARAM Param;
			SPPCMBUFFERLIST	pData;	//	PCMデータ
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
			double	Position;
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
			double	Volume;
      double Time;
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
