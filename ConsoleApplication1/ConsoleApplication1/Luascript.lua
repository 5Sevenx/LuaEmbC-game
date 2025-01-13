--+++++++++++++++++++++++++++++++++++++++++++++++++++ LUA script ++++++++++++++++++++++++++++++++++++++++++++++++

--variables
local TITLE_EMPTY = 0
local TITLE_BLOCK = 1

function LoadLevel(host, level)
    -- Initialize the map and size
    local map = ""
    local size = {w = 16, h = 15}

    if level == 1 then
        map = 
        ".................." ..
        ".................." ..
        ".................." ..
        ".................." ..
        "...........#######" ..
        "..............####" ..
        "######.........###" ..
        "##..............##" ..
        "##...............#" ..
        "##...............#" ..
        "##.....###.......#" ..
        "##################" ..
        ".................." ..
        ".................." ..
        ".................."
    end

    -- C++ function to create the level
    _CreateLevel(host, size.w, size.h)

    -- Construct level tile by tile, iterating through all map
    for y = 1, size.h do
        for x = 1, size.w do
            local c = string.sub(map, ((y - 1) * size.w + x), ((y - 1) * size.w + x))

            if c == '.' then
                _SetTitle(host, x - 1, y - 1, TITLE_EMPTY)
            elseif c == '#' then
                _SetTitle(host, x - 1, y - 1, TITLE_BLOCK)
            end
        end
    end
end
