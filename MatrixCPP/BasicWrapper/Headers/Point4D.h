#pragma once

#include <math.h>

/// <summary>
/// 
/// </summary>
struct TPoint4D
{
public:
	/// <summary>
	/// The x
	/// </summary>
	int X;
	/// <summary>
	/// The y
	/// </summary>
	int Y;
	/// <summary>
	/// The z
	/// </summary>
	int Z;
	/// <summary>
	/// The w
	/// </summary>
	double W;

	/// <summary>
	/// Initializes a new instance of the <see cref="TPoint4D"/> struct.
	/// </summary>
	TPoint4D()
	{
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
		this->W = 0;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="TPoint4D"/> struct.
	/// </summary>
	/// <param name="point">The point.</param>
	TPoint4D(TPoint4D &point)
	{
		this->X = point.X;
		this->Y = point.Y;
		this->Z = point.Z;
		this->W = point.W;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="TPoint4D"/> struct.
	/// </summary>
	/// <param name="X">The x.</param>
	/// <param name="Y">The y.</param>
	/// <param name="Z">The z.</param>
	TPoint4D(const double& X, const double& Y, const double& Z, const double& W)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->W = W;
	}

	/// <summary>
	/// Distances from.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <returns></returns>
	double DistanceFrom(TPoint4D& point)
	{
		return sqrt((double)
					((this->X - point.X) * (this->X - point.X) + 
			         (this->Y - point.Y) * (this->Y - point.Y) + 
					 (this->Z - point.Z) * (this->Z - point.Z)));
	}

	/// <summary>
	/// Squareds the distance from.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <returns></returns>
	double SquaredDistanceFrom(TPoint4D& point)
	{
		return (this->X - point.X) * (this->X - point.X) + 
			   (this->Y - point.Y) * (this->Y - point.Y) + 
			   (this->Z - point.Z) * (this->Z - point.Z);
	}

	/// <summary>
	/// Substracts the specified point.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <returns></returns>
	TPoint4D Substract(TPoint4D& point)
	{
		return TPoint4D(
			point.X - this->X, 
			point.Y - this->Y, 
			point.Z - this->Z, 
			point.W - this->W);
	}

	/// <summary>
	/// Substracts the specified i distance.
	/// </summary>
	/// <param name="iDistance">The i distance.</param>
	/// <returns></returns>
	TPoint4D Substract(const int& iDistance)
	{
		return TPoint4D(
			this->X - iDistance, 
			this->Y - iDistance, 
			this->Z - iDistance, 
			this->W);
	}
};
