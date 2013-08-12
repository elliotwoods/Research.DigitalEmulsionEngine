//@author: vux
//@help: standard constant shader
//@tags: color
//@credits: 

Texture2D<float2> texture2d <string uiname="Texture";>;

SamplerState g_samLinear : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

 
cbuffer cbPerDraw : register( b0 )
{
	float4x4 tVP : VIEWPROJECTION;
	float4x4 tColor <string uiname="Color Transform";>;
	float2 R:TARGETSIZE;
	
	int Index;
	float2 Resolution;
	int2 GridResolution;
};


cbuffer cbPerObj : register( b1 )
{
	float4x4 tW : WORLD;
};

struct VS_IN
{
	float4 PosO : POSITION;
	float4 TexCd : TEXCOORD0;

};

struct vs2ps
{
    float4 PosWVP: SV_POSITION;
    float4 TexCd: TEXCOORD0;
	uint Index : TEXCOORD1;
	uint2 Resolution : TEXCOORD2;
	uint2 GridResolution : TEXCOORD3;
};

vs2ps VS(VS_IN input)
{
    vs2ps Out = (vs2ps)0;
    Out.PosWVP  = mul(input.PosO,mul(tW,tVP));
    Out.TexCd = input.TexCd;
	
	Out.Index = Index;
	Out.Resolution = Resolution;
	Out.GridResolution = GridResolution;
	
    return Out;
}


float2 Lookup(vs2ps In, int Index)
{
	uint x = In.TexCd.x * Resolution.x;
	uint y = In.TexCd.y * Resolution.y;
	
	x += (Index % In.GridResolution.x) * In.Resolution.x;
	y += (Index / In.GridResolution.x) * In.Resolution.y;
    return texture2d[uint2(x, y)];
}

float2 PS(vs2ps In): SV_Target
{
	int index2 = (In.Index / 2) * 2;
	float2 lhs = Lookup(In, index2);
	float2 rhs = Lookup(In, index2 + 1);
	return max(lhs, rhs);
}


technique10 MinMaxGeneration
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
