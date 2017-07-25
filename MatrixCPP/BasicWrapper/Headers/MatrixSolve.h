#include "stdafx.h"
#include <iostream>
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
			std::cout
				<< "WARNING: Assignment is taking place with by changing the number of rows and columns of the matrix";
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
	CMatrix find3WMatrix (const CMatrix &a){
		CMatrix CMatrix_vt = this->Transpose();
		CMatrix CMatrix_result = (CMatrix_vt*(*this)).Inverse()*CMatrix_vt*a;
		CMatrix CMatrix_w(4,1);
		double w4 = 1;
		for(int i = 0 ; i < 3; i++){
			w4 -=CMatrix_result.pData[i][0];
			CMatrix_w.pData[i][0] = CMatrix_result.pData[i][0];
		}
		CMatrix_w.pData[3][0]=w4;
		return CMatrix_w;
	}

	friend std::istream& operator >> (std::istream &is, CMatrix &m);
	friend std::ostream& operator <<(std::ostream &os, const CMatrix &m);
};

std::istream& operator >> (std::istream &is, CMatrix &m)
{
	std::cout << "\n\nRows: "
		<< m.m_iRows << " Cols: " << m.m_iCols << "\n";
	for (int i = 0; i < m.m_iRows; i++)
	{
		for (int j = 0; j < m.m_iCols; j++)
		{
			std::cout << "Input For Row: " << i + 1 << " Col: " << j + 1
				<< " = ";
			is >> m.pData[i][j];
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	return is;
}

std::ostream& operator << (std::ostream &os, const CMatrix &m)
{

	os << "\n\nRows: " << m.m_iRows << " Cols: "
		<< m.m_iCols << "\n\n";
	for (int i = 0; i < m.m_iRows; i++)
	{
		os << " | ";
		for (int j = 0; j < m.m_iCols; j++)
		{
			char buf[32];
			double data = m.pData[i][j];
			if (m.pData[i][j] > -0.00001 && m.pData[i][j] < 0.00001)
				data = 0;
			sprintf(buf, "%10.4lf ", data);
			os << buf;
		}
		os << "|\n";
	}
	os << "\n\n";
	return os;
};