#pragma once
#include "Grid2d.h"
namespace ProceduralGeneration {


    template<typename Type>
    class PerlinNoise {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type.");

        using Vector2t = Tools::Vector2t<Type>;
        using Vector2i = Tools::Vector2t<int>;


    public:
        //  Definition of the gradient grid
        explicit PerlinNoise(int gridWidth, int gridheight): m_grid(gridWidth, gridheight) 
        {
        }

        inline Type compute(const Type& x, const Type& y) const {
            // Find the cell in which the given point is located
            const int cellX = static_cast<int>(x);
            const int cellY = static_cast<int>(y);


            // retrieves the decimal part of x and y
            const float decimalX = x - cellX;
            const float decimalY = y - cellY;

            // Compute the distance vectors between the given point and each vertex of the cell
            const Vector2t gradientTopLeft = m_grid.getCell(cellX, cellY);
            const Vector2t gradientTopRight = m_grid.getCell(cellX + 1, cellY);
            const Vector2t gradientBottomLeft = m_grid.getCell(cellX, cellY + 1);
            const Vector2t gradientBottomRight = m_grid.getCell(cellX + 1, cellY + 1);

            // Interpolate Perlin noise values between neighboring cells
            const Type interpolatedTop = interpolate(gradientTopLeft.dotProduct(Vector2t(decimalX, decimalY)),
                gradientTopRight.dotProduct(Vector2t(decimalX - 1, decimalY)),
                decimalX);
            const Type interpolatedBottom = interpolate(gradientBottomLeft.dotProduct(Vector2t(decimalX, decimalY - 1)),
                gradientBottomRight.dotProduct(Vector2t(decimalX - 1, decimalY - 1)),
                decimalX);

            return interpolate(interpolatedTop, interpolatedBottom, decimalY);
        }

    private:


        // w is the difference between the first corner of the cell and the position of the point in the cell
        inline Type interpolate(Type a0, Type a1, Type w) const {
            return (1 - w) * a0 + w * a1;
        }


        Grid2d<Type> m_grid;
    };

}
