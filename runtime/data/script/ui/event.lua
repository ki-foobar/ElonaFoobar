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

local Key = enums.new_enum {
   UNKNOWN = 0,
   UP = 1,
   DOWN = 2,
   ENTER = 3,
}
exports.Key = Key

local EVENT_HANDLER_NAMES = {
   [EventKind.QUIT] = "on_quit",
   [EventKind.APP_TERMINATING] = "on_app_terminating",
   [EventKind.APP_LOW_MEMORY] = "on_app_low_memory",
   [EventKind.APP_WILL_ENTER_BACKGROUND] = "on_app_will_enter_background",
   [EventKind.APP_DID_ENTER_BACKGROUND] = "on_app_did_enter_background",
   [EventKind.APP_WILL_ENTER_FOREGROUND] = "on_app_will_enter_foreground",
   [EventKind.APP_DID_ENTER_FOREGROUND] = "on_app_did_enter_foreground",
   [EventKind.WINDOW] = "on_window",
   [EventKind.KEY_DOWN] = "on_key_down",
   [EventKind.KEY_UP] = "on_key_up",
   [EventKind.TEXT_EDITING] = "on_text_editing",
   [EventKind.TEXT_INPUT] = "on_text_input",
   [EventKind.MOUSE_MOTION] = "on_mouse_motion",
   [EventKind.MOUSE_BUTTON_DOWN] = "on_mouse_button_down",
   [EventKind.MOUSE_BUTTON_UP] = "on_mouse_button_up",
   [EventKind.MOUSE_WHEEL] = "on_mouse_wheel",
   [EventKind.JOY_AXIS_MOTION] = "on_joy_axis_motion",
   [EventKind.JOY_BALL_MOTION] = "on_joy_ball_motion",
   [EventKind.JOY_HAT_MOTION] = "on_joy_hat_motion",
   [EventKind.JOY_BUTTON_DOWN] = "on_joy_button_down",
   [EventKind.JOY_BUTTON_UP] = "on_joy_button_up",
   [EventKind.JOY_DEVICE_ADDED] = "on_joy_device_added",
   [EventKind.JOY_DEVICE_REMOVED] = "on_joy_device_removed",
   [EventKind.CONTROLLER_AXIS_MOTION] = "on_controller_axis_motion",
   [EventKind.CONTROLLER_BUTTON_DOWN] = "on_controller_button_down",
   [EventKind.CONTROLLER_BUTTON_UP] = "on_controller_button_up",
   [EventKind.CONTROLLER_DEVICE_ADDED] = "on_controller_device_added",
   [EventKind.CONTROLLER_DEVICE_REMOVED] = "on_controller_device_removed",
   [EventKind.CONTROLLER_DEVICE_REMAPPED] = "on_controller_device_remapped",
   [EventKind.FINGER_DOWN] = "on_finger_down",
   [EventKind.FINGER_UP] = "on_finger_up",
   [EventKind.FINGER_MOTION] = "on_finger_motion",
   [EventKind.DOLLAR_GESTURE] = "on_dollar_gesture",
   [EventKind.DOLLAR_RECORD] = "on_dollar_record",
   [EventKind.MULTI_GESTURE] = "on_multi_gesture",
   [EventKind.CLIPBOARD_UPDATE] = "on_clipboard_update",
   [EventKind.DROP_FILE] = "on_drop_file",
   [EventKind.DROP_TEXT] = "on_drop_text",
   [EventKind.DROP_BEGIN] = "on_drop_begin",
   [EventKind.DROP_COMPLETE] = "on_drop_complete",
   [EventKind.AUDIO_DEVICE_ADDED] = "on_audio_device_added",
   [EventKind.AUDIO_DEVICE_REMOVED] = "on_audio_device_removed",
   [EventKind.RENDER_TARGETS_RESET] = "on_render_targets_reset",
   [EventKind.RENDER_DEVICE_RESET] = "on_render_device_reset",
   [EventKind.USER] = "on_user",
   [EventKind.UNKNOWN] = "on_unknown",
}

function exports.update(ui_layers)
   local keys = {}

   local event = __APP:poll_event()
   while event do
      local event_kind = event:kind()
      local handler_name = EVENT_HANDLER_NAMES[event_kind]

      local was_handler_called = false
      for _, layer in ipairs(ui_layers) do
         local handler = layer[handler_name]
         if handler then
            handler(layer, event)
            was_handler_called = true
            break
         end
      end
      if event_kind == EventKind.QUIT and not was_handler_called then
         return "quit"
      end

      event = __APP:poll_event()
   end
end

return exports
