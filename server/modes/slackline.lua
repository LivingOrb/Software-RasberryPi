local current_led
local next_led
local next_leds
local score
local line_led_colors

local next_leds_count = 8
local top_ratio_threshold = 0.05
local max_top_ratio = 0.1
local display_top_ratio = 0.1

local function lerp(a, b, t)
    if type(a) == "table" then
        if #a ~= #b then
            error("Sizes are different")
        end
        local c = {}
        for i = 1, #a do
            table.insert(c, lerp(a[i], b[i], t))
        end
        return c
    end
    return a + t * (b - a)
end

local function top_ratio(led)
    local x, y, z = get_world_position(led)
    if z < 0 then
        return 1
    end
    return x^2 + y^2
end

local function update_next_leds()
    while #next_leds < next_leds_count do
        local led = next_leds[#next_leds]
        local previous_led = next_leds[#next_leds - 1]
        local neighbors = get_neighbors(led)
        local next_led
        repeat
            next_led = neighbors[math.random(#neighbors)]
        until next_led ~= previous_led
        table.insert(next_leds, next_led)
    end
    
    line_led_colors = {}
    for i, led in ipairs(next_leds) do
        line_led_colors[led] = { hsv_to_rgb(0.55, 1.0, lerp(0.2, 0.02, i / next_leds_count)) }
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

local playing = false
local death_timer = 0.0
local orange = { hsv_to_rgb(0.05, 1.0, 1.0) }
local black = { 0.0, 0.0, 0.0 }

function update(dt)
    if not playing then
        death_timer = death_timer - dt
        if death_timer <= 0.0 then
            playing = true
            
            current_led = find_highest_leds()[1]
            local neighbors = get_neighbors(current_led)
            next_led = neighbors[math.random(#neighbors)]
            score = 0
            
            next_leds = {}
            table.insert(next_leds, current_led)
            table.insert(next_leds, next_led)
            update_next_leds()
        else
            return
        end
    end
    
    local current_top_ratio = top_ratio(current_led)
    local next_top_ratio = top_ratio(next_led)
    
	for led = 1, Count do
	    set_color(led, table.unpack(lerp(orange, line_led_colors[led] or black, top_ratio(led) / display_top_ratio)))
    end
    
    if next_top_ratio < top_ratio_threshold then
        table.remove(next_leds, 1)
        update_next_leds()
        current_led = next_led
        next_led = next_leds[1]
        score = score + 1
    elseif current_top_ratio > max_top_ratio and next_top_ratio > max_top_ratio then
        playing = false
        death_timer = 1.0
        print(score)
    end
    
end
