/* SPDX-License-Identifier: ISC */

#include "euler/renderer/polygon.h"

#include "VK2D/Polygon.h"

Euler::Renderer::Polygon::Polygon(const std::vector<VertexColor> &vertices)
{
	std::vector<VK2DVertexColour> vk2d_vertices;
	vk2d_vertices.reserve(vertices.size());
	for (const auto &[position, color] : vertices) {
		vk2d_vertices.push_back({
		    .pos = { position[0], position[1], position[2] },
		    .colour = { color[0], color[1], color[2], color[3] },
		});
	}
	_polygon
	    = vk2dPolygonShapeCreateRaw(vk2d_vertices.data(), vertices.size());
	if (_polygon == nullptr)
		throw std::runtime_error("Failed to create polygon");
}

Euler::Renderer::Polygon::Polygon(const std::vector<Eigen::Vector2f> &vertices,
    const bool filled)
{
	auto vk2d_vertices = new vec2[vertices.size()];
	for (size_t i = 0; i < vertices.size(); ++i) {
		vk2d_vertices[i][0] = vertices[i][0];
		vk2d_vertices[i][1] = vertices[i][1];
	}
	_polygon = filled
	    ? vk2dPolygonCreate(vk2d_vertices, vertices.size())
	    : vk2dPolygonCreateOutline(vk2d_vertices, vertices.size());
	delete[] vk2d_vertices;
}

Euler::Renderer::Polygon::~Polygon()
{
	if (_polygon != nullptr) vk2dPolygonFree(_polygon);
	_polygon = nullptr;
}