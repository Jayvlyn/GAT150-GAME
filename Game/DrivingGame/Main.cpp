#include "Core/Core.h"

#include "Renderer/Renderer.h"

#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"

#include "Framework/Framework.h"

#include "Player.h"
#include "Enemy.h"

#include "DrivingGame.h"

#include "Core/Vector2.h"

#include "Physics/PhysicsSystem.h"

#include <thread>
#include <vector>
#include <iostream>
#include <memory>
#include <cassert>
#include <array>
#include <map>
#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
    kiko::MemoryTracker::Initialize();
    kiko::seedRandom((unsigned int)time(nullptr));
    kiko::setFilePath("assets");

    // Initialize engine
    kiko::g_renderer.Initialize();
    kiko::g_renderer.CreateWindow("TIME DRIFTER", 1600, 900);

    kiko::g_inputSystem.Initialize();
    kiko::g_audioSystem.Initialize();
    kiko::PhysicsSystem::Instance().Initialize();

    // Create Game
    unique_ptr<DrivingGame> game = make_unique<DrivingGame>();
    game->Init();

    kiko::res_t<kiko::Texture> car1 = GET_RESOURCE(kiko::Texture, "Car1.png", kiko::g_renderer);

    kiko::res_t<kiko::Texture> car2 = GET_RESOURCE(kiko::Texture, "Car2.png", kiko::g_renderer);
    kiko::res_t<kiko::Texture> car3 = GET_RESOURCE(kiko::Texture, "Car3.png", kiko::g_renderer);
    kiko::res_t<kiko::Texture> car4 = GET_RESOURCE(kiko::Texture, "Car4.png", kiko::g_renderer);

    bool quit = false;
    while (!quit) 
    {
        kiko::g_time.Tick();

        kiko::g_audioSystem.Update();
        kiko::g_inputSystem.Update();
        kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
        kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

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

