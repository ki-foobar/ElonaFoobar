local enums = require("enums")

local exports = {}

local EventKind = enums.new_enum {
   QUIT = 0,
   APP_TERMINATING = 1,
   APP_LOW_MEMORY = 2,
   APP_WILL_ENTER_BACKGROUND = 3,
   APP_DID_ENTER_BACKGROUND = 4,
   APP_WILL_ENTER_FOREGROUND = 5,
   APP_DID_ENTER_FOREGROUND = 6,
   WINDOW = 7,
   KEY_DOWN = 8,
   KEY_UP = 9,
   TEXT_EDITING = 10,
   TEXT_INPUT = 11,
   MOUSE_MOTION = 12,
   MOUSE_BUTTON_DOWN = 13,
   MOUSE_BUTTON_UP = 14,
   MOUSE_WHEEL = 15,
   JOY_AXIS_MOTION = 16,
   JOY_BALL_MOTION = 17,
   JOY_HAT_MOTION = 18,
   JOY_BUTTON_DOWN = 19,
   JOY_BUTTON_UP = 20,
   JOY_DEVICE_ADDED = 21,
   JOY_DEVICE_REMOVED = 22,
   CONTROLLER_AXIS_MOTION = 23,
   CONTROLLER_BUTTON_DOWN = 24,
   CONTROLLER_BUTTON_UP = 25,
   CONTROLLER_DEVICE_ADDED = 26,
   CONTROLLER_DEVICE_REMOVED = 27,
   CONTROLLER_DEVICE_REMAPPED = 28,
   FINGER_DOWN = 29,
   FINGER_UP = 30,
   FINGER_MOTION = 31,
   DOLLAR_GESTURE = 32,
   DOLLAR_RECORD = 33,
   MULTI_GESTURE = 34,
   CLIPBOARD_UPDATE = 35,
   DROP_FILE = 36,
   DROP_TEXT = 37,
   DROP_BEGIN = 38,
   DROP_COMPLETE = 39,
   AUDIO_DEVICE_ADDED = 40,
   AUDIO_DEVICE_REMOVED = 41,
   RENDER_TARGETS_RESET = 42,
   RENDER_DEVICE_RESET = 43,
   USER = 44,
   UNKNOWN = 45,
}
exports.EventKind = EventKind

function exports.update()
   local event = __APP:poll_event()
   while event do
      local event_kind = event:kind()
      if event_kind == EventKind.QUIT then
         return "quit"
      elseif event_kind == EventKind.KEY_DOWN then
         return "key_down"
      elseif event_kind == EventKind.KEY_UP then
         return "key_up"
      end
      event = __APP:poll_event()
   end
end

return exports
