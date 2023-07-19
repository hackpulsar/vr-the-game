#if !defined (END_GAME_STATE_H)
#define END_GAME_STATE_H

#include "State.h"

namespace engine
{
	class EndGameState
	: public State
	{
	private:
		bool m_bWon;
		std::string m_sTitle;

	public:
		EndGameState(olc::PixelGameEngine* window, bool bWon = false);
		~EndGameState();
		
		void handleInput() override;
		void update(float fDeltaTime) override;
		void render() override;

	};
}

#endif // END_GAME_STATE_H
