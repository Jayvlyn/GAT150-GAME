#include "Renderer.h"
#include <SDL2-2.28.1/include/SDL_ttf.h>

namespace kiko {

    Renderer g_renderer;

    bool Renderer::Initialize() 
    {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();

        return true;
    }

    void Renderer::Shutdown()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        TTF_Quit();
    }

    void Renderer::CreateWindow(const std::string& title, int width, int height) 
    {
        m_width = width;
        m_height = height;

        m_window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

        //SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    
    }

    void Renderer::BeginFrame() {

        SDL_RenderClear(m_renderer);

    }

    void Renderer::EndFrame() {

        SDL_RenderPresent(m_renderer);

    }

    void Renderer::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

    }

    void Renderer::DrawLine(int x1, int y1, int x2, int y2) {

        SDL_RenderDrawLine(m_renderer, (int)x1, (int)y1, (int)x2, (int)y2);

    }

    void Renderer::DrawLine(float x1, float y1, float x2, float y2) {

        SDL_RenderDrawLine(m_renderer, (int)x1, (int)y1, (int)x2, (int)y2);

    }

    void Renderer::DrawLine(vec2& p1, vec2& p2) {

        SDL_RenderDrawLine(m_renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);

    }

    void Renderer::DrawPoint(int x, int y) {

        SDL_RenderDrawPoint(m_renderer, (int)x, (int)y);

    }

    void Renderer::DrawPoint(float x, float y) {

        SDL_RenderDrawPoint(m_renderer, (int)x, (int)y);

    }

    void Renderer::DrawPoint(vec2& p) {

        SDL_RenderDrawPoint(m_renderer, (int)p.x, (int)p.y);

    }

}
