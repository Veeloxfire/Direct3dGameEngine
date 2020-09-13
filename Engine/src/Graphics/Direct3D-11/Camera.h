#pragma once

#include "VertexTypes.h"

class OrthographicCamera
{
	constexpr static float2 UnitSize = { 300, 300 };
	float2 WindowSize = { 1080, 720 };

	float4 m_Bounds = { 0.0f, 0.0f, 0.0f, 0.0f };//Bounds = {left, right, BOTTOM, top}
	float3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0f;

	bool hasMoved = false;
	bool hasResized = false;

	DirectX::XMMATRIX m_Projection = {};
	DirectX::XMMATRIX m_View = {};
	DirectX::XMMATRIX m_ViewProjection = {};

	void CalculateThenSetBounds()
	{
		const float HalfWidth = WindowSize.x / (UnitSize.x * 2.0f);
		const float HalfHeight = WindowSize.y / (UnitSize.y * 2.0f);
		SetBounds({ -HalfWidth, HalfWidth, -HalfHeight, HalfHeight });
	}

	void SetBounds(float4&& newBounds)
	{
		if (m_Bounds != newBounds)
		{
			m_Bounds = std::move(newBounds);
			hasResized = true;
		}
	}

	void SetProjection()
	{
		m_Projection = DirectX::XMMatrixOrthographicOffCenterLH(m_Bounds.x, m_Bounds.y, m_Bounds.z, m_Bounds.w, -1.0f, 1.0f);
	}

	void SetView()
	{
		//Constants
		static constexpr DirectX::XMFLOAT3 EyeDirectionFloat3 = { 0.0f, 0.0f, 1.0f };
		static constexpr DirectX::XMFLOAT3 UpDirectionFloat3 = { 0.0f, 1.0f, 0.0f };

		//Load Floats
		DirectX::XMVECTOR EyePosition = DirectX::XMLoadFloat3((const DirectX::XMFLOAT3*) & m_Position);
		DirectX::XMVECTOR EyeDirection = DirectX::XMLoadFloat3(&EyeDirectionFloat3);
		DirectX::XMVECTOR UpDirection = DirectX::XMLoadFloat3(&UpDirectionFloat3);

		//Calc view
		m_View = DirectX::XMMatrixLookToLH(EyePosition, EyeDirection, UpDirection);
	}

	void CalculateViewProjection()
	{
		m_ViewProjection = XMMatrixMultiply(m_View, m_Projection);
	}

	void Update()
	{
		if (hasResized) SetProjection();
		if (hasMoved) SetView();
		CalculateViewProjection();
		hasMoved = false;
		hasResized = false;
	}

public:
	OrthographicCamera()
	{
		//Setup For Default Values
		CalculateThenSetBounds();
		SetProjection();
		SetView();
		CalculateViewProjection();
	}

	bool NeedsUpdate() const { return hasMoved || hasResized; }

	void UpdateIfNeeded()
	{
		if (NeedsUpdate()) Update();
	}

	void SetWindowSize(const float width, const float height)
	{
		WindowSize.x = width;
		WindowSize.y = height;
		CalculateThenSetBounds();
	}

	void MoveCamra(float horizontal, float vertical, float rotation)
	{
		hasMoved = horizontal || vertical || rotation || hasMoved;

		m_Position.x += horizontal;
		m_Position.y += vertical;
		m_Rotation += rotation;
	}

	mat4 GetViewProjection() const
	{
		return m_ViewProjection;
	}

	mat4 WorldToScreen(const mat4& m4) const
	{
		return DirectX::XMMatrixMultiply(m4, m_ViewProjection);
	}

	float4 WorldToScreen(const float4& f4) const
	{
		return DirectX::XMVector4Transform(f4, m_ViewProjection);
	}
};