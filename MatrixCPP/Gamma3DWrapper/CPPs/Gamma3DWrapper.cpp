#include "Gamma3DWrapper.h"

#define BUFFER_SIZE 16384

namespace Gamma3DWrapper
{
	/// <summary>
	/// Initializes a new instance of the <see cref="CGamma3DWrapper"/> class.
	/// </summary>
	CGamma3DWrapper::CGamma3DWrapper()
	{
		this->m_pLogic = new CGamma3DLogic();
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="CGamma3DWrapper"/> class.
	/// </summary>
	CGamma3DWrapper::~CGamma3DWrapper()
	{
	}

	/// <summary>
	/// Extracts the vol file.
	/// </summary>
	/// <param name="strFilePath">The string file path.</param>
	void CGamma3DWrapper::ExtractGammaVolume(String^ strEvalFilePath, String^ strRefFilePath)
	{
		std::string strEvalPath = CLI::ToUnmanaged(strEvalFilePath);
		std::string strRefPath = CLI::ToUnmanaged(strRefFilePath);

		this->m_pLogic->ExtractGammaVolume(strEvalPath, strRefPath);
	}
}