#include "Terrain.h"
//#include <iostream>
//#include <GL/glew.h>
//#include "SFML/OpenGL.hpp"
//#include "Shader.h"
//
//
//
////void Terrain::GenerateVertex() {
////	sf::Vector2i terrainSizeTest = { 2 , 3 };
////
////	// 2 correspond au nombre de triangle par parcel et 3 au nombre d'indices necéssaire pour chaques triangles.
////	//m_Indices.resize(terrainSizeTest.x * terrainSizeTest.y * 2 * 3);
////
////	m_vertexVect.resize(terrainSizeTest.x * terrainSizeTest.y);
////
////	for (uint32_t y = 0; y < terrainSizeTest.y; ++y) {
////		for (uint32_t x = 0; x < terrainSizeTest.x; ++x)
////		{
////			const uint32_t terrainIndex = y * terrainSizeTest.x + x;
////			auto& position = m_vertexVect[terrainIndex];
////
////			position.x = 1 * x;
////			position.z = 1 * y;
////			position.y = 0;
////		}
////	}
////}
////
////void Terrain::GenerateIndices() {
////
////	sf::Vector2i terrainSizeTest = { 1 , 2 };
////	uint32_t index = 0;
////
////	// 2 correspond au nombre de triangle par parcel et 3 au nombre d'indices necéssaire pour chaques triangles.
////	m_indices.resize(terrainSizeTest.x * terrainSizeTest.y * 2 * 3);
////
////	for (uint32_t y = 0; y < terrainSizeTest.y; ++y) {
////		for (uint32_t x = 0; x < terrainSizeTest.x; ++x)
////		{
////			const uint32_t vertexIndex = y * terrainSizeTest.x + x;
////
////			//top triangle
////			m_indices[index++] = vertexIndex;
////			m_indices[index++] = vertexIndex + terrainSizeTest.x + 1;
////			m_indices[index++] = vertexIndex + 1;
////			//bottom triangle
////			m_indices[index++] = vertexIndex;
////			m_indices[index++] = vertexIndex + terrainSizeTest.x;
////			m_indices[index++] = vertexIndex + terrainSizeTest.x + 1;
////
////		}
////	}
////}
//
//namespace Engine{
//
//
//	template<typename Type>
//	void Terrain<Type>::generateTerrainVerticesIndices(float size, float step) {
//
//		int numVertices = static_cast<int>(size / step) + 1;
//		int height = -1;
//
//		for (int i = 0; i < numVertices; i++) {
//			for (int j = 0; j < numVertices; j++) {
//				Type x = i * step;
//				Type z = j * step;
//				Type y = height;
//				m_vertexVect.push_back(Tools::Point3d<Type>{x, y, z});
//
//			}
//		}
//
//		// Générer les indices pour les triangles
//		for (int i = 0; i < numVertices - 1; i++) {
//			for (int j = 0; j < numVertices - 1; j++) {
//				// Indices des sommets des deux triangles formant un carré
//				unsigned int index1 = i * numVertices + j;
//				unsigned int index2 = index1 + 1;
//				unsigned int index3 = (i + 1) * numVertices + j;
//				unsigned int index4 = index3 + 1;
//
//				// Premier triangle
//				m_indices.push_back(index1);
//				m_indices.push_back(index2);
//				m_indices.push_back(index3);
//
//				// Deuxième triangle
//				m_indices.push_back(index2);
//				m_indices.push_back(index4);
//				m_indices.push_back(index3);
//			}
//		}
//	}
//
//	template<typename Type>
//	void Terrain<Type>::load()
//	{
//
//		// We want only one buffer with the id generated and stored in m_vao
//		glGenVertexArrays(1, &m_vao);
//
//		// create a new active VAO
//		glBindVertexArray(m_vao);
//
//		// we want only one buffer with the id generated and stored in m_vbo
//		glGenBuffers(1, &m_vbo);
//
//		// 1. create a new active VBO if doesn’t exist
//		// 2. if the VBO is active it will make it active
//		// 3. if binded to 0, OpenGL stops
//		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//
//		Tools::Point3d<Type> P000 = { -1, -1, -1 }; Tools::Point3d<Type> P001 = { -1, -1, +1 }; Tools::Point3d<Type> P010 = { -1, +1, -1 }; Tools::Point3d<Type> P011 = { -1, +1, +1 };
//		Tools::Point3d<Type> P100 = { +1, -1, -1 }; Tools::Point3d<Type> P101 = { +1, -1, +1 }; Tools::Point3d<Type> P110 = { +1, +1, -1 }; Tools::Point3d<Type> P111 = { +1, +1, +1 };
//
//		Tools::Color<Type> vr = { 1, 0, 0, 1 }; Tools::Color<Type> vg = { 0, 1, 0, 1 }; Tools::Color<Type> vb = { 0, 0, 1, 1 };
//		Tools::Color<Type> ar = { 0, 1, 1, 1 }; Tools::Color<Type> ag = { 1, 0, 1, 1 }; Tools::Color<Type> ab = { 1, 1, 0, 1 };
//
//		Tools::Point3d<Type> nxn = { -1, 0, 0 }; Tools::Point3d<Type> nxp = { +1, 0, 0 };
//		Tools::Point3d<Type> nyn = { 0, -1, 0 }; Tools::Point3d<Type> nyp = { 0, +1, 0 };
//		Tools::Point3d<Type> nzn = { 0, 0, -1 }; Tools::Point3d<Type> nzp = { 0, 0, +1 };
//
//		using vt = vertex_struct_terrain<Type>;
//		std::vector<vertex_struct_terrain<Type>> points;
//
//		generateTerrainVerticesIndices(20, 0.01);
//
//		for (Tools::Point3d<float>& p : m_vertexVect)
//		{
//			points.push_back(vt{ p, nyp, vg });
//		}
//
//		for (int i = 2; i < m_indices.size(); i += 3) {
//			int index = m_indices.at(i);
//			vt& p1 = points.at(m_indices.at(i));
//			vt& p2 = points.at(m_indices.at(i - 1));
//			vt& p3 = points.at(m_indices.at(i - 2));
//
//			//calculate vector
//			Tools::Point3d<Type> vec12 = { p2.p.x - p1.p.x, p2.p.y - p1.p.y, p2.p.z - p1.p.z };
//			Tools::Point3d<Type> vec13 = { p3.p.x - p1.p.x, p3.p.y - p1.p.y, p3.p.z - p1.p.z };
//
//			//Calculate normal
//			Tools::Point3d<Type> normal =
//			{
//				(vec12.z * vec13.y) - (vec12.y * vec13.z),
//				(vec12.x * vec13.z) - (vec12.z * vec13.x),
//				(vec12.y * vec13.x) - (vec12.x * vec13.y)
//			};
//
//			p1.n = ((p1.n * p1.nb_face) + normal) / (p1.nb_face + 1);
//			p1.n = p1.n / std::sqrt((p1.n.x * p1.n.x) + (p1.n.y * p1.n.y) + (p1.n.z * p1.n.z));
//
//			p1.nb_face += 1;
//
//			p2.n = ((p2.n * p2.nb_face) + normal) / (p2.nb_face + 1);
//			p2.n = p2.n / std::sqrt((p2.n.x * p2.n.x) + (p2.n.y * p2.n.y) + (p2.n.z * p2.n.z));
//
//			p2.nb_face += 1;
//
//			p3.n = ((p3.n * p3.nb_face) + normal) / (p3.nb_face + 1);
//			p3.n = p3.n / std::sqrt((p3.n.x * p3.n.x) + (p3.n.y * p3.n.y) + (p3.n.z * p3.n.z));
//
//			p3.nb_face += 1;
//
//		}
//
//		m_nbVertices = static_cast<GLsizei>(points.size());
//
//		// Allocate storage size units of OpenGL
//		// Copy data from client to server
//		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vertex_struct_terrain<Type>), points.data(), GL_STATIC_DRAW);
//
//		ShaderInfo shaders[] = {
//			{GL_VERTEX_SHADER, "terrain.vert"},
//			{GL_FRAGMENT_SHADER, "terrain.frag"},
//			{GL_NONE, nullptr}
//		};
//
//		m_program = Shader::loadShaders(shaders);
//		glUseProgram(m_program);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), 0);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), (char*)(0) + sizeof(vertex_struct_terrain<Type>::p));
//		glEnableVertexAttribArray(1);
//
//		// En cas de bug penser a changer le décalage par rapport au autres données dans la struct
//		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_terrain<Type>), (char*)(0) + sizeof(vertex_struct_terrain<Type>::p) + sizeof(vertex_struct_terrain<Type>::n));
//		glEnableVertexAttribArray(2);
//
//
//
//		glGenBuffers(1, &m_elementbuffer);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
//	}
//
//	template<typename Type>
//	void Terrain<Type>::render(const Tools::Mat4<Type>& View, const Tools::Mat4<Type>& Projection)
//	{
//		glBindVertexArray(m_vao);
//
//		Tools::Mat4<Type> Model = Tools::Mat4<Type>::translation(0, 0, -5) * Tools::Mat4<Type>::rotationY(m_angleY) * Tools::Mat4<Type>::rotationX(m_angleX);
//
//		glUniformMatrix4fv(glGetUniformLocation(m_program, "ModelMatrix"), 1, GL_FALSE, Model.getData());
//		glUniformMatrix4fv(glGetUniformLocation(m_program, "ViewMatrix"), 1, GL_FALSE, View.getData());
//		glUniformMatrix4fv(glGetUniformLocation(m_program, "ProjectionMatrix"), 1, GL_FALSE, Projection.getData());
//
//		glUniform1f(glGetUniformLocation(m_program, "material.ambient"), 0.3f);
//		glUniform1f(glGetUniformLocation(m_program, "material.diffuse"), 0.7f);
//		glUniform1f(glGetUniformLocation(m_program, "material.specular"), 1.f);
//		glUniform1f(glGetUniformLocation(m_program, "material.specularSmoothness"), 2.0f);
//
//		glUniform3f(glGetUniformLocation(m_program, "light.direction"), 0.f, -1.f, 0.f);
//		glUniform3f(glGetUniformLocation(m_program, "light.color"), 1.f, 1.f, 1.f);
//
//		glUniform3f(glGetUniformLocation(m_program, "camera.worldPosition"), 0.f, 0.f, 0.f);
//
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
//
//		glDrawElements(
//			GL_TRIANGLES,      // mode
//			m_indices.size(),    // count
//			GL_UNSIGNED_INT,   // type
//			(void*)0           // element array buffer offset
//		);
//
//	}
//
//	template<typename Type>
//	void Terrain<Type>::update()
//	{
//		/*m_angleX += 0.0125f;
//		m_angleY += 0.025f;*/
//	}
//}