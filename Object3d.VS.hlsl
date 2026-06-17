#include "Object3d.hlsli"

// 1. 行列を受け取る構造体を定義
struct TransformationMatrix
{
    float32_t4x4 WVP;
};

// 2. 定数バッファを b0 レジスタにバインド (C++側の rootParameters[1] に対応)
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // 3. mul関数を使って座標変換を行う
    output.position = mul(input.position, gTransformationMatrix.WVP);
    
    output.texcoord = input.texcoord;
    return output;
}

