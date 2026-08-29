#include "opengl_backend.hpp"
#include <vector>

// Include your GL loader here.
// Example:
   #include <glad/glad.h>
   #include <GLFW/glfw3.h>
// or:
//   #include <GL/glew.h>
#include <GL/gl.h> // adjust to your setup

static std::vector<GpuBufferRecord> g_buffers;

void OpenGLBackend::init() {
    // Nothing required here if your GL loader is already initialized.
    // Keep this for symmetry / future use.
}

GpuBufferHandle OpenGLBackend::create_buffer(std::size_t sizeBytes) {
    GLBuffer buf = 0;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, nullptr, GL_DYNAMIC_DRAW);

    std::size_t id = g_buffers.size();
    g_buffers.push_back(GpuBufferRecord{
        .glBuffer = buf,
        .size = sizeBytes
    });

    return GpuBufferHandle{ .id = id, .generation = 1 };
}

void OpenGLBackend::destroy_buffer(GpuBufferHandle handle) {
    if (handle.id >= g_buffers.size()) return;

    auto& rec = g_buffers[handle.id];
    if (rec.glBuffer != 0) {
        glDeleteBuffers(1, &rec.glBuffer);
        rec.glBuffer = 0;
        rec.size = 0;
    }
}

void OpenGLBackend::upload(GpuBufferHandle handle,
                           const float* data,
                           std::size_t count) {
    if (handle.id >= g_buffers.size()) return;

    auto& rec = g_buffers[handle.id];
    if (rec.glBuffer == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, rec.glBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), data);
}
/*
void OpenGLBackend::set_debug_name(GpuBufferHandle handle, const char* name) {
    if (handle.id >= g_buffers.size()) return;

    auto& rec = g_buffers[handle.id];
    if (rec.glBuffer == 0) return;

    glObjectLabel(GL_BUFFER, rec.glBuffer,
                  static_cast<GLsizei>(strlen(name)),
                  name);
}
*/
void OpenGLBackend::set_debug_name(GpuBufferHandle handle, const std::string& name) {
    // Optional: GL_KHR_debug
    if (handle.id >= g_buffers.size()) return;

    auto& rec = g_buffers[handle.id];
    if (rec.glBuffer == 0) return;

    glObjectLabel(GL_BUFFER, rec.glBuffer,
                  static_cast<GLsizei>(name.size()),
                  name.c_str());
}

void OpenGLBackend::bind_as_array_buffer(GpuBufferHandle handle) {
    if (handle.id >= g_buffers.size()) return;

    auto& rec = g_buffers[handle.id];
    if (rec.glBuffer == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, rec.glBuffer);
}