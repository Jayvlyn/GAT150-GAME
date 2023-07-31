#include "Core/Core.h"

#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Renderer/Font.h"    
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"

#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"

#include "Framework/Scene.h"

#include "Player.h"
#include "Enemy.h"

#include "DrivingGame.h"

#include <thread>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char* argv[]) 
{
    //class DrivingGame;
    kiko::MemoryTracker::Initialize();
    kiko::seedRandom((unsigned int)time(nullptr));
    kiko::setFilePath("assets");


    // Initialize engine
    kiko::g_renderer.Initialize();
    kiko::g_renderer.CreateWindow("TIME DRIFTER", 1600, 900);

    kiko::g_inputSystem.Initialize();
    kiko::g_audioSystem.Initialize();

    // Create Game
    unique_ptr<DrivingGame> game = make_unique<DrivingGame>();
    game->Init();
        
    bool quit = false;
    while (!quit) 
    {
        kiko::g_time.Tick();

        kiko::g_audioSystem.Update();
        kiko::g_inputSystem.Update();
        kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());

        if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) quit = true;

        game->Update(kiko::g_time.GetDeltaTime());

        kiko::g_renderer.SetColor(0, 0, 0, 0);
        kiko::g_renderer.BeginFrame();

        kiko::g_renderer.SetColor(255, 255, 255, 255);
        
        kiko::g_renderer.SetColor(255, 0, 0, 255);

        game->Draw(kiko::g_renderer);

        kiko::g_particleSystem.Draw(kiko::g_renderer);

        kiko::g_renderer.EndFrame();

    }

    return 0;
}