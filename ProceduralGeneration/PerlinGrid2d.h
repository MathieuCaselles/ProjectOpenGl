#pragma once
#include "Tools/MathUtils.h"
#include<vector>
#include <random>
#include <iostream>

namespace ProceduralGeneration {


template<typename Type>
class PerlinGrid2d {
	static_assert(std::is_floating_point_v<Type>, "Type must be an floating type.");

	using Vector2t = Tools::Vector2t<Type>;

public:
	PerlinGrid2d(const int seed)
	{
		setSeed(seed);
	};

	inline Type getGradientDotProduct(const int& x, const int& y, const Type& distanceX, const Type& distanceY) const
	{
		const Type gradientX[] =
		{
			1, -1, 1, -1,
			1, -1, 1, -1,
			0, 0, 0, 0
		};
		const Type gradientY[] =
		{
			1, 1, -1, -1,
			0, 0, 0, 0,
			1, -1, 1, -1
		};

		unsigned char permutedIndex = getPermutedIndex(x, y);

		return distanceX * gradientX[permutedIndex] + distanceY * gradientY[permutedIndex];
	}

    inline void setSeed(const int seed)
    {
        m_seed = seed;
        setPermutationTable();
    }

    inline const int& getSeed() const
    {
        return m_seed;
    }


private:
	inline void setPermutationTable()
	{
		std::mt19937_64 gen(m_seed);

		for (int i = 0; i < 256; ++i)
			m_permutationTable[i] = i;

		// Shuffle the permutation table using the Fisher-Yates algorithm
		for (int j = 0; j < 256; ++j)
		{
			// Generate a random number between 0 and (256-j-1) inclusive
			int rng = (int)(gen() % (256 - j));
			int k = rng + j;
			int l = m_permutationTable[j];
			m_permutationTable[j] = m_permutationTable[j + 256] = m_permutationTable[k];
			m_permutationTable[k] = l;
			m_permutationTableModulo12[j] = m_permutationTableModulo12[j + 256] = m_permutationTable[j] % 12;
		}
	}

	inline const unsigned char& getPermutedIndex(const int& x, const int& y) const
	{
		// "& 0xff" means that the number is "masked" by using the "and" operator with the hexadecimal value 0xff (which is 255 in decimal). 
		// This ensures that the number is between 0 and 255, as it removes all bits beyond the eighth bit.
		return m_permutationTableModulo12[(x & 0xff) + m_permutationTable[(y & 0xff)]];
	}


	int m_seed;

	unsigned char m_permutationTable[512];
	unsigned char m_permutationTableModulo12[512];
	
};




}