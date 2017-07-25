#pragma once

#include <stdio.h>
#include <string>
#include <queue>

#include "Point4D.h"


class CGamma3DLogic
{
public:
	CGamma3DLogic();
	~CGamma3DLogic();

	void ExtractGammaVolume(const std::string& strEvalFilePath, const std::string& strRefFilePath);
private:
	void ReadAndMappingFile(FILE* pFile, float*** pVol, int& Wx, int &Wy, int& Wz);
	void ImplementAlgorithm();

	float*** m_pEvalVol;
	float*** m_pRefVol;

	int m_dWx;
	int m_dWy;
	int m_dWz;

	double m_dThresholdDoseMin;
	double m_dMaxDeltaPos;
	double m_dMaxDeltaDose;
	double m_dRadius;
};