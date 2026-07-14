#include "Object3d.hlsli"


ConstantBuffer<Material> gMaterial : register(b0); // ← 独自のstruct Materialは削除し、hlsli側のを使う
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
    
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gMaterial.color;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

    output.color = gMaterial.color * textureColor;
    
    
    if (gMaterial.enableLighting != 0)
    {
        //float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));

        
        float NodotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NodotL * 0.5f + 0.5f, 2.0f);
        
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
        
        float4 transformedUV = mul(float32_t4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
        float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}
