#pragma once
#include "PerlinGrid2d.h"
namespace ProceduralGeneration {


    template<typename Type>
    class PerlinNoise {

        using HeightMapType = std::vector<std::vector<Type>>;

        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic type.");

        using Vector2t = Tools::Vector2t<Type>;
        using Vector2i = Tools::Vector2t<int>;


    public:
        //  Definition of the gradient grid
        explicit PerlinNoise(int seed = 666) 
            : m_grid(seed)
            , m_amplitude(1)
            , m_persistance(0.5f)
            , m_frequency(1)
            , m_lacunarity(1.5f)
            , m_scale(1)
            , m_octaves(1)
            , m_exponent(1)
            , m_flatFloorLevel(0)
            , m_lakeLevel(10)
            , m_gen(seed)
            , m_probabilityOfALakeAppearing(0.0001f)
            , m_maximumLakeRadius(80)
            , m_minimumLakeRadius(10)
            , m_maximumLakeDepth(20)
            , m_minimumLakeDepth(5)
        {
        }

        inline const Type computeNoise(Type x, Type y)
        {
            Type noiseValue = 0.0;
            Type amplitude = 1;
            Type frequency = m_frequency;

            for (int i = 0; i < m_octaves; ++i)
            {
                x = x / m_scale * frequency;
                y = y / m_scale * frequency;
                noiseValue += getPerlinNoise2d(x, y) * amplitude;
                amplitude *= m_persistance;
                frequency *= m_lacunarity;
            }

            noiseValue = redistribution(noiseValue);
            noiseValue = applyFlatFloorLevel(noiseValue);

            return noiseValue * m_amplitude;
        }

        inline const HeightMapType computeHeightmap(const int& width, const int& height, const bool& includeLakes = true)
        {
            HeightMapType heightmap(width, std::vector<Type>(height));

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    const Type nx = static_cast<Type>(x) / static_cast<Type>(width) - static_cast<Type>(0.5f);
                    const Type ny = static_cast<Type>(y) / static_cast<Type>(height) - static_cast<Type>(0.5f);
                    heightmap[y][x] = computeNoise(nx, ny); // todo: no hardcord magic value to increase height of noise
                }
            }

            if (includeLakes)
                addLakesInHeightmap(heightmap, width, height);

            return heightmap;
        }

        inline const HeightMapType& addLakesInHeightmap(HeightMapType& heightmap, const int& width, const int& height)
        {
            std::uniform_int_distribution<int> disLakeRadius(m_minimumLakeRadius, m_maximumLakeRadius);
            std::uniform_int_distribution<int> disLakeDepth(m_minimumLakeDepth, m_maximumLakeDepth);
            std::uniform_real_distribution<float> disLakeProbability(0.0f, 1.0f);

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    const Type currentHeight = heightmap[y][x];
                    if (currentHeight <= m_lakeLevel) {

                        if (disLakeProbability(m_gen) < m_probabilityOfALakeAppearing) {
                            const Type lakeRadius = disLakeRadius(m_gen);
                            const Type lakeDepth = disLakeDepth(m_gen);
                            const Type lakeCenterDepth = currentHeight + lakeDepth;

                            for (int dy = -lakeRadius; dy <= lakeRadius; ++dy) {
                                for (int dx = -lakeRadius; dx <= lakeRadius; ++dx) {
                                    const int nx = x + dx;
                                    const int ny = y + dy;
                                    const Type noiseWater = computeNoise(nx, ny);

                                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;

                                    const Type distance = std::sqrt(dx * dx + dy * dy);
                                    if (distance <= lakeRadius)
                                        heightmap[ny][nx] -= lakeCenterDepth * (1 - distance / lakeRadius);
                                }
                            }


                        }
                    }
                }
            }

            return heightmap;
        }


        // setters
        inline void setAmplitude(const Type amplitude)
        {
            m_amplitude = amplitude;
        }

        inline void setPersistance(const Type persistance)
        {
			m_persistance = persistance;
		}

        inline void setFrequency(const Type frequency) 
        {
			m_frequency = frequency;
		}

        inline void setLacunarity(const Type lacunarity)
        {
            m_lacunarity = lacunarity;
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
            m_gen.seed(seed);
            m_grid.setSeed(seed);
        }

        inline void setFlatFloorLevel(const Type waterLevel)
        {
            m_flatFloorLevel = waterLevel;
        }

        // getters
        inline const int& getSeed() const
        {
            return m_grid.getSeed();
        }

        inline const Type& getAmplitude() const
        {
            return m_amplitude;
        }

        inline const Type& getPersistance() const
        {
            return m_persistance;
        }

        inline const Type& getFrequency() const
        {
            return m_frequency;
        }

        inline const Type& getLacunarity() const
        {
            return m_lacunarity;
        }

        inline const Type& getScale() const
        {
            return m_scale;
        }

        inline const int& getOctaves() const
        {
            return m_octaves;
        }

        inline const Type& getExponent() const
        {
            return m_exponent;
        };

        inline const Type& getFlatFloorLevel() const
        {
            return m_flatFloorLevel;
        }

    private:
        inline Type getPerlinNoise2d(const Type& x, const Type& y)
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
        inline Type redistribution(const Type& noiseValue) const
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
        Type m_persistance;

        Type m_frequency;
        Type m_lacunarity;

        Type m_scale;

        int m_octaves;

        Type m_exponent;

        Type m_flatFloorLevel;
        Type m_lakeLevel;

        float m_probabilityOfALakeAppearing;
        int m_maximumLakeRadius;
        int m_minimumLakeRadius;
        int m_maximumLakeDepth;
        int m_minimumLakeDepth;

        std::vector<std::vector<Type>> m_heightmap;

        std::mt19937_64 m_gen;


    };

}
