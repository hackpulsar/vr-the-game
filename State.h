#if !defined (STATE_H)
#define STATE_H

#include "olcPixelGameEngine.h"

namespace engine
{
	class State
	{
	protected:
		olc::PixelGameEngine* m_Window;
		bool m_bActive = true;

		// transition effect stuff
		bool m_bLocked = true;
		bool m_bFading = false;
		float m_fTransitionRadius;

	public:
		State(olc::PixelGameEngine* window);
		virtual ~State();
		
		bool isActive() const;

		void startFading();

		virtual void handleInput();
		virtual void update(float fDeltaTime);
		virtual void render();

	};
}

#endif // STATE_H
