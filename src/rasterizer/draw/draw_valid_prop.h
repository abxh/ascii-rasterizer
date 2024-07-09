#pragma once

#include "math/vec.h"

#include "rasterizer/ascii_table_type.h"
#include "rasterizer/draw.h"

#include <stdbool.h>
#include <stddef.h>

// valid prop check
// ------------------------------------------------------------------------------------------------------------

static inline bool valid_color_check(const size_t n, const color_type color[n]) {
    bool is_valid = true;
    for (size_t i = 0; i < n; i++) {
        is_valid &= vec3_in_range(color[i].vec3, (vec3_type){.array={0.f, 0.f, 0.f}}, (vec3_type){.array={1.f, 1.f, 1.f}});
    }
    return is_valid;
}

static inline bool valid_prop_check(const ascii_table_type* table_p, const size_t n, const prop_type prop[n]) {
    bool is_valid = true;
    for (size_t i = 0; i < n; i++) {
        const char c = prop[i].ascii_char;
        is_valid &= table_p->ascii_to_index[(int)c] != -1;
        is_valid &= vec3_in_range(prop[i].color.vec3, (vec3_type){.array={0.f, 0.f, 0.f}}, (vec3_type){.array={1.f, 1.f, 1.f}});
    }
    return is_valid;
}