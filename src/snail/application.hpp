#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../util/noncopyable.hpp"
#include "../util/scope_guard.hpp"
#include "detail/sdl.hpp"
#include "renderer.hpp"
#include "window.hpp"



namespace elona
{
namespace snail
{

class Application final : public lib::noncopyable
{
public:
    size_t frame() const noexcept
    {
        return _frame;
    }


    int width() const noexcept
    {
        return _width;
    }


    int height() const noexcept
    {
        return _height;
    }

    int physical_width() const noexcept
    {
        return _physical_width;
    }

    int physical_height() const noexcept
    {
        return _physical_height;
    }

    float dpi() const noexcept
    {
        return _dpi;
    }

    const std::string& title() const noexcept
    {
        return _title;
    }


    void set_title(const std::string& title);


    virtual ~Application() override = default;


    static Application& instance();


    void initialize(const std::string& title);

    void quit();



    // NOTE: Do not depend on the order of finalization.
    void register_finalizer(std::function<void()> finalizer);


    Renderer& get_renderer()
    {
        return *_renderer;
    }


    void proc_event();
    void wait(size_t msec);

    bool is_fullscreen()
    {
        return _fullscreen_mode != Window::FullscreenMode::windowed;
    }

    Window::FullscreenMode get_fullscreen_mode()
    {
        return _fullscreen_mode;
    }

    void set_fullscreen_mode(Window::FullscreenMode);

    std::map<std::string, ::SDL_DisplayMode> get_display_modes();

    std::string get_default_display_mode();

    ::SDL_DisplayMode get_display_mode();

    void set_display_mode(const std::string&);
    void set_display_mode(const ::SDL_DisplayMode);


    void set_window_size(int width, int height);

    void set_call_redraw(bool call_redraw)
    {
        _call_redraw = call_redraw;
    }



private:
    std::unique_ptr<detail::SDLCore> _sdl_core;
    std::unique_ptr<detail::SDLTTF> _sdl_ttf;
    std::unique_ptr<detail::SDLImage> _sdl_image;
    std::unique_ptr<detail::SDLMixer> _sdl_mixer;

    int _width = 800;
    int _height = 600;
    int _physical_width = 800;
    int _physical_height = 600;
    float _dpi = 96.0f;
    std::string _title;

    uint32_t _last_wait_time = 0;
    bool _call_redraw = true;

    size_t _frame = 0;
    bool _will_quit = false;
    std::unique_ptr<Window> _window;
    std::unique_ptr<Renderer> _renderer;
    std::vector<lib::scope_guard> _finalizers;
    Window::FullscreenMode _fullscreen_mode = Window::FullscreenMode::windowed;

    Application();

    void initialize_dpi();

    void handle_event(const ::SDL_Event& event);

    void handle_window_event(const ::SDL_WindowEvent& event);
    void on_size_changed(const ::SDL_WindowEvent& event);
};

} // namespace snail
} // namespace elona
