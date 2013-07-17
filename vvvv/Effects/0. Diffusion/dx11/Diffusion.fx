//@author: vux
//@help: standard constant shader
//@tags: color
//@credits: 

#define SearchStep 4
#define SearchIterationsXY 5

Texture2D TexInk <string uiname="Ink";>;
Texture2D TexXYZA <string uiname="World XYZA";>;

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
	float4x4 tTex <string uiname="Texture Transform"; bool uvspace=true; >;
	float4x4 tColor <string uiname="Color Transform";>;
	float DiffusionFactor = 0.01f;
	float MaxDistance = 0.01f;
	float dt;
	float maxInk = 10.0f;
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
    Out.TexCd = mul(input.TexCd, tTex);
    return Out;
}

float4 PS(vs2ps In): SV_Target
{
	uint2 R;
	uint levels;
	TexInk.GetDimensions(0, R.x, R.y, levels);
	
	uint2 PixelCoord = (float2) R * In.TexCd.xy;
	
	float4 value = TexInk[PixelCoord];
	
	float4 positionLookup = TexXYZA[PixelCoord];
    float3 position = positionLookup.xyz;
	bool validPosition = positionLookup.a > 0.5f;
	
	float4 PositionAtCoordLookup;
	
	for (int i=-SearchIterationsXY; i<=SearchIterationsXY; i++)
	{
		for (int j=-SearchIterationsXY; j<=SearchIterationsXY; j++)
		{
			if (i == j)
				continue;
			uint2 Coord = (uint2) ((int2) PixelCoord + int2(i * SearchStep, j * SearchStep));
			bool valid = !(Coord.x < 0 || Coord.y < 0 || Coord.x >= R.x || Coord.y >= R.y);
			
			float4 InkAtCoord = TexInk[Coord];
			PositionAtCoordLookup = TexXYZA[Coord];
			valid = valid & validPosition && (PositionAtCoordLookup.a > 0.5);
			//valid = valid & InkAtCoord.a > 0.5f;
			
			float3 PositionAtCoord = PositionAtCoordLookup.xyz;
			
			float distanceToPoint = length(PositionAtCoord - position);
			
			value.rgb += valid * dt * InkAtCoord.rgb * DiffusionFactor * MaxDistance / distanceToPoint;
			//value.rgb += InkAtCoord.rgb * DiffusionFactor * valid;
			value.rgb = clamp(value.rgb, 0, 1.0f);
		}
	}
	
    return value * Alpha;
}





technique10 DiffusionStep
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}