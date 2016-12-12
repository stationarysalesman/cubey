#ifndef SURFACE_H_
#define SURFACE_H_
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "particle_system.h"


class SurfaceRender
{
	public:
		SurfaceRender(int n, int b, float f, float s); 
		ParticleSystem _psys;
		void init();
		void March();	
		glm::vec3 InterpolateEdge(glm::vec3& v1, glm::vec3& v2);
		void EvaluateCube(float, float, float);	
		void Test();	
		static std::vector<std::tuple<int, int> > EdgeMappings;
		void RenderPoints() { _points = true; _tris = false; }	
		void RenderTriangles() { _points = false; _tris = true; }	
		void ClearGeometry();
		void RenderAsPoints() { _points = true; _tris = false; }
		void RenderAsTris() { _tris = true; _points = false; }	
		std::vector<glm::uvec3> t_faces;
		std::vector<glm::vec4> p_vertices; /* point */
		std::vector<glm::vec4> p_normals;
		std::vector<unsigned int> p_faces;
		bool _points = false;
		bool _tris = true;		
		
	private:
		std::map<int, std::vector<int> > edge_table;
		int boundary;
		int idx_count;
		float iso_value;
		float scale;
};

#endif
