#pragma once

#include "scene.h"
#include "engine_cmd_desc.h"

struct engine_handle_type;

void engine_run(const scene_type default_scene);

set_cmd_desc* engine_get_cmd_hashtable(struct engine_handle_type* handle_p);

void engine_set_next_scene(struct engine_handle_type* handle_p, scene_type next_scene);
