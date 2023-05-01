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

constexpr float WATER_SIZE = 1000;


template<typename Type>
struct vertex_struct_water
{
	vertex_struct_water(const Tools::Point3d<Type>& p_, const Tools::Point3d<Type>& n_, const Tools::Color<Type>& c_, const Tools::Point2d<Type>& t_) : p(p_), c(c_), n(n_), t(t_) {}
	Tools::Point3d<Type> p;
	Tools::Point3d<Type> n;

	Tools::Color<Type> c;

	Tools::Point2d<Type> t;
	int nb_face = 0;

};

template<typename Type>
class Water
{
public:

	Water()
	{
		load();
	}

	~Water()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_elementbuffer);
	}

	void generateWaterVerticesIndices(int size, float step)
	{
		const int numVertices = static_cast<int>(size / step) + 1;

		m_perlinNoise.setFrequency(5);
		m_perlinNoise.setAmplitude(18);
		m_perlinNoise.setOctaves(4);
		m_perlinNoise.setExponent(3.5);

		const Type width = numVertices;
		const Type height = numVertices;

		std::vector<std::vector<Type>> heightmap(width, std::vector<Type>(height));

		// Vertices
		for (int i = 0; i < numVertices; ++i) {
			for (int j = 0; j < numVertices; ++j) {
				const Type x = i * step;
				const Type z = j * step;
				const Type y = 2;
				m_vertexVect.push_back(Tools::Point3d<Type>{x, y, z});

			}
		}

		// Indices
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

				// Deuxieme triangle
				m_indices.push_back(index2);
				m_indices.push_back(index4);
				m_indices.push_back(index3);
			}
		}
	}

	void load()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


		Tools::Color<Type> vb = { 0, 0, 1, 1 };
		Tools::Point3d<Type> nyn = { 0, -1, 0 };
		
		using vt = vertex_struct_water<Type>;
		std::vector<vertex_struct_water<Type>> points;

		generateWaterVerticesIndices(WATER_SIZE, WATER_SIZE);

		Tools::Point2d<Type> test{ 0, 0 };

		for (Tools::Point3d<float>& p : m_vertexVect)
		{
			points.push_back(vt{ p, nyn, vb , test });
		}

		m_nbVertices = static_cast<GLsizei>(points.size());

		const float squareSize = 1.0f / (m_nbVertices / 3);

		for (vertex_struct_water<Type>& p : points) {
			int xSquare = static_cast<int>((p.p.x + 0.5f) / squareSize);
			int zSquare = static_cast<int>((p.p.z + 0.5f) / squareSize);
			p.t = { xSquare * squareSize, zSquare * squareSize };
		}

		glBufferData(GL_ARRAY_BUFFER, m_nbVertices * sizeof(vertex_struct_water<Type>), points.data(), GL_STATIC_DRAW);

		ShaderInfo shaders[] = {
			{GL_VERTEX_SHADER, "Assets/OpenGl/water.vert"},
			{GL_FRAGMENT_SHADER, "Assets/OpenGl/water.frag"},
			{GL_NONE, nullptr}
		};

		m_program = Shader::loadShaders(shaders);
		glUseProgram(m_program);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_water<Type>), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_water<Type>), (char*)(0) + sizeof(vertex_struct_water<Type>::p));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_water<Type>), (char*)(0)
			+ sizeof(vertex_struct_water<Type>::p)
			+ sizeof(vertex_struct_water<Type>::n));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_water<Type>), (char*)(0)
			+ sizeof(vertex_struct_water<Type>::p)
			+ sizeof(vertex_struct_water<Type>::n)
			+ sizeof(vertex_struct_water<Type>::c));
		glEnableVertexAttribArray(3);


		glGenBuffers(1, &m_elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);


	}

	void render(const Tools::Mat4<Type>& View, const Tools::Mat4<Type>& Projection)
	{
		glUseProgram(m_program);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		glActiveTexture(GL_TEXTURE0);
		m_textureWater.bind();

		GLint texture1Loc = glGetUniformLocation(m_program, "textureWater");
		glUniform1i(texture1Loc, 0);


		glDrawElements(
			GL_TRIANGLES,      // mode
			m_indices.size(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);
	}

	void update()
	{
		/*m_angleX += 0.0125f;
		m_angleY += 0.025f;*/
	}

private:
	Type m_angleX = 0;
	Type m_angleY = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_program = 0;
	GLsizei m_nbVertices;

	GLuint m_elementbuffer;
	std::vector<Tools::Point3d<Type>> m_vertexVect;
	std::vector<unsigned int> m_indices;

	ProceduralGeneration::PerlinNoise<Type> m_perlinNoise;

	Texture m_textureWater = Texture("Assets/Textures/water.png");



};