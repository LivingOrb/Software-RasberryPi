local t = 0

function update(dt)
    t = t + dt
	for i = 1,Count do
	    set_color(i, math.sin(t * 10 + i * 0.10), math.sin(t * 8 + i * 0.08), math.sin(t * 13 + i * 0.14))
    end
    
end
