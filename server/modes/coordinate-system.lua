local function sort_indices_by_world_position(axis)
	return table.sort(indices, function(a, b)
		local aX, aY, aZ = get_world_position(a)
		local bX, bY, bZ = get_world_position(b)
		local score = 0
		if axis == "x" then
			score = aX - bX
		elseif axis == "y" then
			score = aY - bY
		elseif axis == "z" then
			score = aZ - bZ
		end
		return score > 0
	end)
end

function update(dt)
	sort_indices_by_world_position("x")
	set_rgb(indices[1], 1.0, 0.0, 0.0)

	sort_indices_by_world_position("y")
	set_rgb(indices[1], 0.0, 1.0, 0.0)

	sort_indices_by_world_position("z")
	set_rgb(indices[1], 0.0, 0.0, 1.0)
end
