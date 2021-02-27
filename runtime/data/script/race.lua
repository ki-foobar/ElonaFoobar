local data = require("data")

local exports = {}

function exports.primary_races()
   return data.instances("core.race")
      :filter(function(_, race) return not race.is_extra end)
end

function exports.extra_races()
   return data.instances("core.race")
      :filter(function(_, race) return race.is_extra end)
end

return exports
