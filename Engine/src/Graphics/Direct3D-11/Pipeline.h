#pragma once
#include "Device.h"
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexData.h"
#include "Texture2D.h"
#include "SamplerState.h"

class Pipeline
{
	Device m_Device;
	
	Texture2D m_TestTexture;
	SamplerState m_SamplerState;

	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

	OrthographicCamera m_Camera;

	VertexData m_VertexData;
	ID3D11InputLayout* m_Layout = nullptr;

	void AssignShaders();
	HRESULT CreateLayout();

	HRESULT Flush();

public:
	Pipeline(LPWSTR vsCSO, LPWSTR psCSO)
		:m_VertexShader(vsCSO), m_PixelShader(psCSO)
	{}
	~Pipeline()
	{
		if (m_Layout) m_Layout->Release();
	}

	void ClearBuffers() { m_Device.ClearBuffers(); }

	HRESULT CreateDevice(HWND hwnd) { return m_Device.Create(hwnd); }
	HRESULT CreatePipeline();

	void MoveCamera(float horizontal, float vertical, float rotation)
	{
		m_Camera.MoveCamra(horizontal, vertical, rotation);
	}

	void SetWindowSize(float width, float height)
	{
		m_Camera.SetWindowSize(width, height);
	}

	HRESULT Resize(int width, int height) 
	{
		SetWindowSize(static_cast<float>(width), static_cast<float>(height));
		return m_Device.ResizeBuffers(); 
	}
	void Begin();
	HRESULT DrawSquare();
	HRESULT End();
};