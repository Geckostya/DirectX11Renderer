struct ConstantData
{
    float4x4 ModelViewProj;
};

struct VS_IN
{
	float3 pos : POSITION0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
};

cbuffer ConstBuf : register(b0) {
	ConstantData ConstData;
}

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	float4x4 testMat = float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	output.pos = mul(float4(input.pos, 1), ConstData.ModelViewProj);
//	output.pos = float4(input.pos, 1);
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 col = float4(1, 1, 1, 1);
#ifdef LINES_COLOR
    col = LINES_COLOR;
#endif
	return col;
}
