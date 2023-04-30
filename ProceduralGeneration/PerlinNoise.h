#pragma once
#include "PerlinGrid2d.h"
namespace ProceduralGeneration {


    template<typename Type>
    class PerlinNoise {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type.");

        using Vector2t = Tools::Vector2t<Type>;
        using Vector2i = Tools::Vector2t<int>;


    public:
        //  Definition of the gradient grid
        explicit PerlinNoise(int seed = 1337) : m_grid(seed), m_frequency(1.0f)
        {
        }

        inline Type compute(const Type& x, const Type& y) {
 
            return getPerlinNoise2d(x * m_frequency, y * m_frequency);
        }

        inline void setFrequency(const Type frequency) {
			m_frequency = frequency;
		}

    private:
        inline Type getPerlinNoise2d(const Type& x, const Type& y)
        {
            // recovery of the cell in which the point given in parameter is located
            int cellX0 = std::floor(x);
            int cellY0 = std::floor(y);
            int cellX1 = cellX0 + 1;
            int cellY1 = cellY0 + 1;

            // calculation of the distances between the corners and the point given in parameter
            Type distanceX0 = x - static_cast<Type>(cellX0);
            Type distanceY0 = y - static_cast<Type>(cellY0);
            Type distanceX1 = distanceX0 - 1;
            Type distanceY1 = distanceY0 - 1;

            //recovery of the gradients stored at each corner
            Type gradientX0Y0 = m_grid.getGradientDotProduct(cellX0, cellY0, distanceX0, distanceY0);
            Type gradientX1Y0 = m_grid.getGradientDotProduct(cellX1, cellY0, distanceX1, distanceY0);
            Type gradientX0Y1 = m_grid.getGradientDotProduct(cellX0, cellY1, distanceX0, distanceY1);
            Type gardientX1Y1 = m_grid.getGradientDotProduct(cellX1, cellY1, distanceX1, distanceY1);


            // Interpolation of the gradients to obtain the final value of the Perlin noise
            Type interpolationX0Y0_X1Y0 = interpolate(gradientX0Y0, gradientX1Y0, distanceX0);
            Type interpolationX0Y1_X1Y1 = interpolate(gradientX0Y1, gardientX1Y1, distanceX0);

            return interpolate(interpolationX0Y0_X1Y0, interpolationX0Y1_X1Y1, distanceY0);
        }


        // w is the difference between the first corner of the cell and the position of the point in the cell
        inline Type interpolate(Type a0, Type a1, Type w) const {
            return  (1 - w) * a0 + w * a1;
        }



        //attributes
        PerlinGrid2d<Type> m_grid;
        Type m_frequency;
    };

}
