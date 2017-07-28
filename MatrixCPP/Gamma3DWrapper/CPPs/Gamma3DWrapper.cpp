#include "Gamma3DWrapper.h"

namespace Gamma3DWrapper
{

	CGamma3DWrapper::CGamma3DWrapper()
	{
		this->m_pLogic = new CGamma3DLogic();
	}

	CGamma3DWrapper::~CGamma3DWrapper()
	{
	}

	void CGamma3DWrapper::SetImageHostHandle(IntPtr^ pHwnd)
	{
		this->m_pLogic->SetImageHostHandle(CLI::ToUnmanaged(pHwnd));
	}

	void CGamma3DWrapper::ExtractGammaVolume(String^ strEvalFilePath, String^ strRefFilePath)
	{
		std::string strEvalPath = CLI::ToUnmanaged(strEvalFilePath);
		std::string strRefPath = CLI::ToUnmanaged(strRefFilePath);

		this->m_pLogic->ExtractGammaVolume(strEvalPath, strRefPath);
	}

	void CGamma3DWrapper::SetGammaInputParam(GammaInputModel^ pGammaModel)
	{
		this->m_pLogic->SetGammaInputParam(CLI::ToUnmanaged(pGammaModel));
	}

	void CGamma3DWrapper::ExtractGammaVolumeToAxis(Int32 iXAxis, Int32 iYAxis, Int32 iZAxis, Double dNormalize)
	{
		int iXA = CLI::ToUnmanaged(iXAxis);
		int iYA = CLI::ToUnmanaged(iYAxis);
		int iZA = CLI::ToUnmanaged(iZAxis);
		double dNor = CLI::ToUnmanaged(dNormalize);

		this->m_pLogic->ExtractGammaVolumeToAxis(iXA, iYA, iZA, dNor);
	}
}