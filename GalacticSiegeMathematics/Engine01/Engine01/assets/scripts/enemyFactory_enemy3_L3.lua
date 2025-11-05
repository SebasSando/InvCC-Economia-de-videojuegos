local spawnTimer = 0
local spawnInterval = 8.00

function createEnemy3(dt, wH, wW)
    spawnTimer = spawnTimer + dt
    if spawnTimer >= spawnInterval then
        enemy3Factory(wH, wW)
        spawnTimer = 0
    end
end