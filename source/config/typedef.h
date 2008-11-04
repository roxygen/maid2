#ifndef maid2_config_typedef_h
#define maid2_config_typedef_h


// ハードウェアを操作するときなど、ビット単位の操作が必要なときに使われる typedef 
// 普段は int とか float を使ってください。

typedef unsigned int unt;  // int と揃えたかったんで unt

typedef unsigned char  unt08;
typedef unsigned short unt16;
typedef unsigned int   unt32;

typedef char  int08;
typedef short int16;
typedef int   int32;

typedef float float32;

#endif