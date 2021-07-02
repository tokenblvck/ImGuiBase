
#include "marty.h"

static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static D3DPRESENT_PARAMETERS g_d3dpp = {};
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int gui() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr,
                     nullptr, nullptr, _T(" "), nullptr };
    ::RegisterClassEx(&wc);
    marty::gui::hwnd = ::CreateWindow(wc.lpszClassName, _T(" "), (WS_POPUP), 680, 400, 400, 250, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(marty::gui::hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        MessageBox(0, "There was an error when creating the D3D Device, please report this to staff.", " ", MB_OK | MB_ICONINFORMATION);
        exit(1);
    }

    ::ShowWindow(marty::gui::hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(marty::gui::hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(marty::gui::hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    /*Starting icons*/

    // NULL

    /*Starting fonts*/

    const auto default_font = io.Fonts->AddFontDefault();

    io.IniFilename = nullptr;
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    static const ImWchar ranges[] = {
            0xf000,
            0xf976,
            NULL
    };

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    bool startscan = false;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowBorderSize = 0.0f;
        style->Alpha = 1.0f;
        style->ChildBorderSize = 1.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowPadding = ImVec2(0, 0);
        style->WindowRounding = 0.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 3.0f;
        style->ItemSpacing = ImVec2(5, 5);
        style->ItemInnerSpacing = ImVec2(5, 5);
        style->IndentSpacing = 6.0f;
        style->TouchExtraPadding = ImVec2(5, 5);
        style->ScrollbarSize = 6.0f;
        style->ScrollbarRounding = 15.0f;
        style->GrabMinSize = 10.0f;
        style->GrabRounding = 2.0f;
        style->ColumnsMinSpacing = 10.0f;
        style->ButtonTextAlign = ImVec2(0.5, 0.5);
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
        style->AntiAliasedLines = true;
        style->AntiAliasedFill = true;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImColor(240, 240, 240);
        colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25);
        colors[ImGuiCol_Border] = ImColor(25, 25, 25);
        colors[ImGuiCol_FrameBg] = ImColor(42, 42, 42);
        colors[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42);
        colors[ImGuiCol_Button] = ImColor(55, 55, 102);
        colors[ImGuiCol_ButtonHovered] = ImColor(55, 55, 102);
        colors[ImGuiCol_ButtonActive] = ImColor(55, 55, 102);
        colors[ImGuiCol_PlotHistogram] = ImColor(55, 55, 102);
        colors[ImGuiCol_PlotHistogramHovered] = ImColor(55, 55, 102);

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::PushFont(default_font);
            ImGui::Begin(("##main"), nullptr,
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoScrollbar);
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowSize(ImVec2(600, 800));

            ImGui::Text("marty!");

            ImGui::End();
            ImGui::PopFont();
        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f),
            (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0) {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        /* Don't remove this please, thanks */

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(marty::gui::hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

int main() {
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
    std::cout << "i hate niggers - marty (est 1940)" << std::endl;

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    gui();

    return 0;
}

bool CreateDeviceD3D(HWND hWnd) {
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    /* Create the D3DDevice */
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; /* Need to use an explicit format with alpha if needing per-pixel alpha composition. */
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           /* Present with vsync */
    /* g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; */  /* Present without vsync, maximum unthrottled framerate */
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp,
        &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D() {
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
    if (g_pD3D) {
        g_pD3D->Release();
        g_pD3D = nullptr;
    }
}

void ResetDevice() {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}