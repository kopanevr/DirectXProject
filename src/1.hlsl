
struct VERTEX_SHADER_INPUT
{
    float4  Pos  :   POSITION;
    float2  Tex  :   TEXCOORD;
};

struct PIXEL_SHADER_INPUT
{
    float4  Pos  :   SV_POSITION;
    float2  Tex  :   TEXCOORD;
};

PIXEL_SHADER_INPUT VertexMain(VERTEX_SHADER_INPUT input)
{
    PIXEL_SHADER_INPUT output;

    output.Pos  = input.Pos;
    output.Tex  = input.Tex;

    return  output;
}

Texture2D       Tex     :   register(t0);               // Текстура.
SamplerState    Sam     :   register(s0);

float4 PixelMain(PIXEL_SHADER_INPUT input)  :   SV_Target0 
{
    return Tex.Sample(Sam, input.Tex);
}
