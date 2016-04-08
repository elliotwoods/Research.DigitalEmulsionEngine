Texture2D<float> Input;
Texture2D<float2> MedianAndDistance;
Texture2D<uint2> Previous <string uiname="Previous";>;

SamplerState g_samLinear : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

 
cbuffer cbPerDraw : register( b0 )
{
	float4x4 tVP : VIEWPROJECTION;
	float2 R:TARGETSIZE;
	
	int FrameIndex <int uimin=0; int uistep=1;> = 0;
	int FrameCountX <int uimin=0; int uistep=1;> = 10;
	bool Clear <bool bang=true;> = false;
	float DistanceThreshold <float uimin=0.0;float uimax=1.0;> = 10.0f / 255.0f;
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
};

vs2ps VS(VS_IN input)
{
    vs2ps Out = (vs2ps)0;
    Out.PosWVP  = mul(input.PosO,mul(tW,tVP));
    Out.TexCd = input.TexCd;
    return Out;
}

uint2 PS(vs2ps In): SV_Target
{
	float2 uv = R * In.TexCd.xy;
    
	float input = Input[uv].r;
	float2 medianAndDistance = MedianAndDistance[uv];
	uint2 value = Previous[uv];
	uint maskAll = (1 << 30) - 1;
	
	if (Clear) {
		value = (uint2) 0;
	}
	
	bool high = input > medianAndDistance.r & medianAndDistance.g > DistanceThreshold;
	
	if (FrameIndex < FrameCountX) {
		int mask = 1 << FrameIndex;
		if (high) {
			value.x = value.x | mask;
		} else {
			value.x = value.x & (maskAll - mask);
		}
	} else {
		int mask = 1 << (FrameIndex - FrameCountX);
		if (high) {
			value.y = value.y | mask;
		} else {
			value.y = value.y & (maskAll - mask);
		}
	}
	
    return value;
}





technique10 Constant
{
	pass P0 <string format="R16G16_UInt"; >
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}




