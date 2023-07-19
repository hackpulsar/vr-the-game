#include <vector>
#if !defined (GAME_STATE_H)
#define GAME_STATE_H

#include "State.h"

#include "LEVEL_SETTINGS.h"
#include "LEVELS_PATH.h"

#include "Player.h"

namespace engine 
{
	struct Particle
	{
		float x, y, vx, vy;
	};

	class GameState
		: public State
	{
	private:
		int m_nLevel = 0; // current level (0 - tutorial level)
		std::wstring m_sLevel = L"";

		Player* m_Player; // player
		int m_nLives = 3; // number of lives

		float m_fCameraPosX = 0.f;
		float m_fCameraPosY = 0.f;

		// render data
		int nVisibleTilesX, nVisibleTilesY;
		float fOffsetX, fOffsetY;
		float fTileOffsetX, fTileOffsetY;

		// background offset (based on player movement direction)
		float m_fBackgroundOffset1, m_fBackgroundOffset2;

		bool m_bPaused = false;

		// sprites
		olc::Sprite* m_sprHeart = nullptr;
		olc::Sprite* m_sprPortal = nullptr;
		olc::Sprite* m_sprBlock = nullptr;
		olc::Sprite* m_sprVirtualBlock = nullptr;
		olc::Sprite* m_sprStopBlock = nullptr;
		olc::Sprite* m_sprVRIndicator = nullptr;
		olc::Sprite* m_sprBackground= nullptr;

		// vector of particles
		std::vector<Particle> m_vParticles;

	private:
		void loadSprites();	

	public:
		GameState(olc::PixelGameEngine* window);
		~GameState() override;
		
		// Makes BOOM in certain position
		void boom(olc::vi2d boom_pos, int boom_amount);

		wchar_t GetTile(const int x, const int y);
		void SetTile(const int x, const int y, const wchar_t c);

		void reloadCurrentLevel(bool bDead = true);
		void loadNextLevel();
		bool loadLevel(const int nLevel, int nLives = 3);

		// Returns true if won
		bool getGameResult() const;

		void handleInput() override;
		void update(float fDeltaTime) override;
		void render() override;

	};
}

#endif // GAME_STATE_H
