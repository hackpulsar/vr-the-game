#include "EndGameState.h"

namespace engine
{
	EndGameState::EndGameState(olc::PixelGameEngine* window, bool bWon)
		: State(window), m_bWon(bWon)
	{
		if (bWon)
		{
			this->m_sTitle = "Won :)";
		}
		else
		{
			this->m_sTitle = "Losed :(";
		}	
	}

	EndGameState::~EndGameState()
	{  }

	void EndGameState::handleInput()
	{
		if (this->m_Window->IsFocused())
		{
			if (this->m_Window->GetKey(olc::Key::SPACE).bPressed)
			{
				if (!this->m_bLocked)
					this->startFading();
			}
		}
	}

	void EndGameState::update(float fDeltaTime)
	{
		State::update(fDeltaTime);
	}

	void EndGameState::render()
	{
		this->m_Window->Clear(olc::WHITE);

		this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 45, m_Window->ScreenHeight() / 2 - 50, "You " + m_sTitle, olc::GREY);
		this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 75, m_Window->ScreenHeight() / 2 - 30, m_bWon == false ? "Good luck next time!" : "Thanks for playing!", olc::GREY);
		this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 90, m_Window->ScreenHeight() - 25, "Press <SPACE> to replay.", olc::GREY);

		State::render();
	}
}

