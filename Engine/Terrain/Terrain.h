#pragma once

#include <vector>
#include "Tools/MathUtils.h"

typedef unsigned int GLuint;
typedef int GLsizei;


namespace Engine {
	template<typename Type>
	struct vertex_struct_terrain
	{
		vertex_struct_terrain(const Tools::Point3d<Type>& p_, const Tools::Point3d<Type>& n_, const Tools::Color<Type>& c_) : p(p_), c(c_), n(n_) {}
		Tools::Point3d<Type> p;
		Tools::Point3d<Type> n;

		Tools::Color<Type> c;
		int nb_face = 0;

	};


	template<typename Type>
	class Terrain
	{
	public:

		Terrain();

		~Terrain();

		//void GenerateVertex();
		//void GenerateIndices();

		void generateTerrainVerticesIndices(float size, float step);
		void load();
		void render(const Tools::Mat4<Type>& View, const Tools::Mat4<Type>& Projection);
		void update();


	private:
		Type m_angleX = 0;
		Type m_angleY = 0;
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_program;
		GLsizei m_nbVertices;

		GLuint m_elementbuffer;
		std::vector<Tools::Point3d<Type>> m_vertexVect;
		std::vector<unsigned int> m_indices;
	};

	template<typename Type>
	inline Terrain<Type>::Terrain()
	{
	}

	template<typename Type>
	inline Terrain<Type>::~Terrain()
	{
	}

	template<typename Type>
	inline void Terrain<Type>::generateTerrainVerticesIndices(float size, float step)
	{
	}

	template<typename Type>
	inline void Terrain<Type>::load()
	{
	}

	template<typename Type>
	inline void Terrain<Type>::render(const Tools::Mat4<Type>& View, const Tools::Mat4<Type>& Projection)
	{
	}

	template<typename Type>
	inline void Terrain<Type>::update()
	{
	}

}