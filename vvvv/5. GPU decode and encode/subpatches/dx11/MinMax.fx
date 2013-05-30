//@author: vux
//@help: standard constant shader
//@tags: color
//@credits: 

Texture2D<float> texture2d <string uiname="Texture";>;

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




float2 PS(vs2ps In): SV_Target
{
	int width, height;
	int levels;
	texture2d.GetDimensions(0, width, height, levels);
	
    float value = texture2d[float2(width, height) * In.TexCd.xy];
	return float2(1.0f - value, value);
}


technique10 MinMax
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
