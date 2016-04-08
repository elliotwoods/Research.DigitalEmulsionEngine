
//This is the buffer from t he renderer
//Renderer automatically assigns BACKBUFFER semantic
RWStructuredBuffer<uint2> rwbuffer : BACKBUFFER;

//Texture we want to read from
Texture2D<uint2> tex <string uiname="Texture";>;

//Buffer containing uvs for sampling
StructuredBuffer<float2> uv <string uiname="UV Buffer";>;

SamplerState mySampler : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp; 
};

[numthreads(1, 1, 1)]
void CS( uint3 i : SV_DispatchThreadID)
{ 
	//Read color and writed to buffer
	//rwbuffer[i.x] = tex.SampleLevel(mySampler,uv[i.x],0);
	uint width;
	uint height;
	uint levelCount;
	tex.GetDimensions(0, width, height, levelCount);
	
	int2 pixelCoords = uv[i.x] * float2(width, height);
	
	rwbuffer[i.x] = tex[pixelCoords];
}

technique11 Process
{
	pass P0
	{
		SetComputeShader( CompileShader( cs_5_0, CS() ) );
	}
}