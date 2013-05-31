
//This is the buffer from t he renderer
//Renderer automatically assigns BACKBUFFER semantic

//Texture we want to read from
Texture2D Tex <string uiname="Input";>;
Texture2D Median <string uiname="Median";>;

//Buffer containing uvs for sampling
StructuredBuffer<float3> uv <string uiname="Pipet coordinates";>;

RWStructuredBuffer<uint> output : BACKBUFFER;

SamplerState samp : IMMUTABLE
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp; 
};

float2 getTexCoord(int index)
{
	float2 texcd = uv[index].xy;
	texcd.x += 1.0f;
	texcd.y = 1.0f - texcd.y;
	texcd /= 2.0f;
	return texcd;
}

uint2 getPixelCoord(int value)
{
	uint width, height, levels;
	Tex.GetDimensions(0, width, height, levels);
	
	float2 texCoord = getTexCoord(value);
	return texCoord * float2(width, height);
}

bool isHigh(int index)
{
	float2 texcd = getTexCoord(index);
	
	float input = Tex.SampleLevel(samp, texcd, 0).r;
	float median = Median.SampleLevel(samp, texcd, 0).r;
	return input > median;
}

[numthreads(1, 1, 1)]
void CS( uint3 i : SV_DispatchThreadID)
{
	uint count, stride;
	uint value = 0;
	uv.GetDimensions(count, stride);
	
	for (uint i=0; i<count; i++) {
		value |= (uint) isHigh(i) << i;
	}
	output[0] = value;
	//output[0] = Tex.SampleLevel(samp, getTexCoord(2), 0).r * 255.0f;
	//output[0] = Tex[getPixelCoord(0)].r * 255;
	
	//float2 texcd = toTexCoord(uv[0].xy);
	//output[0] = texcd.x * 1000.0f;
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







