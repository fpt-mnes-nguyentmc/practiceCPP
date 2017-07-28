#pragma once

enum EGammaPlane
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

class CGammaInput
{
public:
	/// <summary>
	/// The plane
	/// </summary>
	EGammaPlane m_ePlane;
	/// <summary>
	/// The position plan
	/// </summary>
	int m_iPosPlan;
	/// <summary>
	/// The threshold dose minimum
	/// </summary>
	double m_dThresholdDoseMin;
	/// <summary>
	/// The maximum delta position
	/// </summary>
	double m_dMaxDeltaPos;
	/// <summary>
	/// The maximum delta dose
	/// </summary>
	double m_dMaxDeltaDose;
	/// <summary>
	/// The radius
	/// </summary>
	double m_dRadius;
};