#pragma once

#include <Windows.h>

#include "CLIBasicParam.h"
#include "GammaInput.h"

using namespace System;

namespace Gamma3DWrapper
{
	/// <summary>
    /// 
    /// </summary>
    public enum class GammaPlaneEnum
    {
		/// <summary>
		/// The axial
		/// </summary>
		Axial,
		/// <summary>
		/// The coronal
		/// </summary>
		Coronal,
		/// <summary>
		/// The sagital
		/// </summary>
		Sagital
    };

	/// <summary>
	/// 
	/// </summary>
	public ref class GammaInputModel
    {
	public:
		/// <summary>
		/// The plane
		/// </summary>
		GammaPlaneEnum Plane;
		/// <summary>
		/// The position plan
		/// </summary>
		Int32 PosPlan;
		/// <summary>
		/// The threshold dose minimum
		/// </summary>
		Double ThresholdDoseMin;
		/// <summary>
		/// The maximum delta position
		/// </summary>
		Double MaxDeltaPos;
		/// <summary>
		/// The maximum delta dose
		/// </summary>
		Double MaxDeltaDose;
		/// <summary>
		/// The radius
		/// </summary>
		Double Radius;

		/// <summary>
		/// Gammas the input model.
		/// </summary>
		/// <returns></returns>
		GammaInputModel();

		/// <summary>
		/// To the managed.
		/// </summary>
		/// <param name="param">The parameter.</param>
		/// <returns></returns>
		static GammaInputModel^ ToManaged(const CGammaInput& param);

		/// <summary>
		/// To the unmanaged.
		/// </summary>
		/// <param name="param">The parameter.</param>
		/// <returns></returns>
		static CGammaInput ToUnmanaged(GammaInputModel^ param);
	private:
		/// <summary>
		/// To the unmanaged.
		/// </summary>
		/// <returns></returns>
		CGammaInput ToUnmanaged();
    };
}

namespace CLI
{
	using namespace Gamma3DWrapper;

	static CGammaInput ToUnmanaged(GammaInputModel^ param)
	{
		return GammaInputModel::ToUnmanaged(param);
	}
}
