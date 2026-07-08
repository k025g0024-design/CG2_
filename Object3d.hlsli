struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0; // UV座標を追加
    float32_t normal : NORMAL0;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
    
    ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
    
};