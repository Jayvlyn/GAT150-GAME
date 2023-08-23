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

void print(int i)
{
    cout << i << endl;
}

int add(int i1, int i2)
{
    return i1 + i2;
}

int sub(int i1, int i2)
{
    return i1 - i2;
}

class A
{
public:
    int add(int i1, int i2)
    {
        return i1 + i2;
    }
};

union Data
{
    int i;
    bool b;
    char c[6];
};



int main(int argc, char* argv[])
{
    Data data;
    data.b = true;
    cout << data.b << endl;

    void (*func_ptr)(int) = &print;
    func_ptr(5);

    int (*op_ptr)(int, int);
    op_ptr = add;

    cout << op_ptr(4, 4) << endl;

    std::function<int(int, int)> op;
    op = add;
    cout << op(5, 6) << endl;

    A a;
    op = std::bind(&A::add, &a, std::placeholders::_1, std::placeholders::_2);
    cout << op(6, 6) << endl;


    kiko::MemoryTracker::Initialize();
    kiko::seedRandom((unsigned int)time(nullptr));
    kiko::setFilePath("assets");

    rapidjson::Document document;
    kiko::Json::Load("json.txt", document);

    int i1;
    kiko::Json::Read(document, "integer1", i1);
    std::cout << i1 << std::endl;

    int i2;
    kiko::Json::Read(document, "integer2", i2);
    std::cout << i2 << std::endl;

    std::string str;
    kiko::Json::Read(document, "string", str);
    std::cout << str << std::endl;

    bool b;
    kiko::Json::Read(document, "boolean", b);
    std::cout << b << std::endl;

    float f;
    kiko::Json::Read(document, "float", f);
    std::cout << f << std::endl;

    kiko::vec2 v2;
    kiko::Json::Read(document, "vector2", v2);
    std::cout << v2 << std::endl;


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

