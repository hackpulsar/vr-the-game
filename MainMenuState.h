#if !defined (MAINMENU_STATE_H)
#define MAINMENU_STATE_H

#include "State.h"

namespace engine
{
	struct Button
	{
		olc::vi2d size;
		olc::vi2d pos;
		std::string title;
		olc::Pixel color;

		bool isInBounds(const olc::vi2d& position) const
		{
			return position.x > pos.x && position.x < pos.x + size.x
				&& position.y > pos.y && position.y < pos.y + size.y;
		}
	};

	class MainMenuState
		: public State
	{
	private:
		std::vector<Button*> m_vButtons;

		olc::Sprite* m_sprBackground = nullptr;
		float m_fBackgroundOffset1, m_fBackgroundOffset2;

	public:
		MainMenuState(olc::PixelGameEngine* window);
		~MainMenuState();
		
		void handleInput() override;
		void update(float fDeltaTime) override;
		void render() override;

	};
}

#endif // MAINMENU_STATE_H
