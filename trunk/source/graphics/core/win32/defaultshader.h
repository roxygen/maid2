#ifndef graphic_core_win32_defaultshader_h
#define graphic_core_win32_defaultshader_h


 //  頂点と色を流し込むシェーダー
static const char* s_SHADERCODE0100 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;


 //  頂点と色とUVを流し込むシェーダー
static const char* s_SHADERCODE0101 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n   Out.TexCoords = In.TexCoords;"
"\n"
"\n   return Out;"
"\n }"
;













static const char* s_SHADERCODE0200 = 
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float4 Color    : COLOR0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     return input.Color;"
"\n }"
;

/*
static const char* s_SHADERCODE0210 = 
"\n texture texture_slot0;"
"\n sampler Sampler = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n     float2 TexUV : TEXCOORD0;"
"\n     float4 Color : COLOR0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : COLOR"
"\n {"
"\n     return texture_slot0.Sample(Sampler,input.TexUV) * input.Color;"
"\n }"
;
*/
static const char* s_SHADERCODE0210 = 
"\n Texture2D<float4> texture_slot0;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float2 TexUV : TEXCOORD0;"
"\n     float4 Color : COLOR0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     return texture_slot0.Sample(sampler_slot0,input.TexUV) * input.Color;"
"\n }"
;

static const char* s_SHADERCODE0211 = 
"\n Texture2D<float4> texture_slot0;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float2 TexUV   : TEXCOORD0;"
"\n     float4 Diffuse : COLOR0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 base = texture_slot0.Sample(sampler_slot0,input.TexUV) * input.Diffuse;"
"\n     float alpha = base.w;"
"\n     base.xyz *= alpha;"
"\n     return base;"
"\n }"
;




#endif