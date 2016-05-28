local function top_ratio(led)
    local x, y, z = get_world_position(led)
    return x^2 + y^2
end

function update(dt)
    for i = 1, Count do
        set_color(i, hsv_to_rgb(0.04, 1.0, 1.0 - top_ratio(i) / 0.2))
    end
end
