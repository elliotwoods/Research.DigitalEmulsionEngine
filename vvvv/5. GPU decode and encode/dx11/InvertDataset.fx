cbuffer cColor : register(b0)
{
	float2 size : TARGETSIZE;
}

Texture2D<uint2> Input;

RWTexture2D<uint2> Output : BACKBUFFER;

[numthreads(4, 4, 1)]
void main( uint3 threadID : SV_DispatchThreadID )
{	
	uint2 cameraPixelCoord = uint2(threadID.x, threadID.y);
	uint2 projectorPixelCoord = Input[cameraPixelCoord];
	Output[projectorPixelCoord] = cameraPixelCoord;
}
 
technique11 Render
{
	pass P0
	{
		SetComputeShader( CompileShader( cs_5_0, main() ) );
	}
}

