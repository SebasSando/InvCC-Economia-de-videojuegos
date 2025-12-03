local shootTimer = 0
local baseShootInterval = 1.5
local level2ShootInterval = 1.0

local function get_shoot_interval()
    local scene_name = get_current_scene_name()
    if scene_name == "level_02" then
        return level2ShootInterval
    end
    return baseShootInterval
end

function enemy1_Attack(enemyX, enemyY)
	enemyBulletsFactory(enemyX, enemyY)
end

function updateEnemy1Position(dt, enemyX, enemyY)
    shootTimer = shootTimer + dt
    local interval = get_shoot_interval()
    if shootTimer >= interval then
        enemy1_Attack(enemyX, enemyY)
        shootTimer = 0
    end
end
