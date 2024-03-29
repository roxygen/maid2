﻿/*!
 	@file
 	@brief	乱数生成クラス
 */

#ifndef maid2_auxiliary_rand_h
#define maid2_auxiliary_rand_h

#include"../config/define.h"
#include"../config/typedef.h"

#include <boost/random.hpp>

namespace Maid
{
  class Rand
  {
  public:
    int   GetI( int Min );
    int   GetI( int Min, int Max );
    float GetF( float Min );
    float GetF( float Min, float Max );
    bool  Check(int n);
    void  SetSeed(unt Seed);

  private:
    boost::mt19937  m_Generator;
  };


	class GlobalRand
	{
  public:
    GlobalRand();
		static int	GetI( int Min );
    static int	GetI( int Min, int Max );
    static float GetF( float Min );
    static float GetF( float Min, float Max );
		static bool	Check(int n);
		static void	SetSeed(unt Seed);

  private:
    static Rand s_Rand;
	};

}
#endif
