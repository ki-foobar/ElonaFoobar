local graphics = require("graphics")
local ui = require("ui")

local exports = {}

local SCENES = {}
local CURRENT_SCENE = nil

function exports.push_scene(scene)
   SCENES[#SCENES + 1] = scene
   CURRENT_SCENE = scene
end

function exports.start(initial_state)
   local app = __APP

   local update_thread = coroutine.create(function()
      while true do
         for _, layer in ipairs(CURRENT_SCENE.ui_layers) do
            if layer.update then
               layer:update()
            end
         end
         coroutine.yield()
      end
   end)

   local draw_thread = coroutine.create(function()
      while true do
         for _, layer in ipairs(CURRENT_SCENE.ui_layers) do
            graphics.reset_context()
            ui.clear()
            if layer.draw then
               layer:draw()
            end
            ui.render()
         end
         coroutine.yield()
      end
   end)

   if initial_state.on_shown then
      initial_state:on_shown()
   end
   exports.push_scene({
      ui_layers = {initial_state},
   })
   while true do
      app:update()
      local result = ui.event.update(CURRENT_SCENE.ui_layers)
      if result == "quit" then
         break
      end

      local ok, result = coroutine.resume(update_thread)
      if not ok then
         error(result)
      end

      local ok, result = coroutine.resume(draw_thread)
      if not ok then
         error(result)
      end

      -- TODO
      if __APP:is_headless() then
         return "exit"
      end
   end
end

return exports
