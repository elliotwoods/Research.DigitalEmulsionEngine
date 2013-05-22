//@author: vvvv group
//@help: draws a mesh with a constant color
//@tags: template, basic
//@credits:

// --------------------------------------------------------------------------------------------------
// PARAMETERS:
// --------------------------------------------------------------------------------------------------

//transforms
float4x4 tW: WORLD;        //the models world matrix
float4x4 tV: VIEW;         //view matrix as set via Renderer (EX9)
float4x4 tP: PROJECTION;   //projection matrix as set via Renderer (EX9)
float4x4 tWVP: WORLDVIEWPROJECTION;

//material properties
float2 Poly <string uiname="Balance Coefficients";> = float2(0.6519, -.1097);

//texture
texture Tex <string uiname="Texture";>;
sampler Samp = sampler_state    //sampler for doing the texture-lookup
{
    Texture   = (Tex);          //apply a texture to the sampler
    MipFilter = LINEAR;         //sampler states
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4x4 tTex: TEXTUREMATRIX <string uiname="Texture Transform";>;
float Alpha = 1.0f;

//the data structure: vertexshader to pixelshader
//used as output data with the VS function
//and as input data with the PS function
struct vs2ps
{
    float4 Pos : POSITION;
    float4 TexCd : TEXCOORD0;
};

// --------------------------------------------------------------------------------------------------
// VERTEXSHADERS
// --------------------------------------------------------------------------------------------------

vs2ps VS(
    float4 Pos : POSITION,
    float4 TexCd : TEXCOORD0)
{
    //inititalize all fields of output struct with 0
    vs2ps Out = (vs2ps)0;

    //transform position
    Out.Pos = mul(Pos, tWVP);

    //transform texturecoordinates
    Out.TexCd = mul(TexCd, tTex);

    return Out;
}

// --------------------------------------------------------------------------------------------------
// PIXELSHADERS:
// --------------------------------------------------------------------------------------------------

float4 PS(vs2ps In): COLOR
{
    //In.TexCd = In.TexCd / In.TexCd.w; // for perpective texture projections (e.g. shadow maps) ps_2_0

    float A = tex2D(Samp, In.TexCd).b;
	float B = (1.0f - A) * Poly.x + (Poly.y / 255.0f);
	
	float4 col = float4(B, B, A, 1.0f);
    col.a *= Alpha;
    return col;
}

// --------------------------------------------------------------------------------------------------
// TECHNIQUES:
// --------------------------------------------------------------------------------------------------

technique TConstant
{
    pass P0
    {
        //Wrap0 = U;  // useful when mesh is round like a sphere
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }
}