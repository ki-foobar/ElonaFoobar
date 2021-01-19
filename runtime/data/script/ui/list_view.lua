local class = prelude.class

local ui = require("ui")
local Cursor = ui.Cursor
local Key = require("ui.event").Key

local ListView = class("core.ui.ListView")

function ListView:__init(opts)
   self.items = opts.items or {}
   self.cursor = opts.cursor or Cursor.new(1, 1)
end

function ListView:prev()
   self.cursor.position = self.cursor.position - 1
   if self.cursor.position < 1 then
      self.cursor.position = #self.items
   end
end

function ListView:next()
   self.cursor.position = self.cursor.position + 1
   if #self.items < self.cursor.position then
      self.cursor.position = 1
   end
end

function ListView:on_key_down(event)
   local key = event:key()
   if key == Key.UP then
      self:prev()
   elseif key == Key.DOWN then
      self:next()
   elseif key == Key.ENTER then
      self:on_item_selected(self.items[self.cursor.position])
   end
end

function ListView:update()
end

function ListView:draw()
   for index, item in ipairs(self.items) do
      self:draw_item(index, item)
   end
end

function ListView:draw_item()
end

function ListView:on_item_selected(selected_item)
end

return ListView
