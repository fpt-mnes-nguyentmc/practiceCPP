#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;
/// <summary>
/// Define matrix class
/// </summary>
class CMatrix {
private:

	/// <summary>
	/// Define rows x cols
	/// </summary>
	int m_iRows, m_iCols;
	CMatrix();
public:

	/// <summary>
	/// Define value of rows x cols
	/// </summary>
	double **pData;

	/// <summary>
	/// Initializes a new instance of the <see cref="CMatrix"/> struct.
	/// </summary>
	/// <param name="iRows">The rows.</param>
	/// <param name="iCols">The cols.</param>
	CMatrix(int iRows, int iCols) : m_iRows(iRows), m_iCols(iCols)
	{
		pData = new double*[m_iRows];
		for (int i = 0; i < m_iRows; i++)
			pData[i] = new double[m_iCols];
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				pData[i][j] = 0.0;
			}
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="CMatrix"/> struct.
	/// Define template with dynamic rows x columns.
	/// </summary>
	template <size_t rows, size_t cols>
	CMatrix(double(&array)[rows][cols]) :
	m_iRows(rows), m_iCols(cols)
	{
		pData = new double*[m_iRows];
		for (int i = 0; i < m_iRows; i++)
			pData[i] = new double[m_iCols];
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				pData[i][j] = array[i][j];
			}
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="CMatrix"/> struct.
	/// </summary>
	/// <param name="matrix">The matrix.</param>
	CMatrix(const CMatrix &CMatrix_other)
	{
		m_iRows = CMatrix_other.m_iRows;
		m_iCols = CMatrix_other.m_iCols;
		pData = new double*[m_iRows];

		for (int i = 0; i < m_iRows; i++)
			pData[i] = new double[m_iCols];
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				pData[i][j] = CMatrix_other.pData[i][j];
			}
		}
	}

	/// <summary>
	/// Destructor matrix <see cref="CMatrix"/> class.
	/// </summary>
	~CMatrix()
	{
		for (int i = 0; i < m_iRows; i++)
			delete[] pData[i];
		delete[] pData;
		m_iRows = m_iCols = 0;
	}

	/// <summary>
	/// Return the transposed matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <returns>CMatrix Transpose</returns>
	CMatrix Transpose()
	{
		CMatrix Res_cMatrixTransposesed(m_iCols, m_iRows);
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				
				Res_cMatrixTransposesed.pData[j][i] = pData[i][j];
			}
		}
		return Res_cMatrixTransposesed;
	}


	/// <summary>
	/// Return the inversed matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <returns>CMatrix Inverse</returns>
	CMatrix Inverse()
	{
		CMatrix Res_cMatrixInversed(m_iRows, m_iCols);

		double det = pData[0][0] * (pData[1][1] * pData[2][2] - pData[2][1] * pData[1][2]) -
			pData[0][1] * (pData[1][0] * pData[2][2] - pData[1][2] * pData[2][0]) +
			pData[0][2] * (pData[1][0] * pData[2][1] - pData[1][1] * pData[2][0]);

		double invdet = 1.0 / det;

		Res_cMatrixInversed.pData[0][0] = (pData[1][1] * pData[2][2] - pData[2][1] * pData[1][2]) * invdet;
		Res_cMatrixInversed.pData[0][1] = (pData[0][2] * pData[2][1] - pData[0][1] * pData[2][2]) * invdet;
		Res_cMatrixInversed.pData[0][2] = (pData[0][1] * pData[1][2] - pData[0][2] * pData[1][1]) * invdet;
		Res_cMatrixInversed.pData[1][0] = (pData[1][2] * pData[2][0] - pData[1][0] * pData[2][2]) * invdet;
		Res_cMatrixInversed.pData[1][1] = (pData[0][0] * pData[2][2] - pData[0][2] * pData[2][0]) * invdet;
		Res_cMatrixInversed.pData[1][2] = (pData[1][0] * pData[0][2] - pData[0][0] * pData[1][2]) * invdet;
		Res_cMatrixInversed.pData[2][0] = (pData[1][0] * pData[2][1] - pData[2][0] * pData[1][1]) * invdet;
		Res_cMatrixInversed.pData[2][1] = (pData[2][0] * pData[0][1] - pData[0][0] * pData[2][1]) * invdet;
		Res_cMatrixInversed.pData[2][2] = (pData[0][0] * pData[1][1] - pData[1][0] * pData[0][1]) * invdet;

		return Res_cMatrixInversed;
	}

	/// <summary>
	/// Return the added matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <param name="matrix">The matrix.</param>
	/// <returns>Result =>2 Added CMatrix</returns>
	CMatrix operator +(const CMatrix &CMatrix_other)
	{
		if (this->m_iRows != CMatrix_other.m_iRows || this->m_iCols != CMatrix_other.m_iCols)
		{
			// Addition can't be executed => check rows and columns
			return *this;
		}
		CMatrix result(m_iRows, m_iCols);
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				result.pData[i][j] = this->pData[i][j]
				+ CMatrix_other.pData[i][j];
			}
		}
		return result;
	}

	/// <summary>
	/// Return the substracted matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <param name="matrix">The matrix.</param>
	/// <returns>Result =>2 Substracted CMatrix</returns>
	CMatrix operator -(const CMatrix &CMatrix_other)
	{
		if (this->m_iRows != CMatrix_other.m_iRows || this->m_iCols != CMatrix_other.m_iCols)
		{
			// Subtraction can't be executed => check rows and columns
			return *this;
		}
		CMatrix result(m_iRows, m_iCols);
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				result.pData[i][j] = this->pData[i][j]
				- CMatrix_other.pData[i][j];
			}
		}
		return result;
	}

	/// <summary>
	/// Return the multiplied matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <param name="matrix">The matrix.</param>
	/// <returns>Result =>2 multiplied CMatrix</returns>
	CMatrix operator *(const CMatrix &CMatrix_other)
	{
		if (this->m_iCols != CMatrix_other.m_iRows)
		{
			// Subtraction can't be executed => check columns of 1st M and rows of 2nd M
			return *this;
		}
		CMatrix result(this->m_iRows, CMatrix_other.m_iCols);
		for (int i = 0; i < this->m_iRows; i++)
		{
			for (int j = 0; j < CMatrix_other.m_iCols; j++)
			{
				for (int k = 0; k < this->m_iCols; k++)
				{
					result.pData[i][j] += this->pData[i][k]
					* CMatrix_other.pData[k][j];
				}
			}
		}
		return result;
	}

	/// <summary>
	/// Return the multiplied matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <param name="matrix">The matrix.</param>
	/// <returns>Result =>number multiply to CMatrix</returns>
	CMatrix operator *(const double a)
	{
		CMatrix result(this->m_iRows, this->m_iCols);
		for (int i = 0; i < this->m_iRows; i++)
		{
			for (int j = 0; j < this->m_iCols; j++)
			{
				result.pData[i][j] = this->pData[i][j] * a;
			}
		}
		return result;
	}

	CMatrix& operator =(const CMatrix &CMatrix_other)
	{
		if (this->m_iRows != CMatrix_other.m_iRows || this->m_iCols != CMatrix_other.m_iCols)
		{
			/*std::cout
				<< "WARNING: Assignment is taking place with by changing the number of rows and columns of the matrix";*/
		}
		for (int i = 0; i < m_iRows; i++)
			delete[] pData[i];
		delete[] pData;
		m_iRows = m_iCols = 0;
		m_iRows = CMatrix_other.m_iRows;
		m_iCols = CMatrix_other.m_iCols;
		pData = new double*[m_iRows];
		for (int i = 0; i < m_iRows; i++)
			pData[i] = new double[m_iCols];
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				pData[i][j] = CMatrix_other.pData[i][j];
			}
		}
		return *this;
	}

	/// <summary>
	/// Return the 3W matrix <see cref="CMatrix"/> class.
	/// </summary>
	/// <param name="matrix">The p matrix.</param>
	/// <returns>Result =>number 3x1 3W matrix to CMatrix</returns>
	double** find3WMatrix (double P[4][1]){

		double vTransposed[3][4];
		double vMultiply[3][3];
		double vMultiply_Inverse[3][3];

		double cc[3][4];

		double** cc1;
		cc1 = new double*[4];
		for (int i = 0; i < 4; i++){
			cc1[i] = new double[1];
			cc1[i][0] = 0.0;
		}

		std::fill(vTransposed[0], vTransposed[0] + 3 * 4, 0);
		std::fill(vMultiply[0], vMultiply[0] + 3 * 3, 0);
		std::fill(vMultiply_Inverse[0], vMultiply_Inverse[0] + 3 * 3, 0);
		std::fill(cc[0], cc[0] + 3 * 4, 0);
		
		//====> VT[3,4] - Transpose Matrix 
		for (int i = 0; i < m_iRows; i++)
		{
			for (int j = 0; j < m_iCols; j++)
			{
				vTransposed[j][i] = this->pData[i][j];
			}
		} //<==== return vTransposed[3,4]
		
		//====> VT[3,4] * V[4,3] Matrix 
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					vMultiply[i][j] += vTransposed[i][k]
					* this->pData[k][j];
				}
			}
		} //<==== return vMultiply[3,3]

		// =====> Calculate Inverse for [3,3] matrix
		double det = vMultiply[0][0] * (vMultiply[1][1] * vMultiply[2][2] - vMultiply[2][1] * vMultiply[1][2]) -
			vMultiply[0][1] * (vMultiply[1][0] * vMultiply[2][2] - vMultiply[1][2] * vMultiply[2][0]) +
			vMultiply[0][2] * (vMultiply[1][0] * vMultiply[2][1] - vMultiply[1][1] * vMultiply[2][0]);

		double invdet = 1.0 / det;

		vMultiply_Inverse[0][0] = (vMultiply[1][1] * vMultiply[2][2] - vMultiply[2][1] * vMultiply[1][2]) * invdet;
		vMultiply_Inverse[0][1] = (vMultiply[0][2] * vMultiply[2][1] - vMultiply[0][1] * vMultiply[2][2]) * invdet;
		vMultiply_Inverse[0][2] = (vMultiply[0][1] * vMultiply[1][2] - vMultiply[0][2] * vMultiply[1][1]) * invdet;
		vMultiply_Inverse[1][0] = (vMultiply[1][2] * vMultiply[2][0] - vMultiply[1][0] * vMultiply[2][2]) * invdet;
		vMultiply_Inverse[1][1] = (vMultiply[0][0] * vMultiply[2][2] - vMultiply[0][2] * vMultiply[2][0]) * invdet;
		vMultiply_Inverse[1][2] = (vMultiply[1][0] * vMultiply[0][2] - vMultiply[0][0] * vMultiply[1][2]) * invdet;
		vMultiply_Inverse[2][0] = (vMultiply[1][0] * vMultiply[2][1] - vMultiply[2][0] * vMultiply[1][1]) * invdet;
		vMultiply_Inverse[2][1] = (vMultiply[2][0] * vMultiply[0][1] - vMultiply[0][0] * vMultiply[2][1]) * invdet;
		vMultiply_Inverse[2][2] = (vMultiply[0][0] * vMultiply[1][1] - vMultiply[1][0] * vMultiply[0][1]) * invdet;

		// <===== Calculate Inverse for [3,3] matrix

		//====> 1/(VTV)[3,3] * VT[3,4] Matrix 
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					cc[i][j] += vMultiply_Inverse[i][k]
					* vTransposed[k][j];
				}
			}
		} //<==== return cc[3,4]


		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 1; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					cc1[i][j] += cc[i][k]
					* P[k][j];
				}
			}
		}//<==== return [3,1] matrix
		cc1[3][0] = 1 -  cc1[0][0] - cc1[1][0] - cc1[2][0];

		return cc1;

		/*CMatrix CMatrix_vt = this->Transpose();
		CMatrix CMatrix_result = (CMatrix_vt*(*this)).Inverse()*CMatrix_vt*P;
		CMatrix CMatrix_w(4,1);
		double w4 = 1;
		for(int i = 0 ; i < 3; i++){
			w4 -=CMatrix_result.pData[i][0];
			CMatrix_w.pData[i][0] = CMatrix_result.pData[i][0];
		}
		CMatrix_w.pData[3][0]=w4;
		CMatrix res(CMatrix_w);
		return res;*/
	}
};