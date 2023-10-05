#pragma once

#include <cmath>

namespace Reinkan::Math
{
	/// <summary>
	/// Lerp Template Function
	/// </summary>
	/// <typeparam name="T"> : requires operator* (float lhs)</typeparam>
	/// <param name="begin"> : value at t = 0</param>
	/// <param name="end"> : value at t = 11</param>
	/// <param name="t"> : [0:1] describe the value between</param>
	/// <returns> Lerped value</returns>
	template<typename T>
	static T Lerp(const T& begin, const T& end, float t)
	{
		return  begin * (1 - t) + end * t;
	}

	/// <summary>
	/// SLerp Template Function
	/// </summary>
	/// <typeparam name="T"> : requires operator* (float lhs)</typeparam>
	/// <param name="begin"> : value at t = 0</param>
	/// <param name="end"> : value at t = 1</param>
	/// <param name="t"> : [0:1] describe the value between</param>
	/// <returns> SLerped value</returns>
	template<typename T>
	static T Slerp(const T& begin, const T& end, float t)
	{
		T temp = end;

		float dot = T::Dot(begin, end);
		float radian;

		if (dot < 0)
		{
			dot = dot;
			temp = -end;
		}

		if (dot > 0.95f)
		{
			return Lerp(begin, temp, t);
		}
		else
		{
			radian = std::acosf(dot);
			return (begin * std::sinf(radian * (1 - t)) + temp * std::sinf(radian * t)) / std::sinf(radian);
		}
	}

	/// <summary>
	/// ELerp Template Function
	/// (Usually used to interpolate scaling)
	/// </summary>
	/// <typeparam name="T"> : requires operator/ </typeparam>
	/// <param name="begin"> : value at t = 0</param>
	/// <param name="end"> : value at t = 1</param>
	/// <param name="t"> : </param>
	/// <returns> ELerped value</returns>
	template<typename T>
	static T Elerp(const T& begin, const T& end, float t)
	{
		return std::pow(end / begin, t) * begin;
	}
}