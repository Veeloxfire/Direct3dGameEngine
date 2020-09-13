#pragma once
#include "Device.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexTypes.h"

using Vertex = VertexBase<Joint<TexcoordName, Types::Texcoord>, Joint<PositionName, Types::Position>>;
using Instance = VertexBase<Joint<TexIndexName, Types::TexIndex>, Joint<ColourName, Types::Colour>>;
using Constant = mat4;

using Layout = LayoutBase<Vertex, Instance>;

class VertexData
{
public:
	constexpr static unsigned int NumVertices = 4;
	constexpr static unsigned int NumIndices = 6;
	constexpr static unsigned int MaxSquares = 100;

	constexpr static Position SquarePositions[NumVertices] =
	{
		Position{ 0.5f,  0.5f, 0.0f, 1.0f},
		Position{ 0.5f, -0.5f, 0.0f, 1.0f},
		Position{-0.5f, -0.5f, 0.0f, 1.0f},
		Position{-0.5f,  0.5f, 0.0f, 1.0f},
	};

	constexpr static Texcoord SquareTexturePositions[NumVertices] =
	{
		Texcoord{1.0f, 0.0f},
		Texcoord{1.0f, 1.0f},
		Texcoord{0.0f, 1.0f},
		Texcoord{0.0f, 0.0f},
	};

	constexpr static Vertex SquareVertices[NumVertices] =
	{
		{SquareTexturePositions[0], SquarePositions[0]},
		{SquareTexturePositions[1], SquarePositions[1]},
		{SquareTexturePositions[2], SquarePositions[2]},
		{SquareTexturePositions[3], SquarePositions[3]},
	};

	constexpr static unsigned int SquareIndices[NumIndices] =
	{
		0, 1, 2,
		3, 0, 2,
	};

private:
	InstanceBuffer m_InstanceBuffer;

	Instance m_Instances[MaxSquares] = {};
	Instance* m_InstanceTop = m_Instances;

	ConstantBuffer m_ConstantBuffer;
	Constant m_ConstantData = {};

	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;



	template <typename DataType>
	HRESULT UpdateMappedObject(const Device& d, const DynamicBuffer& buffer, const DataType* const DataArr, unsigned int NumDataElements)
	{
		static constexpr unsigned int index = 0;
		D3D11_MAPPED_SUBRESOURCE resource = {0, 0, 0};

		HRESULT hr = d.context()->Map(buffer.get(), index, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		memcpy(resource.pData, DataArr, NumDataElements * sizeof(DataType));
		d.context()->Unmap(buffer.get(), index);
		return hr;
	}


	template <typename DataType>
	HRESULT UpdateMappedObject(const Device& d, const DynamicBuffer& buffer, const DataType& DataObj)
	{
		return UpdateMappedObject<DataType>(d, buffer, &DataObj, 1);
	}

public:
	HRESULT CreateBuffers(const Device& d, const OrthographicCamera& c)
	{
		m_ConstantData = c.GetViewProjection();

		//Vertex Buffer
		HRESULT hr = m_VertexBuffer.Create<Vertex, NumVertices>(d, SquareVertices);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}

		//Constant Buffer
		hr = m_ConstantBuffer.Create<Constant, 1>(d, &m_ConstantData);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}

		//Index Buffer
		hr = m_IndexBuffer.Create<unsigned int, NumIndices>(d, SquareIndices);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}

		//Instance Buffer
		hr = m_InstanceBuffer.Create<Instance, MaxSquares>(d, m_Instances);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		return hr;
	}

	void SetBuffers(const Device& d)
	{
		//Vertex Buffers
		constexpr unsigned int numVBuffers = 2;
		ID3D11Buffer* const vertexBuffers[numVBuffers] = { m_VertexBuffer.get(), m_InstanceBuffer.get() };
		const UINT strides[numVBuffers] = { sizeof(Vertex), sizeof(Instance) };
		const UINT offsets[numVBuffers] = { 0, 0 };

		d.context()->IASetVertexBuffers(0, numVBuffers, vertexBuffers, strides, offsets);

		//Constant Buffers
		constexpr unsigned int numCBuffers = 1;
		ID3D11Buffer* const constantBuffers[numCBuffers] = { m_ConstantBuffer.get() };

		d.context()->VSSetConstantBuffers(0, 1, constantBuffers);

		//Index Buffer
		d.context()->IASetIndexBuffer(m_IndexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

		//Topology
		d.context()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	unsigned int NumInstances() const
	{
		return m_InstanceTop - m_Instances;
	}

	bool ReachedEnd() const
	{
		return NumInstances() == MaxSquares;
	}

	HRESULT UpdateInstances(const Device& d)
	{
		return UpdateMappedObject(d, m_InstanceBuffer, m_Instances, NumInstances());
	}

	HRESULT UpdateConstant(const Device& d, const OrthographicCamera& c)
	{
		m_ConstantData = c.GetViewProjection();
		return UpdateMappedObject(d, m_ConstantBuffer, m_ConstantData);
	}

	void Reset()
	{
		ZeroMemory(m_Instances, MaxSquares);
		m_InstanceTop = m_Instances;
	}

	void AddSquare(Instance&& v)
	{
		*m_InstanceTop = std::forward<Instance>(v);
		m_InstanceTop++;
	}

	const Instance* data() const
	{
		return m_Instances;
	}
};