//@author: vux
//@help: standard constant shader
//@tags: color
//@credits: 

Texture2D<float> texture2d <string uiname="Texture";>;

SamplerState g_samNearest : IMMUTABLE
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

 
cbuffer cbPerDraw : register( b0 )
{
	float4x4 tVP : VIEWPROJECTION;
};


cbuffer cbPerObj : register( b1 )
{
	float4x4 tW : WORLD;
	float Brightness = 1.0f;

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




float4 PS(vs2ps In): SV_Target
{
    float value = texture2d.Sample(g_samNearest,In.TexCd.xy);
	float4 output;
	output.rgb = value * Brightness;
	output.a = 1.0f;
	return output;
}





technique10 BinaryPad
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}




