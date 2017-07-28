#pragma once

#include <stdio.h>
#include <string>
#include <queue>
#include <Windows.h>

#include "Point4D.h"
#include "MatrixSolve.h"
#include "GammaInput.h"
#include "CDoseOutput.h"



class CGamma3DLogic
{
public:
	CGamma3DLogic();
	~CGamma3DLogic();

	void SetGammaInputParam(CGammaInput cInputParam);
	void SetImageHostHandle(HWND pHwnd);
	void ExtractGammaVolume(const std::string& strEvalFilePath, const std::string& strRefFilePath);
	void ExtractGammaVolumeToAxis(int iXaxis, int iYaxis, int iZaxis, double dNormalizeNumber);
private:
	void InitVolSize();
	void ReadAndMappingFile(FILE* pFile, float*** pVol);
	void ImplementAlgorithm();
	void CalculateDose(float** pDose, float*** pVols, int iXaxis, int iYaxis, int iZaxis, int iWx, int iWy, int iWz);

	HWND    m_pImgHostHwnd;
	HDC		m_pImgDC;
	HBITMAP m_pImgBitmap;

	float*** m_pEvalVol;
	float*** m_pRefVol;
	float*** m_pGammaVol;

	float** m_fDoseEval;
	float** m_fDoseRef;

	int m_dWx;
	int m_dWy;
	int m_dWz;
	int m_iX;
	int m_iY;
	int m_iZ;

	CGammaInput m_cInput;

	CDoseOutput m_cAxisOutput;
};