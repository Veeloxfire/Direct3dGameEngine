#include "Shader.h"
#include "Log/Log.h"
#include <fstream>

ShaderData ShaderData::LoadFromFile(LPCWSTR _In_ FileName)
{
	ShaderData data;
	std::ifstream buildFile(FileName, std::ios::in | std::ios::binary | std::ios::ate);

	if (buildFile)
	{
		data.length = (unsigned int)buildFile.tellg();
		data.buffer = new char[data.length];
		buildFile.seekg(0, std::ios::beg);
		buildFile.read(data.buffer, data.length);
		buildFile.close();

		return data;
	}

	Log::cout << "File was not opened: " << FileName << '\n';
	return data;
}

//#include <d3dcompiler.h>
//#pragma comment(lib,"d3dcompiler.lib")

//HRESULT ShaderBlob::CompileFromFile(LPCWSTR _In_ FileName)
//{
//	//Dealloc if already in use
//	if (m_ShaderBlob) m_ShaderBlob->Release();
//	if (m_ErrorBlob) m_ErrorBlob->Release();
//
//	return  D3DCompileFromFile(FileName,
//							   NULL,//Defines
//							   D3D_COMPILE_STANDARD_FILE_INCLUDE,//No includes
//							   "main",//Entry point
//							   "cs_5_0",//Version (sometimes other stuff)
//							   D3DCOMPILE_ENABLE_STRICTNESS,//Flags1
//							   0,//Flags2 not used
//							   &m_ShaderBlob,//Output bytecode
//							   &m_ErrorBlob);//Output errors
//}
