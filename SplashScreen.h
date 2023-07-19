#if !defined (SPLASH_SCREEN_H)
#define SPLASH_SCREEN_H

#include "State.h"
#include <string>

namespace engine 
{
	class SplashScreen
		: public State
	{
	private:
		std::string m_sTitle;
 
		float m_fTimer = 3.f;

	public:
		SplashScreen(olc::PixelGameEngine* window);
		~SplashScreen();

		void update(float fDeltaTime) override;
		void render() override;

	};
}

#endif // SPLASH_SCREEN_H
