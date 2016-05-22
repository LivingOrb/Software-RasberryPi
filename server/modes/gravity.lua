local playing = false
local current_led
local target_led
local max_countdown
local countdown = 0
local hue
local hsvs = {}

for i = 1, Count do
	table.insert(hsvs, {0.0, 0.0, 0.0})
end

local function lerp(a, b, t)
	return a + t * (b - a)
end

local function pingpong(t)
	t = t % 1.0
	if t < 0.5 then
		return 2.0 * t
	else
		return 2.0 * (1.0 - t)
	end
end

local function find_highest_leds()
	table.sort(indices, function(a, b)
		local aX, aY, aZ = get_world_position(a)
		local bX, bY, bZ = get_world_position(b)
		return aZ > bZ
	end)
	return indices
end

local function find_lowest_neighbors(led)
	local neighbors = get_neighbors(led)
	table.sort(neighbors, function(a, b)
		local aX, aY, aZ = get_world_position(a)
		local bX, bY, bZ = get_world_position(b)
		return aZ < bZ
	end)
	return neighbors
end

local function turn_on_current_led()
	hsvs[current_led] = {hue, 1.0, 1.0}
end

function update(dt)
	for _, hsv in ipairs(hsvs) do
		hsv[3] = math.max(0.0, hsv[3] - dt * 0.5)
	end

	countdown = countdown - dt

	if not playing then
		if countdown <= 0.0 then
			playing = true

			max_countdown = 0.5
			countdown = max_countdown
			hue = 0.0

			for i = 1, Count do
				hsvs[i] = {0.0, 0.0, 0.0}
			end

			current_led = find_highest_leds()[1]
			target_led = find_lowest_neighbors(current_led)[1]
			turn_on_current_led()
		else
			for i = 1, Count do
				local x, y, z = get_world_position(i)
				local target_z = lerp(1.0, -1.0, pingpong(countdown / max_countdown))
				hsvs[i] = {0.0, 1.0, 1.0 - 4.0 * math.abs(z - target_z)}
			end
		end
	else
		while countdown <= 0.0 do
			max_countdown = max_countdown * 0.99
			countdown = countdown + max_countdown
			hue = hue + 0.05

			local neighbors = find_lowest_neighbors(target_led)
			local next_target_led = neighbors[1]
			if next_target_led == current_led then
				next_target_led = neighbors[2]
			end
			current_led = target_led
			target_led = next_target_led
			turn_on_current_led()
		end

		local x, y, z = get_world_position(current_led)
		if z < -0.5 then
			playing = false
			max_countdown = 2.0
			countdown = max_countdown
		end
	end

	for i, hsv in ipairs(hsvs) do
		set_hsv(i, table.unpack(hsv))
	end
end
