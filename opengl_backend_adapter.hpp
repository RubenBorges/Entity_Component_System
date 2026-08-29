#pragma once
#include "IGpuBackend.hpp"
#include "opengl_backend.hpp"

inline IGpuBackend OpenGLBackendAPI {
    .name = "OpenGL",
    .init = &OpenGLBackend::init,
    .create_buffer = &OpenGLBackend::create_buffer,
    .destroy_buffer = &OpenGLBackend::destroy_buffer,
    .upload = &OpenGLBackend::upload,
    .set_debug_name = [](GpuBufferHandle h, const char* name) {OpenGLBackend::set_debug_name(h, std::string{name});} //std::string t> const char* wrapper
    //.set_debug_name = &OpenGLBackend::set_debug_name
};