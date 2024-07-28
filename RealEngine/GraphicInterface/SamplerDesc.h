#pragma once

#include "D3D12ThirdPart.h"

class RSamplerDesc : public D3D12_SAMPLER_DESC
{
public:
	RSamplerDesc()
	{
		// MIN 表示当一个屏幕屏幕像素覆盖多个纹理元素时，即纹理缩小
		// MAG 表示多个连续屏幕像素覆盖同一个纹理元素时，即纹理放大
		// MIP 表示mip之间的采样混合方式
		// POINT 表示采样纹理时直接取最近的像素
		// LINEAR 表示用线性插值，无论纹理放大还是纹理缩小，都是以采样点附近最近四点为操作目标。
		// 
		// D3D12_FILTER_MIN_MAG_MIP_LINEAR 三线性过滤
		// D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT 双线性过滤
		// 
		// COMPARISION 只有用这个，才可以在HLSL中用sahdowMapTexture.SampleCmp(sampler, uv, currentDepth)
		// MINIMUM，MAXIMUM 通常的线性插值中不是要按距离权重混合多个采样值嘛，这个版本直接取最大或最小值
		// 对于结合POINT版本，相当于只影响MIP间的取值方式，因为一个MIP中，采样点只有一个，不论MIN还是MAX结果都一样。
		// 通常用于ShadowMapping中取最小深度，AO，GI 的采样。
		// 
		// 各向异性过滤 https://blog.csdn.net/liran2019/article/details/108781848
		// 是为了解决当纹理所在表面倾斜于屏幕时，采样点附近横纵两个方向投影到一个屏幕像素内的纹素数量不同，
		// 此时如果两个方向还是个采一个进行线性插值，在斜的那个方向就会更模糊(细节)，所以，此时可以在这个方向上多插值几个像素
		// 因为两个方向上采样的纹素数量不同，所以称`各向异`
		Filter = D3D12_FILTER_ANISOTROPIC; 

		// 各个方向如何处理uv超过[0,1]的情况
		// WRAP 平铺重复展开
		// CLAMP uv被Clamp到[0,1]
		// BORDER 超出范围时使用边框色
		AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

		// 通常是自动选择，计算纹理空间位置(s,t)相对于屏幕空间位置(u,v)的变化率，偏导数。
		// 这个bias值会被加到这个计算出来的值，可正可负
		MipLODBias = 0.f;

		// 用各向异性采样时，在一个方向上最多的采样次数 通常1 ~ 16
		MaxAnisotropy = 16;

		// 通常，如果采样的纹理是ShadowMap，可以直接用
		// sahdowMapTexture.SampleCmp(sampler, uv, currentDepth)进行比较，而不用手动采样出深度值，手动比较
		// 如果这里是LESS_EQUAL, 当currentDepth <= 采样值时，返回1，否则返回0.
		// 要求过滤器必须使用COMPARISION开头的
		ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

		// 仅当Address为 BORDER时生效
		BorderColor[0] = 1.0f;
		BorderColor[1] = 1.0f;
		BorderColor[2] = 1.0f;
		BorderColor[3] = 1.0f;
		 
		// 允许的最大最小LOD等级
		MinLOD = 0.f;
		MaxLOD = D3D12_FLOAT32_MAX;
	}
};
