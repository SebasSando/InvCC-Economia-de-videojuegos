-- Variables globales
player_velocity = 400;

fixed_player_velocity = math.sqrt((player_velocity * player_velocity) / 2);

function update()
	set_velocity(this, 0, 0);
	vel_x = 0;
	vel_y = 0;
	if is_action_activated("up") then
		vel_y = vel_y + -1;
	end
	if is_action_activated("left") then
		vel_x = vel_x + -1;
	end	
	if is_action_activated("down") then
		vel_y = vel_y + 1;
	end	
	if is_action_activated("right") then
		vel_x = vel_x + 1;
	end	

	if vel_x ~= 0 and vel_y ~= 0 then
		vel_x = vel_x * fixed_player_velocity;
		vel_y = vel_y * fixed_player_velocity;
	else
		vel_x = vel_x * player_velocity;
		vel_y = vel_y * player_velocity;
	end

	local center_x, center_y = get_entity_center(this);
	local mouse_x, mouse_y = get_mouse_position();
	local delta_x = mouse_x - center_x;
	local delta_y = mouse_y - center_y;
	if delta_x ~= 0 or delta_y ~= 0 then
		local angle = math.deg(math.atan(delta_y, delta_x)) + 90;
		set_rotation(this, angle);
	end

	set_velocity(this, vel_x, vel_y);

end

local shootTimer = 0
local shootInterval = 0.5

function shootBullet(playerX, playerY)
	bulletFactory(playerX, playerY)
end

function updateBullets(dt, playerX, playerY)
    shootTimer = shootTimer + dt

    if shootTimer >= shootInterval then
        shootBullet(playerX, playerY)
        shootTimer = 0
    end
end
