
local function find_highest_leds()
	table.sort(indices, function(a, b)
		local aX, aY, aZ = get_world_position(a)
		local bX, bY, bZ = get_world_position(b)
		return aZ > bZ
	end)
	return indices
end

function update(dt)
	local i = find_highest_leds()[1]
	set_rgb(i, 1.0, 0.0, 0.0)
	for _, n in ipairs(get_neighbors(i)) do
		set_rgb(n, 0.0, 1.0, 0.0)
	end
end
