#pragma once
#include "Util/TypeSwitch.h"
#include "Direct3dHeaders.h"
#include <array>

struct float2
{
	float x = 0, y = 0;

	constexpr float2() = default;
	constexpr float2(float x_in, float y_in)
		: x(x_in), y(y_in)
	{}

	constexpr float2(const float2 & f2) = default;
	constexpr float2(float2&& f2) = default;

	constexpr float2& operator=(const float2& f2) = default;
	constexpr float2& operator=(float2 && f2) = default;
};

struct float3
{
	float x = 0, y = 0, z = 0;

	constexpr float3() = default;
	constexpr float3(float x_in, float y_in, float z_in)
		: x(x_in), y(y_in), z(z_in)
	{}

	constexpr float3(const float3 & f3) = default;
	constexpr float3(float3&& f3) = default;

	constexpr float3& operator=(const float3& f3) = default;
	constexpr float3& operator=(float3 && f3) = default;

	float3(const DirectX::XMVECTOR& vec)
	{
		DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, vec);
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat3((const DirectX::XMFLOAT3*)this);
	}
};

struct float4
{
	float x = 0, y = 0, z = 0, w = 0;

	constexpr float4() = default;
	constexpr float4(float x_in, float y_in, float z_in, float w_in)
		: x(x_in), y(y_in), z(z_in), w(w_in)
	{}

	constexpr float4(const float4& f4) = default;
	constexpr float4(float4&& f4) = default;

	constexpr float4& operator=(const float4& f4) = default;
	constexpr float4& operator=(float4&& f4) = default;

	bool operator==(const float4& f4)
	{
		return x == f4.x && y == f4.y && z == f4.z && w == f4.w;
	}

	bool operator!=(const float4& f4)
	{
		return x != f4.x || y != f4.y || z != f4.z || w != f4.w;
	}

	float4(const DirectX::XMVECTOR& vec)
	{
		DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, vec);
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4((const DirectX::XMFLOAT4*)this);
	}
};

struct mat4
{
	float4 x = {}, y = {}, z = {}, w = {};

	constexpr mat4() = default;
	constexpr mat4(float4 x_in, float4 y_in, float4 z_in, float4 w_in)
		: x(x_in), y(y_in), z(z_in), w(w_in)
	{}
	constexpr mat4(float4&& x_in, float4&& y_in, float4&& z_in, float4&& w_in)
		: x(std::forward<float4>(x_in)), y(std::forward<float4>(y_in)), z(std::forward<float4>(z_in)), w(std::forward<float4>(w_in))
	{}

	constexpr mat4(const mat4& m4) = default;
	constexpr mat4(mat4&& m4) = default;

	constexpr mat4& operator=(const mat4& m4) = default;
	constexpr mat4& operator=(mat4 && m4) = default;

	mat4(const DirectX::XMMATRIX& mat)
	{
		DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)this, mat);
	}

	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4((const DirectX::XMFLOAT4X4*)this);
	}
};

using Texcoord = float2;
using Position = float4;
using Colour = float4;

using TypeSwitch = All<unsigned int, float, float2, float3, float4, mat4>;

enum class Types
{
	Unsigned_Int = ElementIndex<unsigned int, TypeSwitch>::getIndex(),

	Float  = ElementIndex<float , TypeSwitch>::getIndex(),
	Float2 = ElementIndex<float2, TypeSwitch>::getIndex(),
	Float3 = ElementIndex<float3, TypeSwitch>::getIndex(),
	Float4 = ElementIndex<float4, TypeSwitch>::getIndex(),
	Mat4   = ElementIndex<mat4  , TypeSwitch>::getIndex(),

	Position = Float4,
	Colour   = Float4,
	Texcoord = Float2,
	TexIndex = Unsigned_Int,
};

constexpr static const char PositionName[] = "POSITION";
constexpr static const char ColourName[] = "COLOUR";
constexpr static const char TexIndexName[] = "TEXINDEX";
constexpr static const char TexcoordName[] = "TEXCOORD";

template<Types t>
using TypeFromEnum = typename ElementType<static_cast<unsigned int>(t), TypeSwitch>::type;

template<const char* name, Types type>
struct Joint;

struct JointStruct
{
	const char* const name;
	const Types type;
};

template<typename ... a>
class VertexBase;

template <const char* ... c, Types ... t>
class VertexBase<Joint<c, t>...> : public All<TypeFromEnum<t> ...>
{
public:
	constexpr VertexBase() : All()
	{}
	constexpr VertexBase(TypeFromEnum<t>&& ... in) : All(std::forward<TypeFromEnum<t>>(in)...)
	{}
	constexpr VertexBase(const TypeFromEnum<t>& ... in) : All(in...)
	{}
};

template<typename Vertex, typename Instance>
class LayoutBase;

template <const char* fC1, Types fT1, const char* ... c1, Types ... t1, const char* fC2, Types fT2, const char* ... c2, Types ... t2>
class LayoutBase<VertexBase<Joint<fC1, fT1>, Joint<c1, t1>...>, VertexBase<Joint<fC2, fT2>, Joint<c2, t2>...>>
{
private:
	// + 1 is to include fC1 and fC2
	constexpr static unsigned int VertexNum = sizeof...(c1) + 1;
	constexpr static unsigned int InstanceNum = sizeof...(c2) + 1;

	constexpr static DXGI_FORMAT format(const Types type)
	{
		switch (type)
		{
		case Types::Unsigned_Int: return DXGI_FORMAT_R32_UINT;
		case Types::Float: return DXGI_FORMAT_R32_FLOAT;
		case Types::Float2: return DXGI_FORMAT_R32G32_FLOAT;
		case Types::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case Types::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

public:
	constexpr static unsigned int NumElements = VertexNum + InstanceNum;

	constexpr static D3D11_INPUT_ELEMENT_DESC Layout[NumElements] =
	{
			D3D11_INPUT_ELEMENT_DESC{fC1, 0, format(fT1), 0, 0                           , D3D11_INPUT_PER_VERTEX_DATA, 0},
			D3D11_INPUT_ELEMENT_DESC{ c1, 0, format( t1), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}...,

			D3D11_INPUT_ELEMENT_DESC{fC2, 0, format(fT2), 1, 0                           , D3D11_INPUT_PER_INSTANCE_DATA, 1},
			D3D11_INPUT_ELEMENT_DESC{ c2, 0, format( t2), 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}...,
	};
};