--+++++++++++++++++++++++++++++++++++++++++++++++++++ LUA script ++++++++++++++++++++++++++++++++++++++++++++++++

players = {}
players[0] = {Title = "Squire", Name = "Ciaran", Family = "Wirral", Level = 20}
players[1] = {Title = "Lord", Name = "Diego", Family = "Brazil", Level = 50}

--lua function
function sum(a,b)
	return a + b
end

function GetPlayer(n)
	return players[n]
end
