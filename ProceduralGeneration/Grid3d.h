#pragma once
#include <vector>

namespace Tools {
	template<typename Type>
	struct Vector3;
}


namespace ProceduralGeneration{

using Vector3f = Tools::Vector3<float>;


class Grid3d
{
public:
	
	Grid3d(int width, int height, int depth);
	~Grid3d();

	const int getWidth() const;
	const int getHeight() const;
	const int getDepth() const;

	const Vector3f& getCell(const int& x, const int& y, const int& z) const;
	void setCell(const int& x, const int& y, const int& z, Vector3f&& value);


private:
	const float randomFloat() const;
	Vector3f randomGradientVector() const;


	int m_width;
	int m_height;
	int m_depth;

	std::vector<Vector3f> m_cells;
};



}