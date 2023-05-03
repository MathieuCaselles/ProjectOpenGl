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
        explicit PerlinNoise(int seed = 666) : m_grid(seed), m_frequency(1), m_scale(1), m_amplitude(1), m_octaves(1), m_exponent(1), m_flatFloorLevel(0)
        {
        }

        inline const Type& compute(Type x, Type y) 
        {
            int seed = 666;
                Type noiseValue = 0.0;
                Type amplitude = 1;
                Type frequency = m_frequency;

                for (int i = 0; i < m_octaves; ++i)
                {
                    x = x / m_scale * frequency;
                    y = y / m_scale * frequency;
                    noiseValue = noiseValue + getPerlinNoise2d(x, y) * amplitude;
                    amplitude = amplitude / static_cast<Type>(2.f);
                    frequency = frequency * static_cast<Type>(2.f);
                }

                noiseValue = redistribution(noiseValue);
                noiseValue = applyFlatFloorLevel(noiseValue);

            return noiseValue * m_amplitude;
        }


        // setters
        inline void setAmplitude(const Type amplitude)
        {
            m_amplitude = amplitude;
        }

        inline void setFrequency(const Type frequency) 
        {
			m_frequency = frequency;
		}

        inline void setScale(const Type scale)
        {
			m_scale = scale;
		}

        inline void setOctaves(const int octaves)
        {
            m_octaves = octaves;
        }

        inline void setExponent(const Type exponent)
        {
            m_exponent = exponent;
        };

        inline void setSeed(const int seed)
        {
            m_grid.setSeed(seed);
        }

        inline const int& getSeed() const
        {
			return m_grid.getSeed();
		}

        inline void setFlatFloorLevel(const Type waterLevel)
        {
            m_flatFloorLevel = waterLevel;
        }

    private:
        inline const Type& getPerlinNoise2d(const Type& x, const Type& y)
        {
            // recovery of the cell in which the point given in parameter is located
            const int cellX0 = std::floor(x);
            const int cellY0 = std::floor(y);
            const int cellX1 = cellX0 + 1;
            const int cellY1 = cellY0 + 1;

            // calculation of the distances between the corners and the point given in parameter
            const Type distanceX0 = x - static_cast<Type>(cellX0);
            const Type distanceY0 = y - static_cast<Type>(cellY0);
            const Type distanceX1 = distanceX0 - 1;
            const Type distanceY1 = distanceY0 - 1;

            //recovery of the gradients stored at each corner
            const Type gradientX0Y0 = m_grid.getGradientDotProduct(cellX0, cellY0, distanceX0, distanceY0);
            const Type gradientX1Y0 = m_grid.getGradientDotProduct(cellX1, cellY0, distanceX1, distanceY0);
            const Type gradientX0Y1 = m_grid.getGradientDotProduct(cellX0, cellY1, distanceX0, distanceY1);
            const Type gardientX1Y1 = m_grid.getGradientDotProduct(cellX1, cellY1, distanceX1, distanceY1);


            // Interpolation of the gradients to obtain the final value of the Perlin noise
            const Type interpolationX0Y0_X1Y0 = interpolate(gradientX0Y0, gradientX1Y0, distanceX0);
            const Type interpolationX0Y1_X1Y1 = interpolate(gradientX0Y1, gardientX1Y1, distanceX0);

            const Type result = interpolate(interpolationX0Y0_X1Y0, interpolationX0Y1_X1Y1, distanceY0);

            // we convert the value from -1 to 1 to 0 to 1
            return (result + static_cast<Type>(1.0)) / static_cast<Type>(2.0);
        }


        // w is the difference between the first corner of the cell and the position of the point in the cell
        inline const Type interpolate(Type a0, Type a1, Type w) const {
            return  (1 - w) * a0 + w * a1;
        }

        // raise the elevation to a power
        inline const Type& redistribution(const Type& noiseValue) const
        {
            return std::pow(noiseValue, m_exponent);
        }

        inline const Type& applyFlatFloorLevel(const Type& noiseValue) const
        {

            return std::max(noiseValue, m_flatFloorLevel);
        }


        //attributes
        PerlinGrid2d<Type> m_grid;

        Type m_amplitude;
        Type m_frequency;
        Type m_scale;

        int m_octaves;

        Type m_exponent;

        Type m_flatFloorLevel;
    };

}
