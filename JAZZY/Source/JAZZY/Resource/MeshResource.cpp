#include <JAZZY/Resource/MeshResource.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec2.h>

#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/Resource/ResourceManager.h>

#include <filesystem>
#include <ranges>
#include <map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

jazzy::MeshResource::MeshResource(const MeshResourceDesc& desc) : Resource(desc.base)
{
	// Get the obj file path
	auto inputfile{ std::filesystem::path(desc.base.path).string() };
	// Declare objReader
	tinyobj::ObjReader reader{};

	// Check if objReader can read the file and if there are any errors or warnings
	if (!reader.ParseFromFile(inputfile)) DX3DLogThrowError("Mesh failed to load.");
	if (!reader.Error().empty()) DX3DLogThrowError("Failed to load mesh {}. Details: {}", inputfile, reader.Error());
	if (!reader.Warning().empty()) DX3DLogWarning("Mesh {} loaded with warnings: {}", inputfile, reader.Warning());

	// Get attribs and shapes from the reader
	// Attribs contains: vertices, normals, texcoords, colors
	const auto& attribs = reader.GetAttrib();
	// A 3D model is made up of 'shape's and a 'shape' is made up of the indices of the vertices
	const auto& shapes = reader.GetShapes();

	// Declare GameEngine MeshVertex list and Indices
	std::vector<MeshVertex> listVertices{};
	std::vector<jazzy::ui32> listIndices{};

	// Urm,,, what?
	// I guess for each material, there are different vertex and index lists?
	std::map<jazzy::i32, std::vector<jazzy::ui32>> indicesPerMaterial{};


	for (const auto& shape : shapes)
	{
		auto indexOffset{ 0u };
		// For each vertex in the shape
		for (auto f : std::views::iota(0u, shape.mesh.num_face_vertices.size()))
		{
			auto numFaceVerts = shape.mesh.num_face_vertices[f];

			int material = shape.mesh.material_ids[f];
			auto& materialIndices = indicesPerMaterial[material];
			auto startIndexMaterial{ materialIndices.size() };

			// For each face in the shape, get the vertices, texcoord, and normals
			for (auto v : std::views::iota(0u, numFaceVerts))
			{
				auto& index{ shape.mesh.indices[indexOffset + v] };

				MeshVertex vertex{};

				vertex.position = Vec3{
					attribs.vertices[index.vertex_index * 3 + 0],
					attribs.vertices[index.vertex_index * 3 + 1],
					-attribs.vertices[index.vertex_index * 3 + 2]
				};

				if (index.texcoord_index >= 0)
				{
					vertex.texcoord = Vec2{
						attribs.texcoords[index.texcoord_index * 2 + 0],
						1.0f - attribs.texcoords[index.texcoord_index * 2 + 1]
					};
				}

				if (index.normal_index >= 0)
				{
					vertex.normal = Vec3{
					attribs.normals[index.normal_index * 3 + 0],
					attribs.normals[index.normal_index * 3 + 1],
					-attribs.normals[index.normal_index * 3 + 2]
					};
				}

				materialIndices.push_back(static_cast<jazzy::ui32>(listVertices.size()));
				listVertices.push_back(vertex);
			}
			std::swap(materialIndices[startIndexMaterial + 1], materialIndices[startIndexMaterial + 2]);

			indexOffset += numFaceVerts;
		}
	}

	for (const auto& [materialIndex, materialIndices] : indicesPerMaterial)
	{
		MaterialSlot slot{
			static_cast<jazzy::ui32>(listIndices.size()),
			static_cast<jazzy::ui32>(materialIndices.size()),
			materialIndex
		};

		listIndices.insert(
			listIndices.end(),
			materialIndices.begin(),
			materialIndices.end());

		m_matSlots.push_back(slot);
	}

	m_vertexBuffer = desc.graphicsDevice.createVertexBuffer(
		{ listVertices.data() ,static_cast<jazzy::ui32>(listVertices.size()), sizeof(MeshVertex) });
	m_indexBuffer = desc.graphicsDevice.createIndexBuffer(
		{ listIndices.data(), static_cast<jazzy::ui32>(listIndices.size()) });
}

const jazzy::MaterialSlot* jazzy::MeshResource::getMaterialSlots(ui32& numSlots) const noexcept
{
	numSlots = static_cast<jazzy::ui32>(m_matSlots.size());
	return m_matSlots.data();
}

jazzy::ui32 jazzy::MeshResource::getNumMaterialSlots() const noexcept
{
	return static_cast<jazzy::ui32>(m_matSlots.size());
}

const jazzy::VertexBuffer& jazzy::MeshResource::getVertexBuffer() const noexcept
{
	return *m_vertexBuffer;
}

const jazzy::IndexBuffer& jazzy::MeshResource::getIndexBuffer() const noexcept
{
	return *m_indexBuffer;
}
