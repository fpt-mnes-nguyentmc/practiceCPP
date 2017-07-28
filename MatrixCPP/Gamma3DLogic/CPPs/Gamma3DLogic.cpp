#include "Gamma3DLogic.h"

#define BUFFER_SIZE 16384

// =================================== Global constant here ===========================================
// Global Constant variables
const int gl_xDir[26] = {  0,  0, -1,  1,  0, -1,  1, -1,  1,  0, -1,  1,  0, -1,  1, -1,  1,  0,  0, -1,  1,  0, -1,  1, -1,  1 };
const int gl_yDir[26] = { -1,  0,  0,  0,  1, -1, -1,  1,  1, -1,  0,  0,  1, -1, -1,  1,  1, -1,  0,  0,  0,  1, -1, -1,  1,  1 };
const int gl_zDir[26] = { -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1 };

const int gl_xInterDir[8] = { -1,  0, -1,  0, -1,  0, -1, 0 };
const int gl_yInterDir[8] = { -1, -1, -1, -1,  0,  0,  0, 0 };
const int gl_zInterDir[8] = { -1, -1,  0,  0, -1, -1,  0, 0 };

// Constant S element directions for [V1r, V2r, V3r, V4r]
const int gl_SArrays[5][4][3] = 
{
	{ {0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {1, 1, 0} },
	{ {0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {1, 0, 1} },
	{ {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0} },
	{ {0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 0} },
	{ {0, 1, 1}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1} }
};
// ====================================================================================================


/// <summary>
/// Initializes a new instance of the <see cref="CGamma3DLogic"/> class.
/// </summary>
CGamma3DLogic::CGamma3DLogic()
{
	this->m_pEvalVol = nullptr;
	this->m_pRefVol = nullptr;
	this->m_dWx = this->m_dWy = this->m_dWz = 0;
	this->m_iX = this->m_iY = this->m_iZ = 0;
}

/// <summary>
/// Finalizes an instance of the <see cref="CGamma3DLogic" /> class.
/// </summary>
CGamma3DLogic::~CGamma3DLogic()
{
	// Free the evaluation & ref matrix
	for (int i = 0; i < this->m_dWx; i++)
	{
		for (int j = 0; j < this->m_dWy; j++)
		{
			delete[] m_pEvalVol[i][j];
			delete[] m_pRefVol[i][j];
		}

		delete[] m_pEvalVol[i];
		delete[] m_pRefVol[i];
	}

	delete[] m_pEvalVol;
	delete[] m_pRefVol;
}

/// <summary>
/// Sets the window handle.
/// </summary>
/// <param name="pHwnd">The p HWND.</param>
void CGamma3DLogic::SetImageHostHandle(HWND pHwnd)
{
	this->m_pImgHostHwnd = pHwnd;
	this->m_pImgDC = GetDC(pHwnd);
}

/// <summary>
/// Sets the gamma input parameter.
/// </summary>
/// <param name="cInputParam">The c input parameter.</param>
void CGamma3DLogic::SetGammaInputParam(CGammaInput cInputParam)
{
	this->m_cInput = cInputParam;
}

/// <summary>
/// Calculate the coresponding Dose to each Axis  
/// </summary>
/// <param name="iXaxis">The iXaxis input parameter.</param>
/// <param name="iYaxis">The iYaxis input parameter.</param>
/// <param name="iZaxis">The iZaxis input parameter.</param>
/// <param name="dNormalizeNumber">The dNormalizeNumber input parameter.</param>
void CGamma3DLogic::ExtractGammaVolumeToAxis(int &iXaxis, int &iYaxis, int &iZaxis, double &dNormalizeNumber)
{

	float*** pVols[2] = { this->m_pEvalVol, this->m_pRefVol }; // Get from each volume files
	float** pDose[2] = { this->m_fDoseRef, this->m_fDoseEval }; // Assign each Dose to each elem
	int i = 0;

	// To boost performance, Using parrallel to loop 2 type of Volume and process it in the same time
#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (int i = 0; i < 2; i++)
	{ 
		CalculateDose(pDose[i], pVols[i], iXaxis, iYaxis, iZaxis, this->m_dWx);
	}

	// pDoseRef: multiply to dNormalizeNumber
	for(i ; i < m_dWx ; i++)
	{
		pDose[1][0][i] *= (float)dNormalizeNumber;
		pDose[1][1][i] *= (float)dNormalizeNumber;
		pDose[1][2][i] *= (float)dNormalizeNumber;
	}

	this->m_cAxisOutput.pDoseEval = pDose[0];
	this->m_cAxisOutput.pDoseRef = pDose[1];
	this->m_cAxisOutput.m_iWsize = this->m_dWx;
}


/// <summary>
/// Extract Dose value to each Axis  
/// </summary>
/// <param name="pDose">The pDose input/output parameter.</param>
/// <param name="pVols">The pVols input parameter.</param>
/// <param name="iXaxis">The iXaxis input parameter.</param>
/// <param name="iYaxis">The iYaxis input parameter.</param>
/// <param name="iZaxis">The iZaxis input parameter.</param>
/// <param name="iWx">The iWx input parameter - wide X.</param>
void CGamma3DLogic::CalculateDose(float** pDose, float*** pVols, int &iXaxis, int &iYaxis, int &iZaxis, int &iWx)
{
	pDose[0] = new float[iWx]; //pDose[0][X elems]
	pDose[1] = new float[iWx]; //pDose[1][Y elems]
	pDose[2] = new float[iWx]; //pDose[2][Z elems]

	/*for (int i = 0; i < iWx; i++)
	{
	pDose[0][i] = pVols[i][iYaxis][iZaxis];
	}

	for (int j = 0; j < iWy; j++)
	{
	pDose[1][j] = pVols[iXaxis][j][iZaxis];
	}

	for (int k = 0; k < iWz; k++)
	{
	pDose[2][k] = pVols[iXaxis][iYaxis][k];
	}*/

	for (int i = 0; i < iWx; i++)
	{
		pDose[0][i] = pVols[i][iYaxis][iZaxis];
		pDose[1][i] = pVols[iXaxis][i][iZaxis];
		pDose[2][i] = pVols[iXaxis][iYaxis][i];
	}
}

/// <summary>
/// Extracts the pVol file.
/// </summary>
/// <param name="strFilePath">The string file path.</param>
void CGamma3DLogic::ExtractGammaVolume(const std::string& strEvalFilePath, const std::string& strRefFilePath)
{
	// Open file VOL to read volxel
	FILE* pEvalFile = fopen(strEvalFilePath.c_str(), "rb" );
	FILE* pRefFile  = fopen(strRefFilePath.c_str(), "rb" );

	// Check invalide file
	if (pEvalFile == NULL || pRefFile == NULL) 
	{
		// TODO: Check invalid open file
		return;
	}

	// obtain file size:
	fseek(pEvalFile, 0, SEEK_END);
	fseek(pRefFile, 0, SEEK_END);

	long lSize = ftell(pEvalFile);

	if(lSize != ftell(pRefFile))
	{
		// TODO: Check Wx, Wy, Wz different here
		return;
	}

	// Determine Width, Height, Depth of volume size
	this->m_dWx = this->m_dWy = this->m_dWz = (int)(pow(lSize / 4.0, 1.0 / 3.0) + 0.5);

	// Init space for volsize
	InitVolSize();

	rewind (pEvalFile);
	rewind (pRefFile);

	FILE* lstFile[2] = { pEvalFile, pRefFile };
	float*** pVols[2] = { this->m_pEvalVol, this->m_pRefVol };

	// To boost performance, Using parrallel to loop 2 file and process it in the same time
#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (int i = 0; i < 2; i++)
	{
		ReadAndMappingFile(lstFile[i], pVols[i]);
	}

	// Close the Evaluation volume file
	fflush(pEvalFile);
	fclose(pEvalFile);

	// Close the Reference volume file
	fflush(pRefFile);
	fclose(pRefFile);
}

/// <summary>
/// Initializes the size of the vol.
/// </summary>
void CGamma3DLogic::InitVolSize()
{
	// Initialize [Exam, Reference & Gamma] volume
	this->m_pEvalVol = new float**[this->m_dWx];
	this->m_pRefVol = new float**[this->m_dWx];
	this->m_pGammaVol = new double**[this->m_dWx];

	for (int i = 0; i < this->m_dWx; i++)
	{
		this->m_pEvalVol[i] = new float*[this->m_dWy];
		this->m_pRefVol[i] = new float*[this->m_dWy];
		this->m_pGammaVol[i] = new double*[this->m_dWy];

		for (int j = 0; j < this->m_dWy; j++)
		{
			this->m_pEvalVol[i][j] = new float[this->m_dWz];
			this->m_pRefVol[i][j] = new float[this->m_dWz];
			this->m_pGammaVol[i][j] = new double[this->m_dWz];

			memset(this->m_pEvalVol[i][j], 0, this->m_dWz * sizeof(float));
			memset(this->m_pRefVol[i][j], 0, this->m_dWz * sizeof(float));
			memset(this->m_pGammaVol[i][j], 0, this->m_dWz * sizeof(double));
		}
	}
}

/// <summary>
/// Reads the and mapping file.
/// </summary>
/// <param name="strFile">The string file.</param>
/// <param name="pVol">The p vol.</param>
void CGamma3DLogic::ReadAndMappingFile(FILE* pFile, float*** pVol)
{
	// Create buffer to read file
	char* buffer = new char[BUFFER_SIZE];
	size_t length = 0, index = 0;

	// Move to begin file
	rewind(pFile);

	// Read buffer file and remember it into the matrix volume
	while ((length = fread(buffer, 1, BUFFER_SIZE, pFile)) > 0)
	{
		// Determime the position of voxel in volume file
		for(size_t n = 0, i = 0, j = 0, k = 0, idx = 0; n < length; n += 4)
		{
			idx = (index + n) / 4;
			i = idx % this->m_dWx;
			j = (idx / this->m_dWx) % this->m_dWy;
			k = idx / (this->m_dWx * this->m_dWy);

			// Store voxel into the matrix volume
			pVol[i][j][k] = *(float*)(buffer + n);
		}

		// Increase index to read buffer
		index += length;
	}

	// Free buffer file
	delete[] buffer;
}

/// <summary>
/// Implements the algorithm.
/// </summary>
void CGamma3DLogic::ImplementAlgorithm()
{
	// Initialize variables
	int temp = 0;
	int Wx = this->m_dWx, Wy = this->m_dWy, Wz = this->m_dWz;
	int stX = 0, stY = 0, stZ = 0;
	int iDiameter = (int)(this->m_cInput.m_dRadius * 2 + 0.5);
	int iMetricVol = (int)((4.0 * M_PI * pow(this->m_cInput.m_dRadius, 3) / 3.0) + 0.5);
	int nMetricVol = 0;

	double dTemp = 0.0;
	double dRadiusSquared = this->m_cInput.m_dRadius * this->m_cInput.m_dRadius;

	if (this->m_cInput.m_dRadius <= 3)
	{
		iMetricVol = (int)(pow(this->m_cInput.m_dRadius * 2, 3) + 0.5);
	}

	// The center position of translate matrix
	TPoint4D pCenPos((int)this->m_cInput.m_dRadius, (int)this->m_cInput.m_dRadius, (int)this->m_cInput.m_dRadius, 0);
	std::queue<TPoint4D> qPoints;
	TPoint4D* pTranPoints = new TPoint4D[iMetricVol];

	// Initialize & set default value as [0] for the tranlate matrix
	double*** pTransMatrix = new double**[iDiameter];
	for (int i = 0; i < iDiameter; i++)
	{
		pTransMatrix[i] = new double*[iDiameter];
		for (int j = 0; j < iDiameter; j++)
		{
			pTransMatrix[i][j] = new double[iDiameter];

			// Set default value as [0]
			memset(pTransMatrix[i][j], 0, iDiameter * sizeof(double));
		}
	}

	// Using loop 8 directions to find the voxel in the 90 angle 3D
	for (int i = 0; i < 8; i++)
	{
		// Find the voxel in the 90 angle 3D
		TPoint4D p(pCenPos.X + gl_xInterDir[i], 
			pCenPos.Y + gl_yInterDir[i], 
			pCenPos.Z + gl_zInterDir[i], 1);

		// Remember the distance of voxel to 
		pTransMatrix[p.X][p.Y][p.Z] = p.W;

		// Push it into the queue
		qPoints.emplace(p);
		pTranPoints[nMetricVol++] = p;
	}

	// Using queue to mark the distance in the matrix volume. 
	// It will create the list of point which will be used to look up and sort the voxels
	while (qPoints.size() > 0)
	{
		// Get value on the FIFO method
		TPoint4D pCurrent = qPoints.front();

		// Pop the first element
		qPoints.pop();

		// Checking the neighbour points
		for (int i = 0; i < 26; i++)
		{
			// Get the neighbour point
			TPoint4D pNext(pCurrent.X + gl_xDir[i], pCurrent.Y + gl_yDir[i], pCurrent.Z + gl_zDir[i], 0);

			// Check The neighbour point:
			// - Belong to the translate matrix.
			// - It's not marked 
			// - Distance from the center point of translate matrix
			if (0 <= pNext.X && pNext.X < iDiameter &&
				0 <= pNext.Y && pNext.Y < iDiameter &&
				0 <= pNext.Z && pNext.Z < iDiameter &&
				pTransMatrix[pNext.X][pNext.Y][pNext.Z] <= 0)
			{
				dTemp = pNext.SquaredDistanceFrom(pCenPos);

				if (dTemp <= dRadiusSquared)
				{
					// Update the [Ln] distance
					pTransMatrix[pNext.X][pNext.Y][pNext.Z] = pNext.W = dTemp;

					// Enqueue the neighbour point
					qPoints.emplace(pNext);

					// Keep it in vector using in the next step
					pTranPoints[nMetricVol++] = pNext;
				}
			}
		}
	}

	switch (this->m_cInput.m_ePlane)
	{
	case EGammaPlane::Axial:
		stZ = this->m_cInput.m_iPosPlan;
		Wz = stZ + 1;
		break;
	case EGammaPlane::Coronal:
		stY = this->m_cInput.m_iPosPlan;
		Wy = stY + 1;
		break;
	case EGammaPlane::Sagital:
		stX = this->m_cInput.m_iPosPlan;
		Wx = stX + 1;
		break;
	default:
		break;
	}

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	// Loop all voxel of the examination volume
	for (int x = stX; x < Wx; x++)
	{
		for (int y = stY; y < Wy; y++)
		{
			for (int z = stZ; z < Wz; z++)
			{
				// Only check the value of point having Dose value larger than [Threshold Dose Min]
				if (this->m_pEvalVol[x][y][z] > this->m_cInput.m_dThresholdDoseMin)
				{
					int nRefPoints = 0;
					double dGammaExamValue = 0;

					// The examination point
					TPoint4D pExamPoint(x, y, z, this->m_pEvalVol[x][y][z]);

					// The reference point
					TPoint4D pRefPoint(x, y, z, this->m_pRefVol[x][y][z]);

					// Translate the ref point to the left-top position
					TPoint4D pRefTranPos(pRefPoint.Substract(this->m_cInput.m_dRadius));
					TPoint4D V1r, V2r, V3r, V4r, pDelta;
					TPoint4D* pRefPoints = new TPoint4D[nMetricVol];

					double P1, P2, P3, P4;
					double Vq1, Vq2, Vq3, Vq4;
					double w1, w2, w3, w4;
					double V00, V01, V02, V10, V11, V12, V20, V21, V22, V30, V31, V32;
					double VTV00, VTV01, VTV02, VTV10, VTV11, VTV12, VTV20, VTV21, VTV22;
					double iVTV00, iVTV01, iVTV02, iVTV10, iVTV11, iVTV12, iVTV20, iVTV21, iVTV22;
					double X1r, Y1r, Z1r, W1r, X2r, Y2r, Z2r, W2r, X3r, Y3r, Z3r, W3r, X4r, Y4r, Z4r, W4r;
					double iVTV_VT00, iVTV_VT01, iVTV_VT02, iVTV_VT03, iVTV_VT10, iVTV_VT11, iVTV_VT12, iVTV_VT13, iVTV_VT20, iVTV_VT21, iVTV_VT22, iVTV_VT23;
					double invertDet, dGammaMin, dGammaTmp;

					// Using look up matrix to the reference points & Order it by [Distance from pRefPoint] and then by [its Index in volume]
					for (TPoint4D* it = pTranPoints, *length = pTranPoints + nMetricVol; it < length ; ++it)
					{
						// Translate the voxel in the translate matrix
						pDelta.X = pRefTranPos.X + it->X;
						pDelta.Y = pRefTranPos.Y + it->Y;
						pDelta.Z = pRefTranPos.Z + it->Z;

						// Check The neighbour point:
						// - Belong to the translate matrix.
						// - It's not marked 
						// - Distance from the center point of ref matrix volume
						if (0 <= pDelta.X && pDelta.X < this->m_dWx &&
							0 <= pDelta.Y && pDelta.Y < this->m_dWy &&
							0 <= pDelta.Z && pDelta.Z < this->m_dWz &&
							pTransMatrix[it->X][it->Y][it->Z] > 0 &&
							this->m_pRefVol[pDelta.X][pDelta.Y][pDelta.Z] >= this->m_cInput.m_dThresholdDoseMin)
						{
							// Keep the voxel value
							pDelta.W = it->W;

							pRefPoints[nRefPoints++] = pDelta;
						}
					}

					// Finding the Y-index value of ref points
					for (TPoint4D* it = pRefPoints, *length = pRefPoints + nRefPoints; it < length ; ++it)
					{
						dGammaMin = DBL_MAX;

						// Calculate the gamma of [S] element
						for (int S = 0; S < 5; S++)
						{
							V1r.X = V2r.X = V3r.X = V4r.X = it->X;
							V1r.Y = V2r.Y = V3r.Y = V4r.Y = it->Y;
							V1r.Z = V2r.Z = V3r.Z = V4r.Z = it->Z;

							switch (S)
							{
							case 0:
								// Determine Vmr of element S1
								V2r.Y = it->Y + 1;
								V3r.Y = it->Y + 1;
								V3r.Z = it->Z + 1;
								V4r.X = it->X + 1;
								V4r.Y = it->Y + 1;
								break;
							case 1:
								// Determine Vmr of element S2
								V2r.Z = it->Z + 1;
								V3r.Y = it->Y + 1;
								V3r.Z = it->Z + 1;
								V4r.X = it->X + 1;
								V4r.Y = it->Y + 1;
								break;
							case 2:
								// Determine Vmr of element S3
								V2r.X = it->X + 1;
								V3r.X = it->X + 1;
								V3r.Z = it->Z + 1;
								V4r.X = it->X + 1;
								V4r.Y = it->Y + 1;
								break;
							case 3:
								// Determine Vmr of element S4
								V2r.Y = it->Y + 1;
								V2r.Z = it->Z + 1;
								V3r.X = it->X + 1;
								V3r.Z = it->Z + 1;
								V4r.X = it->X + 1;
								V4r.Y = it->Y + 1;
								break;
							case 4:
								// Determine Vmr of element S5
								V1r.Y = it->Y + 1;
								V1r.Z = it->Z + 1;
								V2r.X = it->X + 1;
								V2r.Z = it->Z + 1;
								V3r.X = it->X + 1;
								V3r.Y = it->Y + 1;
								V4r.X = it->X + 1;
								V4r.Y = it->Y + 1;
								V4r.Z = it->Z + 1;
								break;
							default:
								break;
							}

							// Get Ln
							V1r.W = this->m_pRefVol[V1r.X][V1r.Y][V1r.Z];
							V2r.W = this->m_pRefVol[V2r.X][V2r.Y][V2r.Z];
							V3r.W = this->m_pRefVol[V3r.X][V3r.Y][V3r.Z];
							V4r.W = this->m_pRefVol[V4r.X][V4r.Y][V4r.Z];

							// Convert int to double type
							X1r = V1r.X; Y1r = V1r.Y; Z1r = V1r.Z; W1r = V1r.W;
							X2r = V2r.X; Y2r = V2r.Y; Z2r = V2r.Z; W2r = V2r.W;
							X3r = V3r.X; Y3r = V3r.Y; Z3r = V3r.Z; W3r = V3r.W;
							X4r = V4r.X; Y4r = V4r.Y; Z4r = V4r.Z; W4r = V4r.W;

							// Determine matrix [P]:
							//|-----------------|
							//| (Xp - X4r) / Δd |
							//| (Yp - Y4r) / Δd |
							//| (Zp - Z4r) / Δd |
							//| (Dp - D4r) / ΔD |
							//|-----------------|
							P1 = (pExamPoint.X - V4r.X) / this->m_cInput.m_dMaxDeltaPos;
							P2 = (pExamPoint.Y - V4r.Y) / this->m_cInput.m_dMaxDeltaPos;
							P3 = (pExamPoint.Z - V4r.Z) / this->m_cInput.m_dMaxDeltaPos;
							P4 = (pExamPoint.W - V4r.W) / this->m_cInput.m_dMaxDeltaDose;

							// Determine matrix [V]:
							//|-------------------------------------------------------|
							//| (X1r - X4r) / Δd, (X2r - X4r) / Δd, (X3r - X4r) / Δd, |
							//| (Y1r - Y4r) / Δd, (Y2r - Y4r) / Δd, (Y3r - Y4r) / Δd, |
							//| (Z1r - Z4r) / Δd, (Z2r - Z4r) / Δd, (Z3r - Z4r) / Δd, |
							//| (D1r - D4r) / ΔD, (D2r - D4r) / ΔD, (D3r - D4r) / ΔD, |
							//|-------------------------------------------------------|
							V00 = (X1r - X4r) / this->m_cInput.m_dMaxDeltaPos;
							V10 = (Y1r - Y4r) / this->m_cInput.m_dMaxDeltaPos;
							V20 = (Z1r - Z4r) / this->m_cInput.m_dMaxDeltaPos;
							V30 = (W1r - W4r) / this->m_cInput.m_dMaxDeltaDose;

						}

						if (it->W > dGammaMin)
						{
							dGammaExamValue = dGammaMin;
						}
					}

					this->m_pGammaVol[x][y][z] = dGammaExamValue;

					delete[] pRefPoints;
				}
			}
		}
	}

	// Free the tranlate matrix
	for (int i = 0; i < iDiameter; i++)
	{
		for (int j = 0; j < iDiameter; j++)
		{
			delete[] pTransMatrix[i][j];
		}

		delete[] pTransMatrix[i];
	}

	delete[] pTransMatrix;
}