﻿#include"debug.h"

#include"../../../../auxiliary/macro.h"

static std::string _sprintf( const char* format, ...)
{
  va_list args;
  char buf[1024];

  va_start(args,format);
  vsprintf_s(buf, format, args);

  return std::string(buf);
}

std::string DebugStringD3D09( D3DFORMAT fmt )
{
  std::string ret;

  switch( fmt )
  {
  case D3DFMT_UNKNOWN:  { ret="_UNKNOWN"; }break;
  case D3DFMT_R8G8B8:   { ret="_R8G8B8"; }break;
  case D3DFMT_A8R8G8B8: { ret="_A8R8G8B8"; }break;
  case D3DFMT_X8R8G8B8: { ret="_X8R8G8B8"; }break;
  case D3DFMT_R5G6B5:   { ret="_R5G6B5"; }break;
  case D3DFMT_X1R5G5B5: { ret="_X1R5G5B5"; }break;
  case D3DFMT_A1R5G5B5: { ret="_A1R5G5B5"; }break;
  case D3DFMT_A4R4G4B4: { ret="_A4R4G4B4"; }break;
  case D3DFMT_R3G3B2:   { ret="_R3G3B2"; }break;
  case D3DFMT_A8:       { ret="_A8"; }break;
  case D3DFMT_A8R3G3B2: { ret="_A8R3G3B2"; }break;
  case D3DFMT_X4R4G4B4: { ret="_X4R4G4B4"; }break;
  case D3DFMT_A2B10G10R10: { ret="_A2B10G10R10"; }break;
  case D3DFMT_A8B8G8R8: { ret="_A8B8G8R8"; }break;
  case D3DFMT_X8B8G8R8: { ret="_X8B8G8R8"; }break;
  case D3DFMT_G16R16:   { ret="_G16R16"; }break;
  case D3DFMT_A2R10G10B10: { ret="_A2R10G10B10"; }break;
  case D3DFMT_A16B16G16R16: { ret="_A16B16G16R16"; }break;
  case D3DFMT_A8P8:     { ret="_A8P8"; }break;
  case D3DFMT_P8:       { ret="_P8"; }break;
  case D3DFMT_L8:       { ret="_L8"; }break;
  case D3DFMT_A8L8:     { ret="_A8L8"; }break;
  case D3DFMT_A4L4:     { ret="_A4L4"; }break;
  case D3DFMT_V8U8:     { ret="_V8U8"; }break;
  case D3DFMT_L6V5U5:   { ret="_L6V5U5"; }break;
  case D3DFMT_X8L8V8U8: { ret="_X8L8V8U8"; }break;
  case D3DFMT_Q8W8V8U8: { ret="_Q8W8V8U8"; }break;
  case D3DFMT_V16U16:   { ret="_V16U16"; }break;
  case D3DFMT_A2W10V10U10: { ret="_A2W10V10U10"; }break;
  case D3DFMT_UYVY:     { ret="_UYVY"; }break;
  case D3DFMT_R8G8_B8G8:{ ret="_R8G8_B8G8"; }break;
  case D3DFMT_YUY2:     { ret="_YUY2"; }break;
  case D3DFMT_G8R8_G8B8:{ ret="_G8R8_G8B8"; }break;
  case D3DFMT_DXT1:     { ret="_DXT1"; }break;
  case D3DFMT_DXT2:     { ret="_DXT2"; }break;
  case D3DFMT_DXT3:     { ret="_DXT3"; }break;
  case D3DFMT_DXT4:     { ret="_DXT4"; }break;
  case D3DFMT_DXT5:     { ret="_DXT5"; }break;
  case D3DFMT_D16_LOCKABLE: { ret="_D16_LOCKABLE"; }break;
  case D3DFMT_D32:      { ret="_D32"; }break;
  case D3DFMT_D15S1:    { ret="_D15S1"; }break;
  case D3DFMT_D24S8:    { ret="_D24S8"; }break;
  case D3DFMT_D24X8:    { ret="_D24X8"; }break;
  case D3DFMT_D24X4S4:  { ret="_D24X4S4"; }break;
  case D3DFMT_D16:      { ret="_D16"; }break;
  case D3DFMT_D32F_LOCKABLE: { ret="_D32F_LOCKABLE"; }break;
  case D3DFMT_D24FS8:   { ret="_D24FS8"; }break;
  case D3DFMT_L16:      { ret="_L16"; }break;
  case D3DFMT_VERTEXDATA: { ret="_VERTEXDATA"; }break;
  case D3DFMT_INDEX16:  { ret="_INDEX16"; }break;
  case D3DFMT_INDEX32:  { ret="_INDEX32"; }break;
  case D3DFMT_Q16W16V16U16: { ret="_Q16W16V16U16"; }break;
  case D3DFMT_MULTI2_ARGB8: { ret="_MULTI2_ARGB8"; }break;
  case D3DFMT_R16F:     { ret="_R16F"; }break;
  case D3DFMT_G16R16F:  { ret="_G16R16F"; }break;
  case D3DFMT_A16B16G16R16F: { ret="_A16B16G16R16F"; }break;
  case D3DFMT_R32F:     { ret="_R32F"; }break;
  case D3DFMT_G32R32F:  { ret="_G32R32F"; }break;
  case D3DFMT_A32B32G32R32F: { ret="_A32B32G32R32F"; }break;
  case D3DFMT_CxV8U8:   { ret="_CxV8U8"; }break;
  }

  ret += _sprintf( "(%0d)", fmt );
  return ret;
}

std::string DebugStringD3D09( BOOL b )
{
  std::string ret;

  if( b==TRUE ) { ret = "TRUE"; }
  else          { ret = "FALSE"; }

  ret += _sprintf( "(%0x)", b );
  return ret;
}


std::string DebugStringD3D09( D3DMULTISAMPLE_TYPE type )
{
  std::string ret;

  switch( type )
  {
  case D3DMULTISAMPLE_NONE:       { ret="NONE"; }break;
  case D3DMULTISAMPLE_NONMASKABLE:{ ret="NONMASKABLE"; }break;
  case D3DMULTISAMPLE_2_SAMPLES:  { ret="2_SAMPLES"; }break;
  case D3DMULTISAMPLE_3_SAMPLES:  { ret="3_SAMPLES"; }break;
  case D3DMULTISAMPLE_4_SAMPLES:  { ret="4_SAMPLES"; }break;
  case D3DMULTISAMPLE_5_SAMPLES:  { ret="5_SAMPLES"; }break;
  case D3DMULTISAMPLE_6_SAMPLES:  { ret="6_SAMPLES"; }break;
  case D3DMULTISAMPLE_7_SAMPLES:  { ret="7_SAMPLES"; }break;
  case D3DMULTISAMPLE_8_SAMPLES:  { ret="8_SAMPLES"; }break;
  case D3DMULTISAMPLE_9_SAMPLES:  { ret="9_SAMPLES"; }break;
  case D3DMULTISAMPLE_10_SAMPLES: { ret="10_SAMPLES"; }break;
  case D3DMULTISAMPLE_11_SAMPLES: { ret="11_SAMPLES"; }break;
  case D3DMULTISAMPLE_12_SAMPLES: { ret="12_SAMPLES"; }break;
  case D3DMULTISAMPLE_13_SAMPLES: { ret="13_SAMPLES"; }break;
  case D3DMULTISAMPLE_14_SAMPLES: { ret="14_SAMPLES"; }break;
  case D3DMULTISAMPLE_15_SAMPLES: { ret="15_SAMPLES"; }break;
  case D3DMULTISAMPLE_16_SAMPLES: { ret="16_SAMPLES"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}

std::string DebugStringD3D09( D3DSWAPEFFECT type )
{
  std::string ret;

  switch( type )
  {
  case D3DSWAPEFFECT_DISCARD: { ret="DISCARD"; }break;
  case D3DSWAPEFFECT_FLIP:    { ret="FLIP"; }break;
  case D3DSWAPEFFECT_COPY:    { ret="COPY"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}

std::string DebugStringD3D09PRESENTFLAG( UINT flag )
{
  std::string ret;

  if( IsFlag(flag,D3DPRESENTFLAG_LOCKABLE_BACKBUFFER)  ) { ret+="+LOCKABLE_BACKBUFFER";}
  if( IsFlag(flag,D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL) ) { ret+="+DISCARD_DEPTHSTENCIL";}
  if( IsFlag(flag,D3DPRESENTFLAG_DEVICECLIP) )  { ret+="+DEVICECLIP";}
  if( IsFlag(flag,D3DPRESENTFLAG_VIDEO) )       { ret+="+VIDEO";}

  ret += _sprintf( "(%0x)", flag );
  return ret;
}

std::string DebugStringD3D09PRESENT( UINT flag )
{
  std::string ret;

  switch( flag )
  {
  case D3DPRESENT_INTERVAL_DEFAULT:   { ret="DEFAULT"; }break;
  case D3DPRESENT_INTERVAL_ONE:       { ret="ONE"; }break;
  case D3DPRESENT_INTERVAL_TWO:       { ret="TWO"; }break;
  case D3DPRESENT_INTERVAL_THREE:     { ret="THREE"; }break;
  case D3DPRESENT_INTERVAL_FOUR:      { ret="FOUR"; }break;
  case D3DPRESENT_INTERVAL_IMMEDIATE: { ret="IMMEDIATE"; }break;
  }

  ret += _sprintf( "(%0x)", flag );
  return ret;
}



std::string DebugStringD3D09( const D3DPRESENT_PARAMETERS& desc )
{
  std::string ret;

  ret += _sprintf( " BackBufferWidth:%0d", desc.BackBufferWidth );
  ret += _sprintf( " BackBufferHeight:%0d", desc.BackBufferHeight );
  ret += _sprintf( " BackBufferFormat:%s", DebugStringD3D09(desc.BackBufferFormat).c_str() );
  ret += _sprintf( " BackBufferCount:%0d", desc.BackBufferCount );

  ret += _sprintf( " MultiSampleType:%s", DebugStringD3D09(desc.MultiSampleType).c_str() );
  ret += _sprintf( " MultiSampleQuality:%0d", desc.MultiSampleQuality );

  ret += _sprintf( " SwapEffect:%s", DebugStringD3D09(desc.SwapEffect).c_str() );

  ret += _sprintf( " hDeviceWindow:%0d", desc.hDeviceWindow );
  ret += _sprintf( " Windowed:%s", DebugStringD3D09(desc.Windowed).c_str() );
  ret += _sprintf( " EnableAutoDepthStencil:%s", DebugStringD3D09(desc.EnableAutoDepthStencil).c_str() );

  ret += _sprintf( " AutoDepthStencilFormat:%s", DebugStringD3D09(desc.AutoDepthStencilFormat).c_str() );

  ret += _sprintf( " Flags:%s", DebugStringD3D09PRESENTFLAG(desc.Flags).c_str() );

  ret += _sprintf( " FullScreen_RefreshRateInHz:%0d", desc.FullScreen_RefreshRateInHz );

  ret += _sprintf( " PresentationInterval:%s", DebugStringD3D09PRESENT(desc.PresentationInterval).c_str() );

  return ret;
}
