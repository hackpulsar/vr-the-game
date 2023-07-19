#include "SplashScreen.h"

namespace engine
{
	SplashScreen::SplashScreen(olc::PixelGameEngine* window)
		: State(window)
	{
		this->m_sTitle = "MADE BY FELIXJOYKIND";
		this->m_bLocked = false;
	}

	SplashScreen::~SplashScreen()
	{  }

	void SplashScreen::update(float fDeltaTime)
	{
		this->m_fTimer -= 1.f * fDeltaTime;

		if (this->m_fTimer <= 0.0f)
		{
			if (!this->m_bLocked)
				this->startFading();
		}
		
		State::update(fDeltaTime);
	}

	void SplashScreen::render()
	{
		this->m_Window->Clear(olc::WHITE);

		this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 80, m_Window->ScreenHeight() / 2 - 10, m_sTitle, olc::GREY, 1U);
	
		State::render();
	}
}

