//@author: vux
//@help: standard constant shader
//@tags: color
//@credits: 

Texture2D<float3> WorldXYZ <string uiname="World";>;
Texture2D Color <string uiname="Texture";>;

SamplerState g_samLinear : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

 
cbuffer cbPerDraw : register( b0 )
{
	float4x4 tVP : VIEWPROJECTION;
	float Alpha <float uimin=0.0; float uimax=1.0;> = 1; 
	float4 cAmb <bool color=true;String uiname="Color";> = { 1.0f,1.0f,1.0f,1.0f };
	float4x4 tTex <string uiname="Texture Transform"; bool uvspace=true; >;
	float4x4 tColor <string uiname="Color Transform";>;
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
	uint width, height, levels;
	WorldXYZ.GetDimensions(0, width, height, levels);
	
	float4 position;
	position.xyz = WorldXYZ[float2(width, height) * input.TexCd.xy];
	position.w = 1.0f;
	
    vs2ps Out = (vs2ps)0;
    Out.PosWVP  = mul(position,mul(tW,tVP));
    Out.TexCd = mul(input.TexCd, tTex);
    return Out;
}




float4 PS(vs2ps In): SV_Target
{
	return Color.Sample(g_samLinear, In.TexCd.xy);
}





technique10 Constant
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}




