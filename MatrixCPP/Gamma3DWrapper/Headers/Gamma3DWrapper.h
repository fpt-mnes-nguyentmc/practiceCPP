#pragma once

#include <Windows.h>

#include "CLIBasicParam.h"
#include "Gamma3DLogic.h"
#include "Gamma3DWrapperConverter.h"

namespace Gamma3DWrapper
{
	public ref class CGamma3DWrapper
	{
	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="CGamma3DWrapper" /> class.
		/// </summary>
		CGamma3DWrapper();

		/// <summary>
		/// Finalizes an instance of the <see cref="CGamma3DWrapper" /> class.
		/// </summary>
		~CGamma3DWrapper();

		/// <summary>
		/// Sets the window handle.
		/// </summary>
		/// <param name="pHwnd">The p HWND.</param>
		void SetImageHostHandle(IntPtr^ pHwnd);

		/// <summary>
		/// Sets the gamma input.
		/// </summary>
		/// <param name="pGammaModel">The p gamma model.</param>
		void SetGammaInputParam(GammaInputModel^ pGammaModel);

		/// <summary>
		/// Extracts the vol file.
		/// </summary>
		/// <param name="strEvalFilePath">The string eval file path.</param>
		/// <param name="strRefFilePath">The string reference file path.</param>
		void ExtractGammaVolume(String^ strEvalFilePath, String^ strRefFilePath);

		/// <summary>
		/// Extracts the vol file to X,Y,Z axis.
		/// </summary>
		/// <param name="strEvalFilePath">The string eval file path.</param>
		/// <param name="strRefFilePath">The string reference file path.</param>
		//void ExtractGammaVolumeToAxis(int iXAxis, int iYAxis, int iZAxis, double dNormalize);
		void ExtractGammaVolumeToAxis(Int32 iXAxis, Int32 iYAxis, Int32 iZAxis, Double dNormalizeNumber);

	private:
		/// <summary>
		/// The m p logic
		/// </summary>
		CGamma3DLogic* m_pLogic;
		/// <summary>
		/// The m p gamma model
		/// </summary>
		GammaInputModel^ m_pGammaModel;
	};
}