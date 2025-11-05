local spawnTimer = 0
local spawnInterval = 2.33

function createEnemy2(dt, wH, wW)
    spawnTimer = spawnTimer + dt
    if spawnTimer >= spawnInterval then
        enemy2Factory(wH, wW)
        spawnTimer = 0
    end
end