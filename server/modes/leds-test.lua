local index = 1
local hue = 0

function update(dt)
	hue = (hue + 0.02) % 1.0
	set_color(index, hsv_to_rgb(hue, 1.0, 1.0))

	index = index + 1
	if index > Count then
		index = 1
	end
end
