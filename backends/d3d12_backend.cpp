#include "d3d12_backend.hpp"
#include <stdexcept>
#include <cstring>

ID3D12Device* D3D12Backend::s_device = nullptr;
std::vector<D3D12BufferRecord> D3D12Backend::s_buffers;

void D3D12Backend::configure(ID3D12Device* device) {
    s_device = device;
}

void D3D12Backend::init() {
    // No-op; keep for symmetry with other backends
}

GpuBufferHandle D3D12Backend::create_buffer(std::size_t sizeBytes) {
    if (!s_device) throw std::runtime_error("D3D12Backend::configure not called");

    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeBytes;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* resource = nullptr;
    HRESULT hr = s_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&resource)
    );

    if (FAILED(hr) || !resource)
        throw std::runtime_error("Failed to create D3D12 buffer");

    std::size_t id = s_buffers.size();
    s_buffers.push_back(D3D12BufferRecord{
        .resource = resource,
        .size = sizeBytes
    });

    return GpuBufferHandle{ .id = id, .generation = 1 };
}

void D3D12Backend::destroy_buffer(GpuBufferHandle handle) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (rec.resource) {
        rec.resource->Release();
        rec.resource = nullptr;
        rec.size = 0;
    }
}

void D3D12Backend::upload(GpuBufferHandle handle,
                          const float* data,
                          std::size_t count) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (!rec.resource) return;

    void* mapped = nullptr;
    D3D12_RANGE readRange{ 0, 0 };
    HRESULT hr = rec.resource->Map(0, &readRange, &mapped);
    if (FAILED(hr) || !mapped) return;

    std::memcpy(mapped, data, count * sizeof(float));

    D3D12_RANGE writtenRange{ 0, count * sizeof(float) };
    rec.resource->Unmap(0, &writtenRange);
}

void D3D12Backend::set_debug_name(GpuBufferHandle handle,
                                  const char* name) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (!rec.resource) return;

    rec.resource->SetName(std::wstring{name, name + std::strlen(name)}.c_str());
}

ID3D12Resource* D3D12Backend::get_resource(GpuBufferHandle handle) {
    if (handle.id >= s_buffers.size()) return nullptr;
    return s_buffers[handle.id].resource;
}