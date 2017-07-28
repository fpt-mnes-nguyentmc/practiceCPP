#include "Gamma3DWrapperConverter.h"

namespace Gamma3DWrapper
{

	GammaInputModel::GammaInputModel()
	{
	}

	GammaInputModel^ GammaInputModel::ToManaged(const CGammaInput& param)
	{
		auto pObj = gcnew GammaInputModel();

		pObj->Plane = static_cast<GammaPlaneEnum>(param.m_ePlane);
		pObj->PosPlan = CLI::ToManaged(param.m_iPosPlan);
		pObj->ThresholdDoseMin = CLI::ToManaged(param.m_dThresholdDoseMin);
		pObj->MaxDeltaPos = CLI::ToManaged(param.m_dMaxDeltaPos);
		pObj->MaxDeltaDose = CLI::ToManaged(param.m_dMaxDeltaDose);
		pObj->Radius = CLI::ToManaged(param.m_dRadius);

		return nullptr;
	}

	CGammaInput GammaInputModel::ToUnmanaged(GammaInputModel^ param)
	{
		return param->ToUnmanaged();
	}

	CGammaInput GammaInputModel::ToUnmanaged()
	{
		CGammaInput obj;

		obj.m_ePlane = static_cast<EGammaPlane>(this->Plane);
		obj.m_iPosPlan = CLI::ToUnmanaged(this->PosPlan);
		obj.m_dThresholdDoseMin = CLI::ToUnmanaged(this->ThresholdDoseMin);
		obj.m_dMaxDeltaPos = CLI::ToUnmanaged(this->MaxDeltaPos);
		obj.m_dMaxDeltaDose = CLI::ToUnmanaged(this->MaxDeltaDose);
		obj.m_dRadius = CLI::ToUnmanaged(this->Radius);

		return obj;
	}
}