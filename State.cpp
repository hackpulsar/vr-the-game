#include "State.h"

#include "LOG.h"
namespace engine
{
	State::State(olc::PixelGameEngine* window)
		: m_Window(window)
	{ 
		this->m_fTransitionRadius = m_Window->ScreenWidth();
	}

	State::~State()
	{  }

	bool State::isActive() const { return this->m_bActive; }

	void State::startFading()
	{
		this->m_bLocked = true;
		this->m_bFading = true;
		this->m_fTransitionRadius = 0;
	}
	
	void State::handleInput()
	{  }

	void State::update(float fDeltaTime)
	{
		if (this->m_bLocked == true)
		{
			if (this->m_bFading == true)
			{
				this->m_fTransitionRadius += 300.f * fDeltaTime;
				if (this->m_fTransitionRadius >= m_Window->ScreenWidth())
					this->m_bActive = false;
			}
			else
				this->m_fTransitionRadius -= 300.f * fDeltaTime;
		}

		if (this->m_fTransitionRadius <= 0.0f)
			this->m_bLocked = false;
	}

	void State::render()
	{
		// this->m_Window->Clear(olc::WHITE);

		if (this->m_bLocked)
		{
			this->m_Window->SetPixelMode(olc::Pixel::Mode::ALPHA);
			this->m_Window->FillCircle(m_Window->ScreenWidth() / 2, m_Window->ScreenHeight() / 2, (int32_t)m_fTransitionRadius, olc::BLACK);
		}
	}
}
