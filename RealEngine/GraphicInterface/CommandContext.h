#pragma once 
#include <string_view>
#include <queue>

#include "D3D12ThirdPart.h"
#include "RefCounting.h"

class RRHIBuffer;
class RCommandContext;

class RCommandContextManger
{
public:
    RCommandContextManger() {};

    RCommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE Type);
    void FreeContext(RCommandContext* InContext);
    void DestroyAllContexts();
private:
    // 四种CommmandList
    std::vector<std::unique_ptr<RCommandContext>> m_ContextPool[4];
    std::queue<RCommandContext*> m_AvailableContexts[4];
    std::mutex m_ContextAllocationMutex;
};

// 
class RCommandContext
{
    friend class RCommandContextManger;
private:
    RCommandContext(D3D12_COMMAND_LIST_TYPE InType): 
        m_Type(InType), 
        m_CommandList(nullptr),
        m_CurrentAllocator(nullptr),
        m_Device(nullptr)
    {};
    void Reset();
public:
    RCommandContext(const RCommandContext&) = delete;
    RCommandContext& operator=(const RCommandContext&) = delete;

    ~RCommandContext();

    void Initialize();

    void SetID(const std::string& ID) { m_ID = ID; }; 

    uint64_t Flush(bool WaitForCompletion = false);
    uint64_t Finish(bool WaitForCompletion = false);

    D3D12_COMMAND_LIST_TYPE GetContextType()const { return m_Type; };

    RRHIBuffer* CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName);
private:
    D3D12_HEAP_PROPERTIES GetUploadBufferHeapProps() const;
    D3D12_RESOURCE_DESC GetUploadBufferResourceDesc(uint32_t BufferSize) const;

    // Resource
    void TransitionResource(RRHIBuffer* Buffer, D3D12_RESOURCE_STATES NewState, bool FlushImmediate);

private:
    D3D12_COMMAND_LIST_TYPE m_Type;
    ID3D12GraphicsCommandList* m_CommandList;
    ID3D12CommandAllocator* m_CurrentAllocator; 
    ID3D12Device* m_Device;

    std::string m_ID;
};