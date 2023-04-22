#include "Grid3d.h"
#include <random>
#include <Tools/MathUtils.h>

namespace ProceduralGeneration {

	using Vector3f = Tools::Vector3<float>;



Grid3d::Grid3d(int width, int height, int depth) : m_width(width), m_height(height), m_depth(depth)
{
	m_cells = std::vector<Vector3f>(width * height * depth, randomGradientVector());
}

Grid3d::~Grid3d()
{
}

const int Grid3d::getWidth() const
{
	return m_width;
}

const int Grid3d::getHeight() const
{
	return m_height;
}

const int Grid3d::getDepth() const
{
	return m_depth;
}

const Vector3f& Grid3d::getCell(const int& x, const int& y, const int& z) const
{
	return m_cells[x + y * m_width + z * m_width * m_height];
}

void Grid3d::setCell(const int& x, const int& y, const int& z, Vector3f&& value)
{
	m_cells.emplace(m_cells.begin() + (x + y * m_width + z * m_width * m_height), std::move(value));
}


const float Grid3d::randomFloat() const
{
	std::mt19937_64 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	return dist(rng);
}

Vector3f Grid3d::randomGradientVector() const
{
	float x = randomFloat();
	float y = randomFloat();
	float z = randomFloat();

	float length = std::sqrt(x * x + y * y + z * z);

	return Vector3f(x, y, z) / length;
}




}
