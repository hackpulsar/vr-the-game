#define OLC_PGE_APPLICATION

#include "SplashScreen.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "EndGameState.h"

class Game : public olc::PixelGameEngine
{
private:
	engine::State* m_State = nullptr; // current state in game

public:
	Game()
	{
		sAppName = "VR";
	}

	~Game()
	{
		delete this->m_State;
	}

public:
	bool OnUserCreate() override
	{
		this->ChangeState(new engine::SplashScreen(this));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{ // update stuff here
		if (this->m_State != nullptr)
		{
			if (this->m_State->isActive() == false)
			{
				// WARNING: SHIT CODE

				engine::SplashScreen* s = dynamic_cast<engine::SplashScreen*>(m_State);
				engine::MainMenuState* m = dynamic_cast<engine::MainMenuState*>(m_State);
				engine::GameState* g = dynamic_cast<engine::GameState*>(m_State);
				engine::EndGameState* e = dynamic_cast<engine::EndGameState*>(m_State);

				if (s != nullptr || e != nullptr)
				{
					this->ChangeState(new engine::MainMenuState(this));
				}

				if (m != nullptr)
				{
					this->ChangeState(new engine::GameState(this));
				}

				if (g != nullptr)
				{
					this->ChangeState(new engine::EndGameState(this, g->getGameResult()));
				}
			}
		}

		this->m_State->handleInput(); // handle input
		this->m_State->update(fElapsedTime); // update

		this->Render();

		return true;
	}

	void Render()
	{ // render stuff here
		this->m_State->render();
	}

	void ChangeState(engine::State* newState)
	{
		delete this->m_State;
		this->m_State = newState;
	}

};

int main()
{
	Game demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}

