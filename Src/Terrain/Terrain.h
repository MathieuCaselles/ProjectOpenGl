#pragma once
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <vector>
#include <array>
#include <iostream>

#include "Tools/MathUtils.h"
#include "Shader.h"
#include "ProceduralGeneration/PerlinNoise.h"
#include "Texture.h"

constexpr float TERRAIN_STEP = 1;



template<typename Type>
struct vertex_struct_terrain
{
	vertex_struct_terrain(const Tools::Point3d<Type>& p_, const Tools::Point3d<Type>& n_, const Tools::Color<Type>& c_, const Tools::Point2d<Type>& t_) : p(p_), c(c_), n(n_), t(t_) {}
	Tools::Point3d<Type> p;
	Tools::Point3d<Type> n;

	Tools::Color<Type> c;

	Tools::Point2d<Type> t;
	int nb_face = 0;

};


template<typename Type>
class Terrain
{
	using vt = vertex_struct_terrain<Type>;

public:

	Terrain()
	{
		load();
	}

	~Terrain()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_elementbuffer);
	}

	void generateTerrainVerticesIndices(int size, float step)
	{
		const int numVertices = static_cast<int>(size / step) + 1;

		m_perlinNoise.setAmplitude(20);
		m_perlinNoise.setPersistance(0.5f);
		m_perlinNoise.setFrequency(6);
		m_perlinNoise.setLacunarity(0.8);
		m_perlinNoise.setScale(1);
		m_perlinNoise.setOctaves(4);
		m_perlinNoise.setExponent(3);
		m_perlinNoise.setFlatFloorLevel(0.2);

		const Type width = numVertices;
		const Type height = numVertices;

		std::vector<std::vector<Type>> heightmap(width, std::vector<Type>(height));

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const Type nx = static_cast<Type>(x) / static_cast<Type>(width) - static_cast<Type>(0.5f);
				const Type ny = static_cast<Type>(y) / static_cast<Type>(height) - static_cast<Type>(0.5f);
				heightmap[y][x] = m_perlinNoise.compute(nx, ny); // todo: no hardcord magic value to increase height of noise
			}
		}

		// Vertices
        m_vertexVect.clear();
		for (int i = 0; i < numVertices; ++i) {
			for (int j = 0; j < numVertices; ++j) {
				const Type x = i * step;
				const Type z = j * step;
				const Type y = heightmap[z][x];
				m_vertexVect.push_back(Tools::Point3d<Type>{x, y, z});

			}
		}

		// Indices
        m_indices.clear();
		for (int i = 0; i < numVertices - 1; ++i) {
			for (int j = 0; j < numVertices - 1; ++j) {

				const unsigned int index1 = i * numVertices + j;
				const unsigned int index2 = index1 + 1;
				const unsigned int index3 = (i + 1) * numVertices + j;
				const unsigned int index4 = index3 + 1;

				// Premier triangle
				m_indices.push_back(index1);
				m_indices.push_back(index2);
				m_indices.push_back(index3);

				// Deuxi�me triangle
				m_indices.push_back(index2);
				m_indices.push_back(index4);
				m_indices.push_back(index3);
			}
		}
		m_vertexVect.shrink_to_fit();
		m_indices.shrink_to_fit();
	}

	void setSeed(int newSeed) {
        if (m_perlinNoise.getSeed() != newSeed) {
            m_perlinNoise.setSeed(newSeed);
            m_shouldReload = true;
        }
	};

	void setFrequency(Type newFrequency) {
        if (m_perlinNoise.getFrequency() != newFrequency) {
            m_perlinNoise.setFrequency(newFrequency);
            m_shouldReload = true;
        }
	};

	void setAmplitude(Type newAmplitude) {
        if (m_perlinNoise.getAmplitude() != newAmplitude) {
            m_perlinNoise.setAmplitude(newAmplitude);
            m_shouldReload = true;
        }
	};

	void setOctave(int newOctave) {
        if (m_perlinNoise.getOctaves() != newOctave) {
            m_perlinNoise.setOctaves(newOctave);
            m_shouldReload = true;
        }
	};

	void setExponent(Type newExponent) {
        if (m_perlinNoise.getExponent() != newExponent) {
            m_perlinNoise.setExponent(newExponent);
            m_shouldReload = true;
        }
	};

	void showPolygon() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void showPoint() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	void showFill() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void setSnowHeight(float snowHeight) {
		m_snowHeight = snowHeight;
	}

	void setStoneAngle(float stoneAngle) {
		m_stoneAngle = stoneAngle;
	}

	void setTerrainSize(int terrainSize) {

		m_terrainSize = terrainSize;

		m_vertexVect.clear();
		points.clear();
		m_indices.clear();

		load();
	};

	void setWaterHeight(float waterHeight) {
		m_waterHeight = waterHeight;
	}

	void reloadHeight() {
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		const int numVertices = static_cast<int>(m_terrainSize / TERRAIN_STEP) + 1;

		const Type width = numVertices;
		const Type height = numVertices;

		std::vector<std::vector<Type>> heightmap(width, std::vector<Type>(height));

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const Type nx = static_cast<Type>(x) / static_cast<Type>(width) - static_cast<Type>(0.5f);
				const Type ny = static_cast<Type>(y) / static_cast<Type>(height) - static_cast<Type>(0.5f);
				heightmap[y][x] = m_perlinNoise.compute(nx, ny); // todo: no hardcord magic value to increase height of noise
			}
		}

		for (vertex_struct_terrain<Type>& p : points) {

			p.p.y = heightmap[p.p.z][p.p.x] - 15;
		}

		for (int i = 2; i < m_indices.size(); i += 3) {

			int index = m_indices.at(i);
			vt& p3 = points.at(m_indices.at(i));
			vt& p2 = points.at(m_indices.at(i - 1));
			vt& p1 = points.at(m_indices.at(i - 2));

			Tools::Point3d<Type> vec12 = { p2.p.x - p1.p.x, p2.p.y - p1.p.y, p2.p.z - p1.p.z };
			Tools::Point3d<Type> vec13 = { p3.p.x - p1.p.x, p3.p.y - p1.p.y, p3.p.z - p1.p.z };

			Tools::Point3d<Type> normal =
			{
				(vec12.z * vec13.y) - (vec12.y * vec13.z),
				(vec12.x * vec13.z) - (vec12.z * vec13.x),
				(vec12.y * vec13.x) - (vec12.x * vec13.y)
			};

			p1.n = ((p1.n * p1.nb_face) + normal) / (p1.nb_face);
			p1.n = p1.n / std::sqrt((p1.n.x * p1.n.x) + (p1.n.y * p1.n.y) + (p1.n.z * p1.n.z));

			p1.nb_face += 1;

			p2.n = ((p2.n * p2.nb_face) + normal) / (p2.nb_face);
			p2.n = p2.n / std::sqrt((p2.n.x * p2.n.x) + (p2.n.y * p2.n.y) + (p2.n.z * p2.n.z));

			p2.nb_face += 1;

			p3.n = ((p3.n * p3.nb_face) + normal) / (p3.nb_face);
			p3.n = p3.n / std::sqrt((p3.n.x * p3.n.x) + (p3.n.y * p3.n.y) + (p3.n.z * p3.n.z));

			p3.nb_face += 1;

		}

		glUseProgram(m_program);
		glBufferData(GL_ARRAY_BUFFER, m_nbVertices * sizeof(vertex_struct_terrain<Type>), points.data(), GL_STATIC_DRAW);
	}

	void load()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		Tools::Color<Type> vg = { 0, 1, 0, 1 };
		Tools::Point3d<Type> nyp = { 0, +1, 0 };
		Tools::Point2d<Type> texCoord;

		generateTerrainVerticesIndices(m_terrainSize, 1);
		

        points.clear();
		for (Tools::Point3d<float>& p : m_vertexVect)
		{
			points.push_back(vt{ p, nyp, vg , texCoord });
		}

		points.shrink_to_fit();

		m_nbVertices = static_cast<GLsizei>(points.size());

		const float squareSize = 1.0f / (m_nbVertices / 3);

		for (vertex_struct_terrain<Type>& p : points) {

			float xSquare = (p.p.x + 0.5f) / squareSize;
			float zSquare = (p.p.z + 0.5f) / squareSize;
			p.t = Tools::Point2d<Type>{ Type(xSquare * squareSize), Type(zSquare * squareSize) };
		}


		for (int i = 2; i < m_indices.size(); i += 3) {

			int index = m_indices.at(i);
			vt& p3 = points.at(m_indices.at(i));
			vt& p2 = points.at(m_indices.at(i - 1));
			vt& p1 = points.at(m_indices.at(i - 2));

			Tools::Point3d<Type> vec12 = { p2.p.x - p1.p.x, p2.p.y - p1.p.y, p2.p.z - p1.p.z };
			Tools::Point3d<Type> vec13 = { p3.p.x - p1.p.x, p3.p.y - p1.p.y, p3.p.z - p1.p.z };

			//Calcul de la normal
			Tools::Point3d<Type> normal =
			{
				(vec12.z * vec13.y) - (vec12.y * vec13.z),
				(vec12.x * vec13.z) - (vec12.z * vec13.x),
				(vec12.y * vec13.x) - (vec12.x * vec13.y)
			};

			p1.n = ((p1.n * p1.nb_face) + normal) / (p1.nb_face);
			p1.n = p1.n / std::sqrt((p1.n.x * p1.n.x) + (p1.n.y * p1.n.y) + (p1.n.z * p1.n.z));

			p1.nb_face += 1;

			p2.n = ((p2.n * p2.nb_face) + normal) / (p2.nb_face);
			p2.n = p2.n / std::sqrt((p2.n.x * p2.n.x) + (p2.n.y * p2.n.y) + (p2.n.z * p2.n.z));

			p2.nb_face += 1;

			p3.n = ((p3.n * p3.nb_face) + normal) / (p3.nb_face);
			p3.n = p3.n / std::sqrt((p3.n.x * p3.n.x) + (p3.n.y * p3.n.y) + (p3.n.z * p3.n.z));

			p3.nb_face += 1;

		}

		glBufferData(GL_ARRAY_BUFFER, m_nbVertices * sizeof(vertex_struct_terrain<Type>), points.data(), GL_STATIC_DRAW);

		ShaderInfo shaders[] = {
			{GL_VERTEX_SHADER, "Assets/OpenGl/terrain.vert"},
			{GL_FRAGMENT_SHADER, "Assets/OpenGl/terrain.frag"},
			{GL_NONE, nullptr}
		};

		m_program = Shader::loadShaders(shaders);
		glUseProgram(m_program);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), (char*)(0) + sizeof(vertex_struct_terrain<Type>::p));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), (char*)(0)
			+ sizeof(vertex_struct_terrain<Type>::p)
			+ sizeof(vertex_struct_terrain<Type>::n));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), (char*)(0)
			+ sizeof(vertex_struct_terrain<Type>::p)
			+ sizeof(vertex_struct_terrain<Type>::n)
			+ sizeof(vertex_struct_terrain<Type>::c));
		glEnableVertexAttribArray(3);


		glGenBuffers(1, &m_elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);


	}

	void render(const Tools::Mat4<Type>& View, const Tools::Mat4<Type>& Projection)
	{
		glUseProgram(m_program);

		glBindVertexArray(m_vao);

		Tools::Mat4<Type> Model = Tools::Mat4<Type>::translation(0, 0, -5) * Tools::Mat4<Type>::rotationY(m_angleY) * Tools::Mat4<Type>::rotationX(m_angleX);

		glUniformMatrix4fv(glGetUniformLocation(m_program, "ModelMatrix"), 1, GL_FALSE, Model.getData());
		glUniformMatrix4fv(glGetUniformLocation(m_program, "ViewMatrix"), 1, GL_FALSE, View.getData());
		glUniformMatrix4fv(glGetUniformLocation(m_program, "ProjectionMatrix"), 1, GL_FALSE, Projection.getData());

		glUniform1f(glGetUniformLocation(m_program, "material.ambient"), 0.2f);
		glUniform1f(glGetUniformLocation(m_program, "material.diffuse"), 0.7f);
		glUniform1f(glGetUniformLocation(m_program, "material.specular"), 1.f);
		glUniform1f(glGetUniformLocation(m_program, "material.specularSmoothness"), 1.0f);

		glUniform3f(glGetUniformLocation(m_program, "light.direction"), 0.5f, 0.5f, 0.f);
		glUniform3f(glGetUniformLocation(m_program, "light.color"), 1.f, 1.f, 1.f);

		glUniform3f(glGetUniformLocation(m_program, "camera.worldPosition"), 0.f, 0.f, 0.f);

		glUniform1f(glGetUniformLocation(m_program, "snowHeight"), m_snowHeight);
		glUniform1f(glGetUniformLocation(m_program, "stoneAngle"), m_stoneAngle);
		glUniform1f(glGetUniformLocation(m_program, "waterHeight"), m_waterHeight);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		glActiveTexture(GL_TEXTURE0);
		m_textureGrass.bind();

		glActiveTexture(GL_TEXTURE1);
		m_textureSnow.bind();

		glActiveTexture(GL_TEXTURE2);
		m_textureStone.bind();

		glActiveTexture(GL_TEXTURE3);
		m_textureSand.bind();

		GLint texture1Loc = glGetUniformLocation(m_program, "textureGrass");
		GLint texture2Loc = glGetUniformLocation(m_program, "textureSnow");
		GLint texture3Loc = glGetUniformLocation(m_program, "textureStone");
		GLint texture4Loc = glGetUniformLocation(m_program, "textureSand");
		glUniform1i(texture1Loc, 0);
		glUniform1i(texture2Loc, 1);
		glUniform1i(texture3Loc, 2);
		glUniform1i(texture4Loc, 3);

		glDrawElements(
			GL_TRIANGLES,      // mode
			m_indices.size(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);
	}

	void update()
	{
        if (m_shouldReload) {
            reloadHeight();
            m_shouldReload = false;
        }
	}

private:
	Type m_angleX = 0;
	Type m_angleY = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_program;
	GLsizei m_nbVertices;

	int m_terrainSize = 1000;

	float m_snowHeight = 30;
	float m_stoneAngle = 60;
	float m_waterHeight = 8;

	GLuint m_elementbuffer;
	std::vector<Tools::Point3d<Type>> m_vertexVect;
	std::vector<vertex_struct_terrain<Type>> points;
	std::vector<unsigned int> m_indices;

	ProceduralGeneration::PerlinNoise<Type> m_perlinNoise;

	Texture m_textureGrass = Texture("Assets/Textures/grass.png");
	Texture m_textureSnow = Texture("Assets/Textures/snow.png");
	Texture m_textureStone = Texture("Assets/Textures/stone.png");
	Texture m_textureSand = Texture("Assets/Textures/sand.png");

	//Texture m_textureStone = Texture("C:/Users/Thomas/Desktop/testTexture.png");


    bool m_shouldReload = false;
};
