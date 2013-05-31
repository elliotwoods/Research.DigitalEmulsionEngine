
//This is the buffer from t he renderer
//Renderer automatically assigns BACKBUFFER semantic

//Texture we want to read from
Texture2D<float> Tex <string uiname="Input";>;
Texture2D<float> Median <string uiname="Median";>;

//Buffer containing uvs for sampling
StructuredBuffer<float3> uv <string uiname="Pipet coordinates";>;

RWStructuredBuffer<uint> output : BACKBUFFER;

SamplerState samp : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp; 
};

[numthreads(1, 1, 1)]
void CS( uint3 i : SV_DispatchThreadID)
{
	uint count, stride;
	uint value = 0;
	uv.GetDimensions(count, stride);
	for (uint i=0; i<count; i++) {
		float input = Tex.SampleLevel(samp, uv[i].xy, 0);
		float median = Median.SampleLevel(samp, uv[i].xy, 0);
		uint high = input > median;
		
		value |= high << i;
	}
	output[0] = value;
	//Read color and writed to buffer
	//rwbuffer[i.x] = tex.SampleLevel(mySampler,uv[i.x],0);
}

technique11 Process
{
	pass P0
	{
		SetComputeShader( CompileShader( cs_5_0, CS() ) );
	}
}







