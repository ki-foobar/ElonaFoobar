local graphics = require("graphics")
local ui = require("ui")

local function main_loop(initial_state)
   local app = __APP

   local ui_layers = {}

   local update_thread = coroutine.create(function(event)
      while true do
         for _, layer in ipairs(ui_layers) do
            if layer.update then
               layer:update(event)
            end
         end
         event = coroutine.yield()
      end
   end)

   local draw_thread = coroutine.create(function()
      while true do
         for _, layer in ipairs(ui_layers) do
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
   ui_layers[#ui_layers + 1] = initial_state
   while true do
      app:update()
      local event = ui.event.update()
      if event == "quit" then
         break
      end

      local ok, result = coroutine.resume(update_thread, event)
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

return main_loop
