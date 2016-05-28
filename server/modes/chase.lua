local current_led = 1
local target_led = 2
local max_countdown = 0.1
local countdown = max_countdown
local hue = 0.0
local hsvs = {}

for i = 1, Count do
	table.insert(hsvs, {0.0, 0.0, 0.0})
end

local function turn_on_current_led()
	hsvs[current_led] = {hue, 1.0, 1.0}
end

turn_on_current_led()

function update(dt)
	for _, hsv in ipairs(hsvs) do
		hsv[3] = math.max(0.0, hsv[3] - dt * 0.5)
	end
	
	countdown = countdown - dt
	
	while countdown <= 0.0 do
		countdown = countdown + max_countdown
		hue = hue + 0.05

		local neighbors = get_neighbors(target_led)
		local next_target_led
		repeat
		    next_target_led = neighbors[math.random(#neighbors)]
		until next_target_led ~= current_led
		
		current_led = target_led
		target_led = next_target_led
		turn_on_current_led()
	end

	for i, hsv in ipairs(hsvs) do
		set_color(i, hsv_to_rgb(table.unpack(hsv)))
	end
end
