#pragma once
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <vector>
#include <array>
#include <iostream>

#include "Tools/MathUtils.h"
#include "Shader.h"
#include "ProceduralGeneration/PerlinNoise.h"

constexpr float TERRAIN_SIZE = 20;

namespace Engine {
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

	struct Texture
	{
		Texture()
		{
			//std::string path = "C:/Users/Thomas/Desktop/grass.png";
			std::string path = "C:/Users/Thomas/Desktop/grass.png";

			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D, m_texture);

			sf::Image image;
			image.loadFromFile(path);

			auto size = image.getSize();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		void bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		GLuint m_texture;
	};


	template<typename Type>
	class Terrain
	{
	public:

		Terrain()
		{
			load();
		}

		~Terrain()
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
		}

		//void GenerateVertex();
		//void GenerateIndices();

		void generateTerrainVerticesIndices(int size, float step)
		{
			int numVertices = static_cast<int>(size / step) + 1;

			m_perlinNoise.setFrequency(7);

			const Type width = numVertices;
			const Type height = numVertices;

			std::vector<std::vector<Type>> heightmap(width, std::vector<Type>(height));

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					Type nx = static_cast<Type>(x) / static_cast<Type>(width) - static_cast<Type>(0.5f);
					Type ny = static_cast<Type>(y) / static_cast<Type>(height) - static_cast<Type>(0.5f);
					heightmap[y][x] = m_perlinNoise.compute(nx, ny) * 10; // todo: no hardcord magic value to increase height of noise
				}
			}


			for (int i = 0; i < numVertices; ++i) {
				for (int j = 0; j < numVertices; ++j) {
					Type x = i * step;
					Type z = j * step;
					Type y = heightmap[z][x];
					m_vertexVect.push_back(Tools::Point3d<Type>{x, y, z});

				}
			}

			// Générer les indices pour les triangles
			for (int i = 0; i < numVertices - 1; ++i) {
				for (int j = 0; j < numVertices - 1; ++j) {
					// Indices des sommets des deux triangles formant un carré
					unsigned int index1 = i * numVertices + j;
					unsigned int index2 = index1 + 1;
					unsigned int index3 = (i + 1) * numVertices + j;
					unsigned int index4 = index3 + 1;

					// Premier triangle
					m_indices.push_back(index1);
					m_indices.push_back(index2);
					m_indices.push_back(index3);

					// Deuxième triangle
					m_indices.push_back(index2);
					m_indices.push_back(index4);
					m_indices.push_back(index3);
				}
			}
		}

		void load()
		{
			// We want only one buffer with the id generated and stored in m_vao
			glGenVertexArrays(1, &m_vao);

			// create a new active VAO
			glBindVertexArray(m_vao);

			// we want only one buffer with the id generated and stored in m_vbo
			glGenBuffers(1, &m_vbo);

			// 1. create a new active VBO if doesn’t exist
			// 2. if the VBO is active it will make it active
			// 3. if binded to 0, OpenGL stops
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			Tools::Point3d<Type> P000 = { -1, -1, -1 }; Tools::Point3d<Type> P001 = { -1, -1, +1 }; Tools::Point3d<Type> P010 = { -1, +1, -1 }; Tools::Point3d<Type> P011 = { -1, +1, +1 };
			Tools::Point3d<Type> P100 = { +1, -1, -1 }; Tools::Point3d<Type> P101 = { +1, -1, +1 }; Tools::Point3d<Type> P110 = { +1, +1, -1 }; Tools::Point3d<Type> P111 = { +1, +1, +1 };

			Tools::Color<Type> vr = { 1, 0, 0, 1 }; Tools::Color<Type> vg = { 0, 1, 0, 1 }; Tools::Color<Type> vb = { 0, 0, 1, 1 };
			Tools::Color<Type> ar = { 0, 1, 1, 1 }; Tools::Color<Type> ag = { 1, 0, 1, 1 }; Tools::Color<Type> ab = { 1, 1, 0, 1 };

			Tools::Point3d<Type> nxn = { -1, 0, 0 }; Tools::Point3d<Type> nxp = { +1, 0, 0 };
			Tools::Point3d<Type> nyn = { 0, -1, 0 }; Tools::Point3d<Type> nyp = { 0, +1, 0 };
			Tools::Point3d<Type> nzn = { 0, 0, -1 }; Tools::Point3d<Type> nzp = { 0, 0, +1 };

			using vt = vertex_struct_terrain<Type>;
			std::vector<vertex_struct_terrain<Type>> points;

			generateTerrainVerticesIndices(TERRAIN_SIZE, 0.1);
			Tools::Point2d<Type> test{0, 0};



			for (Tools::Point3d<float>& p : m_vertexVect)
			{
				points.push_back(vt{ p, nyp, vg , test});
			}

			m_nbVertices = static_cast<GLsizei>(points.size());

			float squareSize = 1.0f / (m_nbVertices / 3);

			for (vertex_struct_terrain<Type>& p : points) {

				// Small texture
				int xSquare = static_cast<int>((p.p.x + 0.5f) / squareSize);
				int zSquare = static_cast<int>((p.p.z + 0.5f) / squareSize);
				p.t = { xSquare * squareSize, zSquare * squareSize };

				// Multiple Medium Texture
				//p.t = { p.p.x * 0.5f + 0.5f, p.p.z * 0.5f + 0.5f };

				// One big texture
				//p.t = { (p.p.x - 0) / TERRAIN_SIZE, (p.p.z - 0) / TERRAIN_SIZE };
			}


			unsigned int counter = 0;
			for (int i = 2; i < m_indices.size(); i += 3) {

				int index = m_indices.at(i);
				vt& p3 = points.at(m_indices.at(i));
				vt& p2 = points.at(m_indices.at(i - 1));
				vt& p1 = points.at(m_indices.at(i - 2));

				
				

				//calculate vector
				Tools::Point3d<Type> vec12 = { p2.p.x - p1.p.x, p2.p.y - p1.p.y, p2.p.z - p1.p.z };
				Tools::Point3d<Type> vec13 = { p3.p.x - p1.p.x, p3.p.y - p1.p.y, p3.p.z - p1.p.z };

				//Calculate normal
				Tools::Point3d<Type> normal =
				{
					(vec12.z * vec13.y) - (vec12.y * vec13.z),
					(vec12.x * vec13.z) - (vec12.z * vec13.x),
					(vec12.y * vec13.x) - (vec12.x * vec13.y)
				};

				p1.n = ((p1.n * p1.nb_face) + normal) / (p1.nb_face + 1);
				p1.n = p1.n / std::sqrt((p1.n.x * p1.n.x) + (p1.n.y * p1.n.y) + (p1.n.z * p1.n.z));

				p1.nb_face += 1;

				p2.n = ((p2.n * p2.nb_face) + normal) / (p2.nb_face + 1);
				p2.n = p2.n / std::sqrt((p2.n.x * p2.n.x) + (p2.n.y * p2.n.y) + (p2.n.z * p2.n.z));

				p2.nb_face += 1;

				p3.n = ((p3.n * p3.nb_face) + normal) / (p3.nb_face + 1);
				p3.n = p3.n / std::sqrt((p3.n.x * p3.n.x) + (p3.n.y * p3.n.y) + (p3.n.z * p3.n.z));

				p3.nb_face += 1;

			}

			

			// Allocate storage size units of OpenGL
			// Copy data from client to server
			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vertex_struct_terrain<Type>), points.data(), GL_STATIC_DRAW);

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

			// En cas de bug penser a changer le décalage par rapport au autres données dans la struct
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
			glBindVertexArray(m_vao);

			Tools::Mat4<Type> Model = Tools::Mat4<Type>::translation(0, 0, -5) * Tools::Mat4<Type>::rotationY(m_angleY) * Tools::Mat4<Type>::rotationX(m_angleX);

			glUniformMatrix4fv(glGetUniformLocation(m_program, "ModelMatrix"), 1, GL_FALSE, Model.getData());
			glUniformMatrix4fv(glGetUniformLocation(m_program, "ViewMatrix"), 1, GL_FALSE, View.getData());
			glUniformMatrix4fv(glGetUniformLocation(m_program, "ProjectionMatrix"), 1, GL_FALSE, Projection.getData());

			glUniform1f(glGetUniformLocation(m_program, "material.ambient"), 0.3f);
			glUniform1f(glGetUniformLocation(m_program, "material.diffuse"), 0.7f);
			glUniform1f(glGetUniformLocation(m_program, "material.specular"), 1.f);
			glUniform1f(glGetUniformLocation(m_program, "material.specularSmoothness"), 2.0f);

			glUniform3f(glGetUniformLocation(m_program, "light.direction"), 0.f, -1.f, 0.f);
			glUniform3f(glGetUniformLocation(m_program, "light.color"), 1.f, 1.f, 1.f);

			glUniform3f(glGetUniformLocation(m_program, "camera.worldPosition"), 0.f, 0.f, 0.f);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
			m_texture.bind();

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
		GLuint m_program;
		GLsizei m_nbVertices;

		GLuint m_elementbuffer;
		std::vector<Tools::Point3d<Type>> m_vertexVect;
		std::vector<unsigned int> m_indices;

		Texture m_texture;
		ProceduralGeneration::PerlinNoise<Type> m_perlinNoise;

	};
