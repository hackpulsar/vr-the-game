#include "MainMenuState.h"

#include "LOG.h"

namespace engine 
{
	MainMenuState::MainMenuState(olc::PixelGameEngine* window)
		: State(window)
	{
		this->m_sprBackground = new olc::Sprite("./assets/background.png");

		this->m_fBackgroundOffset1 = 0.f;
		this->m_fBackgroundOffset2 = m_Window->ScreenWidth();

		this->m_vButtons.push_back(new Button{ {60, 30}, {m_Window->ScreenWidth() / 2 - 30, 120}, "Play", olc::WHITE}); // play button
		this->m_vButtons.push_back(new Button{ {60, 30}, {m_Window->ScreenWidth() / 2 - 30, 160}, "Exit", olc::WHITE}); // exit button
	}

	MainMenuState::~MainMenuState()
	{
		for (int i = 0; i < (int)m_vButtons.size(); ++i)
			delete this->m_vButtons[i];
		this->m_vButtons.clear();
	}
	
	void MainMenuState::handleInput()
	{
		if (this-m_Window->IsFocused())
		{
			if (this->m_Window->GetKey(olc::Key::SPACE).bPressed)
			{
				// disable state
				this->m_bActive = false;
			}

			if (this->m_Window->GetMouse(0).bPressed)
			{
				if (this->m_vButtons[0]->isInBounds(m_Window->GetMousePos()))
				{ // play button pressed
					// this->m_bActive = false;
					this->startFading();
				}
				else if (this->m_vButtons[1]->isInBounds(m_Window->GetMousePos()))
				{ // exit button pressed
					exit(0);
				}
			}
		}
	}

	void MainMenuState::update(float fDeltaTime)
	{
		// update background
		this->m_fBackgroundOffset1 -= 10.f * fDeltaTime;
		this->m_fBackgroundOffset2 -= 10.f * fDeltaTime;


		if ((int)this->m_fBackgroundOffset1 < 0 - m_Window->ScreenWidth())
		{
			this->m_fBackgroundOffset1 = m_Window->ScreenWidth();
		}
		else if ((int)this->m_fBackgroundOffset2 > m_Window->ScreenWidth())
		{
			this->m_fBackgroundOffset1 = 0.f;
		}

		if ((int)this->m_fBackgroundOffset2 < 0 - m_Window->ScreenWidth())
		{
			this->m_fBackgroundOffset2 = m_Window->ScreenWidth();
		}
		else if ((int)this->m_fBackgroundOffset2 > m_Window->ScreenWidth())
		{
			this->m_fBackgroundOffset2 = 0 - m_Window->ScreenWidth();
		}

		State::update(fDeltaTime);
		if (this->m_bLocked) { return; }

		for (int i = 0; i < (int)m_vButtons.size(); ++i)
		{
			if (m_vButtons[i]->isInBounds(m_Window->GetMousePos()))
			{
				m_vButtons[i]->color = olc::YELLOW;
			}
			else
			{
				m_vButtons[i]->color = olc::WHITE;
			}
		}
	}

	void MainMenuState::render()
	{
		this->m_Window->Clear(olc::BLACK);

		this->m_Window->DrawSprite(olc::vi2d(0 + (int)m_fBackgroundOffset1, 0), m_sprBackground, 4U);
		this->m_Window->DrawSprite(olc::vi2d(0 + (int)m_fBackgroundOffset2, 0), m_sprBackground, 4U);

		this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 30, 25, "VR", olc::Pixel(43, 56, 140), 4U);

		this->m_Window->DrawString(3, m_Window->ScreenHeight() - 10, "OLC CODEJAM 2021", olc::YELLOW, 1U);

		// render buttons
		for (const auto& b : this->m_vButtons)
			this->m_Window->DrawString(b->pos, b->title, b->color, 2U);

		State::render();
	}
	
}
