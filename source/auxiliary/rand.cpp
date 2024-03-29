﻿#include "rand.h"
#include "debug/assert.h"

#include<ctime>

namespace Maid
{
/*!
 	@class	Rand rand.h
 	@brief	乱数生成クラス。生成される値を確実にしたい向け
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 0<= x <= Max の範囲で乱数を取得する
/*!
 	@param	Max	[i ]	最大値
 */
int	Rand::GetI( int Max )
{
  return GetI( 0, Max );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Min <= x < Max の範囲で乱数を取得する
/*!
 	@param	Min	[i ]	最小値
 	@param	Max	[i ]	最大値
 */
int	Rand::GetI( int Min, int Max )
{
	MAID_ASSERT( !(Min<=Max), "引数の指定が反対です" <<  Min << " <= " << Max );

  boost::uniform_smallint<> dst( Min, Max );
  boost::variate_generator<
			  boost::mt19937&, boost::uniform_smallint<>
		  > die( m_Generator, dst );
	return die();
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 0<= x <= Max の範囲で乱数を取得する
/*!
 	@param	Max	[i ]	最大値
 */
float	Rand::GetF( float Max )
{
  return GetF( 0.0f, Max );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Min <= x < Max の範囲で乱数を取得する
/*!
 	@param	Min	[i ]	最小値
 	@param	Max	[i ]	最大値
 */
float	Rand::GetF( float Min, float Max )
{
	MAID_ASSERT( !(Min<=Max), "引数の指定が反対です" <<  Min << " <= " << Max );

  boost::uniform_real<> dst( Min, Max );
  boost::variate_generator<
			  boost::mt19937&, boost::uniform_real<>
		  > die( m_Generator, dst );
	return (float)die();
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 1/n の確率を満たしているか調べる
/*!
 	@param	n	[i ]	確率
 */
bool Rand::Check(int n)
{
	MAID_ASSERT( n<=0, "0以下は指定できません " <<  n );

	return GetI(0,n-1)==0;
}


void Rand::SetSeed( unt Seed )
{
	m_Generator.seed( boost::uint32_t(Seed) );
}




/*!
 	@class	GlobalRand rand.h
 	@brief	乱数生成クラス。値そのものはどうでもいい向け
 */
Rand  GlobalRand::s_Rand;


GlobalRand::GlobalRand()
{
  SetSeed( static_cast<unt>(std::time(0)) );
}

int	GlobalRand::GetI( int Max ){ return s_Rand.GetI(Max); }
int	GlobalRand::GetI( int Min, int Max ){ return s_Rand.GetI(Min,Max); }
float	GlobalRand::GetF( float Max ){ return s_Rand.GetF(Max); }
float	GlobalRand::GetF( float Min, float Max ){ return s_Rand.GetF(Min,Max); }

bool	GlobalRand::Check(int n){ return s_Rand.Check(n); }

void	GlobalRand::SetSeed( unt Seed ) { return s_Rand.SetSeed(Seed); }

}
