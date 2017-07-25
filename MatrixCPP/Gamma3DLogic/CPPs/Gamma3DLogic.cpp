#include "Gamma3DLogic.h"

#define BUFFER_SIZE 16384

// =================================== Global constant here ===========================================
// Global Constant variables
const int gl_xDir[6] = {  0,  0, -1, 0, 0, 1 };
const int gl_yDir[6] = {  0, -1,  0, 0, 1, 0 };
const int gl_zDir[6] = { -1,  0,  0, 1, 0, 0 };

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
	this->m_dThresholdDoseMin = this->m_dMaxDeltaPos = this->m_dMaxDeltaDose = this->m_dRadius = 0.0;
}

/// <summary>
/// Finalizes an instance of the <see cref="CGamma3DLogic"/> class.
/// </summary>
CGamma3DLogic::~CGamma3DLogic()
{
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

	if(ftell(pEvalFile) != ftell(pRefFile))
	{
		// TODO: Check Wx, Wy, Wz different here
		return;
	}

	rewind (pEvalFile);
	rewind (pRefFile);

	FILE* lstFile[2] = { pEvalFile, pRefFile };
	float*** pVols[2] = { this->m_pEvalVol, this->m_pRefVol };
	int dWs[2][3] = { 0 };

	// To boost performance, Using parrallel to loop 2 file and process it in the same time
#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for (int i = 0; i < 2; i++)
	{
		ReadAndMappingFile(lstFile[i], pVols[i], dWs[i][0], dWs[i][1], dWs[i][2]);
	}

	// Close the Evaluation volume file
	fflush(pEvalFile);
	fclose(pEvalFile);

	// Close the Reference volume file
	fflush(pRefFile);
	fclose(pRefFile);
}

/// <summary>
/// Reads the and mapping file.
/// </summary>
/// <param name="strFile">The string file.</param>
/// <param name="pVol">The p vol.</param>
void CGamma3DLogic::ReadAndMappingFile(FILE* pFile, float*** pVol, int& Wx, int &Wy, int& Wz)
{
	// obtain file size:
	fseek(pFile , 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);

	// Create buffer to read file
	char* buffer = new char[BUFFER_SIZE];
	int N, length = 0, index = 0;

	// Determine Width, Height, Depth of volume size
	Wx = Wy = Wz = (int)(pow(lSize / 4.0, 1.0 / 3.0) + 0.5);

	// Initialize [Examl & Reference] volume
	pVol = new float**[Wx];
	for (int i = 0; i < Wx; i++)
	{
		pVol[i] = new float*[Wy];
		for (int j = 0; j < Wy; j++)
		{
			pVol[i][j] = new float[Wz];
			memset(pVol[i][j], 0, Wz * sizeof(float));
		}
	}

	// Read buffer file and remember it into the matrix volume
	while ((length = fread(buffer, 1, BUFFER_SIZE, pFile)) > 0)
	{
		// Determime the position of voxel in volume file
		for(int n = 0, i = 0, j = 0, k = 0, idx = 0; n < length; n += 4)
		{
			idx = (index + n) / 4;
			i = idx % Wx;
			j = (idx / Wx) % Wy;
			k = idx / (Wx * Wy);

			// Store voxel into the matrix volume
			pVol[i][j][k] = *(float*)(buffer + n);
		}

		// Increase index to read buffer
		index += length;
	}

	// Free buffer file
	free(buffer);
}

/// <summary>
/// Implements the algorithm.
/// </summary>
void CGamma3DLogic::ImplementAlgorithm()
{
	// Initialize variables
	int temp = 0;

	// TODO: Hard code the search distance radius
	int dRadiusSquared = this->m_dRadius * this->m_dRadius;
	int iDiameter = this->m_dRadius * 2;

	// The center position of translate matrix
	TPoint4D pCenPos(this->m_dRadius, this->m_dRadius, this->m_dRadius, 0);
	std::queue<TPoint4D> qPoints;
	std::vector<TPoint4D> vtTranPoints;

	// Initialize & set default value as [0] for the tranlate matrix
	int*** pTransMatrix = new int**[iDiameter];
	for (int i = 0; i < iDiameter; i++)
	{
		pTransMatrix[i] = new int*[iDiameter];
		for (int j = 0; j < iDiameter; j++)
		{
			pTransMatrix[i][j] = new int[iDiameter];

			// Set default value as [0]
			memset(pTransMatrix[i][j], 0, iDiameter * sizeof(int));
		}
	}

	// Using loop 8 directions to find the voxel in the 90 angle 3D
	for (int i = 0; i < 8; i++)
	{
		// Find the voxel in the 90 angle 3D
		TPoint4D p(this->m_dRadius + gl_xInterDir[i], this->m_dRadius + gl_yInterDir[i], this->m_dRadius + gl_zInterDir[i], 1);

		// Remember the distance of voxel to 
		pTransMatrix[p.X][p.Y][p.Z] = p.W;

		// Push it into the queue
		qPoints.emplace(p);
		vtTranPoints.emplace_back(p);
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
		for (int i = 0; i < 6; i++)
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
				pTransMatrix[pNext.X][pNext.Y][pNext.Z] <= 0 && 
				pNext.SquaredDistanceFrom(pCenPos) <= dRadiusSquared)
			{
				// Update the [Ln] distance
				pTransMatrix[pNext.X][pNext.Y][pNext.Z] = pNext.W = pCurrent.W + 1;

				// Enqueue the neighbour point
				qPoints.emplace(pNext);

				// Keep it in vector using in the next step
				vtTranPoints.emplace_back(pNext);
			}
		}
	}

#if defined(_OPENMP)
#pragma omp parallel for
#endif
	// Loop all voxel of the examination volume
	for (int x = 0; x < this->m_dWx; x++)
	{
		for (int y = 0; y < this->m_dWy; y++)
		{
			for (int z = 0; z < this->m_dWz; z++)
			{
				// Only check the value of point having Dose value larger than [Threshold Dose Min]
				if (this->m_pEvalVol[x][y][z] > this->m_dThresholdDoseMin)
				{
					// The examination point
					TPoint4D pExamPoint(x, y, z, this->m_pEvalVol[x][y][z]);

					// The reference point
					TPoint4D pRefPoint(x, y, z, this->m_pRefVol[x][y][z]);

					// Translate the ref point to the left-top position
					TPoint4D pRefTranPos(pRefPoint.Substract(this->m_dRadius));

					std::vector<TPoint4D> vtRefPoints;

					// Using look up matrix to the reference points & Order it by [Distance from pRefPoint] and then by [its Index in volume]
					for (auto&& it = vtTranPoints.begin(); it != vtTranPoints.end() ; ++it)
					{
						// Translate the voxel in the translate matrix
						TPoint4D pDelta(pRefTranPos.X + it->X, pRefTranPos.Y + it->Y, pRefTranPos.Z + it->Z, 0);

						// Check The neighbour point:
						// - Belong to the translate matrix.
						// - It's not marked 
						// - Distance from the center point of ref matrix volume
						if (0 <= pDelta.X && pDelta.X < this->m_dWx &&
							0 <= pDelta.Y && pDelta.Y < this->m_dWy &&
							0 <= pDelta.Z && pDelta.Z < this->m_dWz &&
							pTransMatrix[it->X][it->Y][it->Z] > 0 &&
							this->m_pRefVol[pDelta.X][pDelta.Y][pDelta.Z] >= this->m_dThresholdDoseMin)
						{
							// Keep the voxel value
							pDelta.W = this->m_pRefVol[pDelta.X][pDelta.Y][pDelta.Z];

							vtRefPoints.emplace_back(pDelta);
						}
					}

					// Finding the Y-index value of ref points
					for (auto&& it = vtRefPoints.begin(); it != vtRefPoints.end() ; ++it)
					{
						// Calculate [S] element
						for (int S = 0; S < 5; S++)
						{
							std::vector<TPoint4D> Vmrs;

							// Determine [V1r, V2r, V3r, V4r]
							for (int SPos = 0; SPos < 4; SPos++)
							{
								// Determine the position of [V1r, V2r, V3r, V4r]
								TPoint4D sPoint(it->X + gl_SArrays[S][SPos][0], 
									it->Y + gl_SArrays[S][SPos][1], 
									it->Z + gl_SArrays[S][SPos][2],
									0);

								// Check out of range of [V1r, V2r, V3r, V4r]
								if (0 <= sPoint.X && sPoint.X < this->m_dWx &&
									0 <= sPoint.Y && sPoint.Y < this->m_dWy &&
									0 <= sPoint.Z && sPoint.Z < this->m_dWz)
								{
									sPoint.W = this->m_pRefVol[sPoint.X][sPoint.Y][sPoint.Z];

									// Remember them
									Vmrs.emplace_back(sPoint);
								}
								else
								{
									break;
								}
							}

							// Determine matrix [P]:
							//|-----------------|
							//| (Xp - X4r) / Δd |
							//| (Yp - Y4r) / Δd |
							//| (Zp - Z4r) / Δd |
							//| (Dp - D4r) / ΔD |
							//|-----------------|
							double P[4][1] = { 
								{ (pExamPoint.X - Vmrs[3].X) / this->m_dMaxDeltaPos  }, 
								{ (pExamPoint.Y - Vmrs[3].Y) / this->m_dMaxDeltaPos  },
								{ (pExamPoint.Z - Vmrs[3].Z) / this->m_dMaxDeltaPos  },
								{ (pExamPoint.W - Vmrs[3].W) / this->m_dMaxDeltaDose } 
							};

							// Determine matrix [V]:
							//|-------------------------------------------------------|
							//| (X1r - X4r) / Δd, (X2r - X4r) / Δd, (X3r - X4r) / Δd, |
							//| (Y1r - Y4r) / Δd, (Y2r - Y4r) / Δd, (Y3r - Y4r) / Δd, |
							//| (Z1r - Z4r) / Δd, (Z2r - Z4r) / Δd, (Z3r - Z4r) / Δd, |
							//| (D1r - D4r) / ΔD, (D2r - D4r) / ΔD, (D3r - D4r) / ΔD, |
							//|-------------------------------------------------------|
							double V[4][3] = {
								{ (Vmrs[0].X - Vmrs[3].X) / this->m_dMaxDeltaPos,  (Vmrs[1].X - Vmrs[3].X) / this->m_dMaxDeltaPos,  (Vmrs[2].X - Vmrs[3].X) / this->m_dMaxDeltaPos  },
								{ (Vmrs[0].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos,  (Vmrs[1].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos,  (Vmrs[2].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos  },
								{ (Vmrs[0].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos,  (Vmrs[1].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos,  (Vmrs[2].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos  },
								{ (Vmrs[0].W - Vmrs[3].W) / this->m_dMaxDeltaDose, (Vmrs[1].W - Vmrs[3].W) / this->m_dMaxDeltaDose, (Vmrs[2].W - Vmrs[3].W) / this->m_dMaxDeltaDose },
							};

							double VT[3][4] = {
								{ (Vmrs[0].X - Vmrs[3].X) / this->m_dMaxDeltaPos, (Vmrs[0].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos, (Vmrs[0].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos, (Vmrs[0].W - Vmrs[3].W) / this->m_dMaxDeltaDose },
								{ (Vmrs[1].X - Vmrs[3].X) / this->m_dMaxDeltaPos, (Vmrs[1].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos, (Vmrs[1].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos, (Vmrs[1].W - Vmrs[3].W) / this->m_dMaxDeltaDose },
								{ (Vmrs[2].X - Vmrs[3].X) / this->m_dMaxDeltaPos, (Vmrs[2].Y - Vmrs[3].Y) / this->m_dMaxDeltaPos, (Vmrs[2].Z - Vmrs[3].Z) / this->m_dMaxDeltaPos, (Vmrs[2].W - Vmrs[3].W) / this->m_dMaxDeltaDose },
							};
						}
					}
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
