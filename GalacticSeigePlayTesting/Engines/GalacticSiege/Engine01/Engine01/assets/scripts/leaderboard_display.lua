local last_text = ""

local function get_message()
	local message = get_last_score_message()
	if message == nil or message == "" then
		return "Mejor resultado historico: 0"
	end
	return message
end

function update()
	local current_text = get_message()
	if current_text ~= last_text then
		set_text(this, current_text)
		last_text = current_text
	end
end
