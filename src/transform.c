#include <math.h>

#include "transform.h"

// ----------------------------------------------------------------------------
// int math

int signof_int(int expr) {
    return (expr > 0) - (expr < 0);
}

int abs_int(int expr) {
    return (expr > 0) ? expr : -expr;
}

int max_int(int x, int y) {
    return (x > y) ? x : y;
}

int min_int(int x, int y) {
    return (x < y) ? x : y;
}

int clamp_int(int x, int min, int max) {
    return min_int(max_int(x, min), max);
}

// ----------------------------------------------------------------------------
// float math

static const float float_delta = 0.001f;

int round_float_to_int(float expr) {
    return (expr > (floorf(expr) + 0.5f + float_delta)) ? ceilf(expr) : floorf(expr);
}

int cmp_float(float x, float y) {
    return (x > y + float_delta) - (x + float_delta < y);
}

float max_float(float x, float y) {
    return (x > y + float_delta) ? x : y;
}

float min_float(float x, float y) {
    return (x + float_delta < y) ? x : y;
}

float clamp_float(float x, float min, float max) {
    return min_float(max_float(x, min), max);
}

// ----------------------------------------------------------------------------
// angle math

float to_angle_in_radians(float angle_deg) {
    return angle_deg * M_PI / 180.f;
}

float to_angle_in_degrees(float angle_rad) {
    return angle_rad * 180.f / M_PI;
}

float to_fov_constant_from_radians(float angle_rad) {
    return 1.f / tanf(angle_rad / 2.f);
}

float to_fov_constant_from_degrees(float angle_deg) {
    return to_fov_constant_from_radians(to_angle_in_radians(angle_deg));
}

// ----------------------------------------------------------------------------
// vec2 math

vec2 sum_vec2(vec2 a, vec2 b) {
    return (vec2){.x = a.x + b.x, .y = a.y + b.y};
}

vec2 scaled_vec2(vec2 vec, float scalar) {
    return (vec2){.x = scalar * vec.x, .y = scalar * vec.y};
}

vec2 rotate_around_origo_vec2(vec2 vec, float angle_rad) {
    return (vec2){.x = vec.x * cosf(angle_rad) - vec.y * sinf(angle_rad), .y = vec.x * sinf(angle_rad) + vec.y * cosf(angle_rad)};
}

// ----------------------------------------------------------------------------
// vec3 math

vec3 sum_vec3(vec3 a, vec3 b) {
    return (vec3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

vec3 scaled_vec3(vec3 vec, float scalar) {
    return (vec3){.x = scalar * vec.x, .y = scalar * vec.y, .z = scalar * vec.z};
}

vec2 vec3_projected_as_vec2(vec3 vec, float fov_const) {
    return scaled_vec2((vec2){.x = vec.x / vec.z, vec.y / vec.z}, 1.f / fov_const);
}

vec3 rotate_around_x_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x, .y = vec.y * cosf(angle_rad) - vec.z * sinf(angle_rad), .z = vec.y * sinf(angle_rad) + vec.z * cosf(angle_rad)};
}

vec3 rotate_around_y_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x * cosf(angle_rad) + vec.z * sinf(angle_rad), .y = vec.y, .z = vec.z * cosf(angle_rad) - vec.x * sinf(angle_rad)};
}

vec3 rotate_around_z_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x * cosf(angle_rad) - vec.y * sinf(angle_rad), .y = vec.x * sinf(angle_rad) + vec.y * cosf(angle_rad), .z = vec.z};
}
