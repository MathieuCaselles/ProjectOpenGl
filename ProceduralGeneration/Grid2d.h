#pragma once
#include "Tools/MathUtils.h"
#include<vector>
#include <random>
#include <iostream>

namespace ProceduralGeneration {


template<typename Type>
class Grid2d {
	static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type.");

	using Vector2t = Tools::Vector2t<Type>;

public:
		Grid2d(int width, int height)
			: m_width(width)
			, m_height(height)
			, m_cells(width * height)
			, m_rng(std::random_device()())
			, m_dist(-1.0, 1.0)
		{
			// Definition of the gradient grid
			for (int y = 0; y < m_height; ++y)
				for (int x = 0; x < m_width; ++x)
					setCell(x, y, generateRandomGradientVector());
		}


		//getter and setters
		inline const Vector2t& getCell(int x, int y) const {
			return m_cells[x + y * m_width];
		}

		inline void setCell(const int& x, const int& y, Vector2t&& value)
		{
			const auto index = x + y * m_width;
			m_cells.emplace(m_cells.begin() + index, std::move(value));
		}

		inline const int getWidth() const {
			return m_width;
		}

		inline const int getHeight() const {
			return m_height;
		}


		//other methods
		inline Type getRandomNumber() {
			return m_dist(m_rng);
		}

		inline Vector2t generateRandomGradientVector() {
			Vector2t result(getRandomNumber(), getRandomNumber());
			result.normalize();

			return result;
		}

private:
	int m_width;
	int m_height;
	std::vector<Vector2t> m_cells;

	std::mt19937_64 m_rng;
	std::uniform_real_distribution<Type> m_dist;
	
};




}