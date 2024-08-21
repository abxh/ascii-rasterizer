#include "engine.h"
#include "engine_sdl_window.h"
#include "engine_timer.h"
#include "math/angle.h"
#include "rasterizer/renderer.h"
#include "rasterizer/screen.h"

#include "sc_map.h"

#include <SDL2/SDL.h>

typedef struct engine_handle_type {
    bool on_running;
    scene_type next_scene;
    struct sc_map_str cmd_map;
    struct renderer_type* renderer_p;
} engine_handle_type;

void engine_quit(engine_handle_type* this) {
    this->on_running = false;
}

struct sc_map_str* engine_get_cmd_text(engine_handle_type* this) {
    return &this->cmd_map;
}

struct renderer_type* engine_get_renderer(engine_handle_type* this) {
    return this->renderer_p;
}

void engine_set_next_scene(engine_handle_type* this, scene_type next_scene) {
    this->next_scene = next_scene;
}

void engine_run(const scene_type default_scene, const size_t n, const char default_ascii_palette[n]) {
    engine_handle_type engine_handle = {.on_running = true, .next_scene = default_scene};
    sc_map_init_str(&engine_handle.cmd_map, 0, 0);
    struct engine_sdl_window_type* engine_sdl_window_p = engine_sdl_window_create(&engine_handle.cmd_map);
    struct screen_type* screen_p = screen_create(stdout);
    struct engine_timer_type* engine_timer_p = engine_timer_create();

    const perspective_proj_prop_type default_perspective_proj_prop = {
        .fovy_rad = ANGLE_DEG_TO_RAD(60), .aspect_ratio = ASPECT_RATIO, .z_near = 0.1f, .z_far = 10.f};
    struct renderer_type* renderer_p = renderer_create(screen_p, n, default_ascii_palette, default_perspective_proj_prop);
    engine_handle.renderer_p = renderer_p;

    scene_type current_scene = default_scene;
    void* scene_p = current_scene.create(&engine_handle);

    SDL_Event current_event;
    while (engine_handle.on_running) {
        while (SDL_PollEvent(&current_event)) {
            if (current_event.type == SDL_QUIT) {
                engine_handle.on_running = false;
            }
            current_scene.on_event(scene_p, &engine_handle, &current_event);
        }
        while (engine_timer_scene_should_update(engine_timer_p)) {
            current_scene.update(scene_p, MS_PER_UPDATE);
            engine_timer_scene_tick(engine_timer_p);
        }
        current_scene.render(scene_p, renderer_p);

        screen_refresh(screen_p);
        screen_clear(screen_p);

        engine_sdl_window_update(engine_sdl_window_p);
        engine_sdl_window_render(engine_sdl_window_p);

        if (!scene_is_equal(current_scene, engine_handle.next_scene)) {
            current_scene.destroy(scene_p, &engine_handle);
            current_scene = engine_handle.next_scene;
            scene_p = current_scene.create(&engine_handle);
        }

        engine_timer_frame_tick(engine_timer_p);
    }

    current_scene.destroy(scene_p, &engine_handle);

    renderer_destroy(renderer_p);
    sc_map_term_str(&engine_handle.cmd_map);
    screen_destroy(screen_p);
    engine_sdl_window_destroy(engine_sdl_window_p);
    engine_timer_destroy(engine_timer_p);
}
