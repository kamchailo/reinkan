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
		return (1 - t) * begin + t * end;
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
		float dot = Dot(begin, end);
		float radian;

		return (std::sin(radian - t * radian) / sin(radian)) * begin + (std::sin(t * radian) / sin(radian)) * end;
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
		return std::pow(end / begin) * begin;
	}
}