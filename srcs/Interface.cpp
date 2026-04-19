#include <d3d12.h>
#include <dxgi1_4.h>

#include "../vendor/kiero/kiero.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/backends/imgui_impl_win32.h"
#include "../vendor/imgui/backends/imgui_impl_dx12.h"

#include "../includes/Interface.h"
#include "../includes/Menu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- Hook typedefs ---
typedef long(__stdcall* Present)(IDXGISwapChain3*, UINT, UINT);
typedef void(__stdcall* ExecuteCommandLists)(ID3D12CommandQueue*, UINT, ID3D12CommandList* const*);

static Present             oPresent             = nullptr;
static ExecuteCommandLists oExecuteCommandLists = nullptr;

// --- Per-frame resources ---
struct FrameContext
{
    ID3D12CommandAllocator*   commandAllocator = nullptr;
    ID3D12Resource*           renderTarget     = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle      = {};
};

static FrameContext*              g_frameContexts = nullptr;
static UINT                       g_bufferCount   = 0;

// --- Shared DX12 resources ---
static ID3D12CommandQueue*        g_commandQueue  = nullptr;
static ID3D12DescriptorHeap*      g_srvHeap       = nullptr;
static ID3D12DescriptorHeap*      g_rtvHeap       = nullptr;
static ID3D12GraphicsCommandList* g_commandList   = nullptr;

// --- SRV descriptor bump allocator (ImGui 1.92+ requires callbacks) ---
static constexpr UINT SRV_HEAP_SIZE = 64;

struct SrvAllocator
{
    D3D12_CPU_DESCRIPTOR_HANDLE cpuBase  = {};
    D3D12_GPU_DESCRIPTOR_HANDLE gpuBase  = {};
    UINT                        stride   = 0;
    UINT                        next     = 0;
} g_srvAlloc;

static void AllocSrv(ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* outCpu, D3D12_GPU_DESCRIPTOR_HANDLE* outGpu)
{
    IM_ASSERT(g_srvAlloc.next < SRV_HEAP_SIZE);
    outCpu->ptr = g_srvAlloc.cpuBase.ptr + (SIZE_T)g_srvAlloc.next * g_srvAlloc.stride;
    outGpu->ptr = g_srvAlloc.gpuBase.ptr + (UINT64)g_srvAlloc.next * g_srvAlloc.stride;
    g_srvAlloc.next++;
}

static void FreeSrv(ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE)
{
    // Bump allocator — no per-entry free needed for this use case
}

// --- Window ---
static HWND    g_hwnd      = nullptr;
static WNDPROC g_oWndProc  = nullptr;

static bool          g_initialized  = false;
static volatile bool g_shuttingDown = false;

// -------------------------------------------------------------------------
// WndProc hook — forwards input to ImGui, then to the original proc
// -------------------------------------------------------------------------
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    return CallWindowProc(g_oWndProc, hWnd, msg, wParam, lParam);
}

// -------------------------------------------------------------------------
// ExecuteCommandLists hook — captures the real command queue (index 54)
// -------------------------------------------------------------------------
static void __stdcall hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT numCommandLists, ID3D12CommandList* const* ppCommandLists)
{
    if (!g_commandQueue)
        g_commandQueue = queue;

    oExecuteCommandLists(queue, numCommandLists, ppCommandLists);
}

// -------------------------------------------------------------------------
// One-time initialisation called from the Present hook
// -------------------------------------------------------------------------
static bool InitImGui(IDXGISwapChain3* swapChain)
{
    DXGI_SWAP_CHAIN_DESC desc = {};
    swapChain->GetDesc(&desc);
    g_hwnd        = desc.OutputWindow;
    g_bufferCount = desc.BufferCount;

    ID3D12Device* device = nullptr;
    if (swapChain->GetDevice(__uuidof(ID3D12Device), (void**)&device) < 0)
        return false;

    // SRV heap — 64 slots, callbacks feed ImGui 1.92's allocator API
    D3D12_DESCRIPTOR_HEAP_DESC srvDesc = {};
    srvDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDesc.NumDescriptors = SRV_HEAP_SIZE;
    srvDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (device->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(&g_srvHeap)) < 0)
        return false;

    g_srvAlloc.cpuBase = g_srvHeap->GetCPUDescriptorHandleForHeapStart();
    g_srvAlloc.gpuBase = g_srvHeap->GetGPUDescriptorHandleForHeapStart();
    g_srvAlloc.stride  = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    g_srvAlloc.next    = 0;

    // RTV heap — one descriptor per swap-chain buffer
    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
    rtvDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDesc.NumDescriptors = g_bufferCount;
    rtvDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&g_rtvHeap)) < 0)
        return false;

    g_frameContexts = new FrameContext[g_bufferCount];

    UINT rtvStep = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < g_bufferCount; i++)
    {
        if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(&g_frameContexts[i].commandAllocator)) < 0)
            return false;

        if (swapChain->GetBuffer(i, IID_PPV_ARGS(&g_frameContexts[i].renderTarget)) < 0)
            return false;

        device->CreateRenderTargetView(g_frameContexts[i].renderTarget, nullptr, rtvHandle);
        g_frameContexts[i].rtvHandle = rtvHandle;
        rtvHandle.ptr += rtvStep;
    }

    if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
            g_frameContexts[0].commandAllocator, nullptr, IID_PPV_ARGS(&g_commandList)) < 0)
        return false;
    g_commandList->Close();

    // ImGui context + backends (ImGui 1.92+ API)
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplWin32_Init(g_hwnd);

    ImGui_ImplDX12_InitInfo dx12Info = {};
    dx12Info.Device             = device;
    dx12Info.CommandQueue       = g_commandQueue;   // required for texture uploads in 1.92
    dx12Info.NumFramesInFlight  = (int)g_bufferCount;
    dx12Info.RTVFormat          = DXGI_FORMAT_R8G8B8A8_UNORM;
    dx12Info.DSVFormat          = DXGI_FORMAT_UNKNOWN;
    dx12Info.SrvDescriptorHeap  = g_srvHeap;
    dx12Info.SrvDescriptorAllocFn = AllocSrv;
    dx12Info.SrvDescriptorFreeFn  = FreeSrv;
    ImGui_ImplDX12_Init(&dx12Info);

    g_oWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

    device->Release();
    return true;
}

// -------------------------------------------------------------------------
// Present hook — renders the ImGui overlay each frame (index 140)
// -------------------------------------------------------------------------
static long __stdcall hkPresent(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags)
{
    if (g_shuttingDown)
        return oPresent(swapChain, syncInterval, flags);

    // Wait until ExecuteCommandLists has given us a real command queue
    if (!g_commandQueue)
        return oPresent(swapChain, syncInterval, flags);

    if (!g_initialized)
    {
        if (!InitImGui(swapChain))
            return oPresent(swapChain, syncInterval, flags);
        g_initialized = true;
    }

    // --- New ImGui frame ---
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Menu::Render();

    ImGui::Render();

    // --- Record draw commands ---
    UINT frameIndex = swapChain->GetCurrentBackBufferIndex();
    FrameContext& frame = g_frameContexts[frameIndex];

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource   = frame.renderTarget;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;

    frame.commandAllocator->Reset();
    g_commandList->Reset(frame.commandAllocator, nullptr);
    g_commandList->ResourceBarrier(1, &barrier);
    g_commandList->OMSetRenderTargets(1, &frame.rtvHandle, FALSE, nullptr);
    g_commandList->SetDescriptorHeaps(1, &g_srvHeap);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_commandList);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
    g_commandList->ResourceBarrier(1, &barrier);
    g_commandList->Close();

    g_commandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_commandList);

    return oPresent(swapChain, syncInterval, flags);
}

// -------------------------------------------------------------------------
// Public API
// -------------------------------------------------------------------------
void InitInterface()
{
    if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
    {
        kiero::bind(54,  (void**)&oExecuteCommandLists, hkExecuteCommandLists);
        kiero::bind(140, (void**)&oPresent,             hkPresent);
    }
}

void ShutdownInterface()
{
    // 1. Signal hkPresent to stop rendering immediately
    g_shuttingDown = true;

    // 2. Disable hooks — must happen before freeing any resource they touch
    kiero::shutdown();

    // 3. Wait one frame so any in-progress hkPresent call can finish
    Sleep(100);

    if (!g_initialized)
        return;

    // 4. Restore original WndProc
    if (g_hwnd && g_oWndProc)
        SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)g_oWndProc);

    // 5. Shut down ImGui backends
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // 6. Release DX12 resources
    if (g_commandList)  { g_commandList->Release();  g_commandList = nullptr; }
    if (g_srvHeap)      { g_srvHeap->Release();      g_srvHeap     = nullptr; }
    if (g_rtvHeap)      { g_rtvHeap->Release();      g_rtvHeap     = nullptr; }

    if (g_frameContexts)
    {
        for (UINT i = 0; i < g_bufferCount; i++)
        {
            if (g_frameContexts[i].commandAllocator)
                g_frameContexts[i].commandAllocator->Release();
            if (g_frameContexts[i].renderTarget)
                g_frameContexts[i].renderTarget->Release();
        }
        delete[] g_frameContexts;
        g_frameContexts = nullptr;
    }
}