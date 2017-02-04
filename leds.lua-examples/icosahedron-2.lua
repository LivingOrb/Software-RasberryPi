local vertices = {
	{0.0, 0.0, 1.0},
	{0.894425, 0.0, 0.447215},
	{0.276385, 0.85064, 0.447215},
	{-0.7236, 0.52572, 0.447215},
	{-0.7236, -0.52572, 0.447215},
	{0.276385, -0.85064, 0.447215},
	{0.0, 0.0, -1.0},
	{-0.894425, 0.0, -0.447215},
	{-0.276385, -0.85064, -0.447215},
	{0.7236, -0.52572, -0.447215},
	{0.7236, 0.52572, -0.447215},
	{-0.276385, 0.85064, -0.447215},
}

local faces = {
	{1, 2, 3},
	{1, 3, 4},
	{1, 4, 5},
	{1, 5, 6},
	{1, 6, 2},
	{7, 8, 9},
	{7, 9, 10},
	{7, 10, 11},
	{7, 11, 12},
	{7, 12, 8},
	{2, 11, 10},
	{2, 3, 11},
	{3, 12, 11},
	{3, 4, 12},
	{4, 8, 12},
	{4, 5, 8},
	{5, 9, 8},
	{5, 6, 9},
	{6, 10, 9},
	{6, 2, 10},
}

-- Tesselate icosahedron.

local middle_index_map = {}

local function get_middle_index(index1, index2)
	local key = (index1 < index2) and (index1 .. '-' .. index2) or (index2 .. '-' .. index1)
	if middle_index_map[key] then
		return middle_index_map[key]
	end

	local vertex1 = vertices[index1];
	local vertex2 = vertices[index2];
	local vertex = {
		vertex1[1] + vertex2[1],
		vertex1[2] + vertex2[2],
		vertex1[3] + vertex2[3],
	}
	local length = math.sqrt(vertex[3] * vertex[3] + vertex[1] * vertex[1] + vertex[2] * vertex[2])
	vertex[1] = vertex[1] / length;
	vertex[2] = vertex[2] / length;
	vertex[3] = vertex[3] / length;

	table.insert(vertices, vertex)
	middle_index_map[key] = #vertices
	return #vertices
end

for _ = 1, 2 do
	local new_faces = {}
	for _, face in ipairs(faces) do
		local index01 = get_middle_index(face[3], face[1])
		local index12 = get_middle_index(face[1], face[2])
		local index20 = get_middle_index(face[2], face[3])

		table.insert(new_faces, {face[3], index01, index20})
		table.insert(new_faces, {face[1], index12, index01})
		table.insert(new_faces, {face[2], index20, index12})
		table.insert(new_faces, {index01, index12, index20})
	end
	faces = new_faces
end

-- Find highest vertex,
-- Compute angles around Z axis,
-- Compute neighbor leds.

local top_vertex_index
local top_vertex_z = 0

local neighbor_sets = {}

for i, vertex in ipairs(vertices) do
	if vertex[3] > top_vertex_z then
		top_vertex_index = i
		top_vertex_z = vertex[3]
	end

	vertex.angle = math.atan2(-vertex[2], vertex[1])
	if vertex.angle < -0.01 then
		vertex.angle = vertex.angle + 2 * math.pi
	end

	local neighbor_set = {}
	for _, face in ipairs(faces) do
		if face[1] == i then
			neighbor_set[face[2]] = true
			neighbor_set[face[3]] = true
		end
		if face[2] == i then
			neighbor_set[face[1]] = true
			neighbor_set[face[3]] = true
		end
		if face[3] == i then
			neighbor_set[face[1]] = true
			neighbor_set[face[2]] = true
		end
	end
	table.insert(neighbor_sets, neighbor_set)
end

-- Sort vertices by horizontal layer, then angle.

local discovered_vertices = { top_vertex_index }

function table_contains(t, vertex_index)
	for _, v in ipairs(t) do
		if v == vertex_index then
			return true
		end
	end
	return false
end

while true do
	local closest_vertices = {}

	for _, face in ipairs(faces) do
		local c1 = table_contains(discovered_vertices, face[1])
		local c2 = table_contains(discovered_vertices, face[2])
		local c3 = table_contains(discovered_vertices, face[3])
		if c1 or c2 or c3 then
			if not c1 and not table_contains(closest_vertices, face[1]) then
				table.insert(closest_vertices, face[1])
			end
			if not c2 and not table_contains(closest_vertices, face[2]) then
				table.insert(closest_vertices, face[2])
			end
			if not c3 and not table_contains(closest_vertices, face[3]) then
				table.insert(closest_vertices, face[3])
			end
		end
	end

	if #closest_vertices == 0 then
		break
	end

	table.sort(closest_vertices, function(a, b)
		return vertices[a].angle < vertices[b].angle
	end)

	for _, v in ipairs(closest_vertices) do
		table.insert(discovered_vertices, v)
	end
end

-- Export values.

set_count(#vertices)

for i, index in ipairs(discovered_vertices) do
	local vertex = vertices[index]
	vertex.index = i
end

for i, index in ipairs(discovered_vertices) do
	local vertex = vertices[index]
	set_sphere_position(i, vertex[1], vertex[2], vertex[3])

	for neighbor in pairs(neighbor_sets[index]) do
		add_neighbor(i, vertices[neighbor].index)
	end
end
