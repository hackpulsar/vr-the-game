#include "GameState.h"

#include "LOG.h"
#include "Math.h"

#include <fstream>
#include <stdlib.h>

namespace engine 
{
	GameState::GameState(olc::PixelGameEngine* window)
		: State(window), m_Player(new Player(this, { 2.f, 10.f }, 60, 60.f))
	{
		this->loadSprites();
		this->loadLevel(this->m_nLevel);
	}

	GameState::~GameState()
	{  
		delete this->m_Player;
		
		// delete all sprites
		delete this->m_sprBlock;
		delete this->m_sprVirtualBlock;
		delete this->m_sprVRIndicator;
		delete this->m_sprBackground;
		delete this->m_sprHeart;
		delete this->m_sprPortal;
	}

	void GameState::loadSprites()
	{
		this->m_sprHeart = new olc::Sprite("./assets/heart.png");
		this->m_sprPortal = new olc::Sprite("./assets/portal.png");
		this->m_sprBlock = new olc::Sprite("./assets/block.png");
		this->m_sprVirtualBlock = new olc::Sprite("./assets/virtual_block.png");
		this->m_sprVRIndicator = new olc::Sprite("./assets/vr_indicate.png");
		this->m_sprBackground = new olc::Sprite("./assets/background.png");
	}

	void GameState::boom(olc::vi2d pos, int boom_amount)
	{
		auto random_float = [&](float min, float max)
		{
			return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
		};

		for (int i = 0; i < boom_amount; ++i)
		{
			float a = random_float(0, 2.0f * 3.14159f);
			Particle p = { float(pos.x), float(pos.y), float(cos(a) * random_float(200.f, 250.f)), float(sin(a) * random_float(200.f, 250.f)) };
			this->m_vParticles.push_back(p);
		}
	}

	wchar_t GameState::GetTile(const int x, const int y)
	{
		if (x >= 0 && x < N_LEVEL_WIDTH && y >= 0 && y < N_LEVEL_HEIGHT)
			return this->m_sLevel[y * N_LEVEL_WIDTH + x];
		else
			return L' ';
	}

	void GameState::SetTile(const int x, const int y, const wchar_t c)
	{
		if (x >= 0 && x < N_LEVEL_WIDTH && y >= 0 && y < N_LEVEL_HEIGHT)
			this->m_sLevel[y * N_LEVEL_WIDTH + x] = c;
	}

	void GameState::reloadCurrentLevel(bool bDead)
	{
		if (bDead)
			this->m_nLives--;
		this->loadLevel(this->m_nLevel, bDead ? this->m_nLives : 3);
	}

	void GameState::loadNextLevel()
	{
		if (this->m_nLives <= 3 && this->m_nLives > 0 && this->m_nLevel == 5)
		{
			this->startFading();
			return;
		}

		this->m_nLevel += 1;
		this->loadLevel(this->m_nLevel);
	}
	
	bool GameState::loadLevel(const int nLevel, int nLives)
	{
		std::fstream level_file;
		level_file.open(LEVELS[nLevel], std::ios::in);

		if (level_file.is_open())
		{
			// clear previous level
			this->m_sLevel = L"";

			this->m_fBackgroundOffset1 = 0.f;
			this->m_fBackgroundOffset2 = m_Window->ScreenWidth();

			// reset player		
			int nVirtualBlocks;
			float fGlassesLimit;

			if (m_nLevel == 1)
			{
				nVirtualBlocks = 5;
				fGlassesLimit = 10.f;
			}
			else if (m_nLevel == 2)
			{
				nVirtualBlocks = 12;
				fGlassesLimit = 15.f;
			}
			else if (m_nLevel == 3)
			{
				nVirtualBlocks = 10;
				fGlassesLimit = 15.f;
			}
			else if (m_nLevel == 4)
			{
				nVirtualBlocks = 12;
				fGlassesLimit = 25.f;
			}
			else if (m_nLevel == 5)
			{
				nVirtualBlocks = 10;
				fGlassesLimit = 30.f;
			}
			else
			{
				nVirtualBlocks = 60;
				fGlassesLimit = 60.f;
			}
			
			this->m_Player = new Player(this, { 2.f, 10.f }, nVirtualBlocks, fGlassesLimit);
			this->m_nLives = nLives;
			this->m_bPaused = false;

			std::string line;
			while (std::getline(level_file, line))
			{
				this->m_sLevel += std::wstring(line.begin(), line.end());
			}

			return true;
		}
		else
		{
			LOG("Could not open level file :(");
			return false;
		}
	}

	bool GameState::getGameResult() const { return m_nLevel == LAST_LEVEL && m_nLives > 0; }

	void GameState::handleInput()
	{	
		if (this->m_Window->IsFocused())
		{
			if (this->m_Window->GetKey(olc::Key::ESCAPE).bPressed)
			{
				this->m_bPaused = !m_bPaused;
			}

			if (this->m_bPaused)
			{
				if (this->m_Window->GetKey(olc::Key::SPACE).bPressed)
				{
					exit(0);
				}

				if (this->m_Window->GetKey(olc::Key::ENTER).bPressed)
				{
					this->reloadCurrentLevel(false);
				}
			}

			if (this->m_bPaused || m_bLocked) { return; }

			if (this->m_Window->GetMouse(0).bPressed)
			{
				if (this->m_Player->glassesAreOn())
				{
					olc::vi2d viTilePos = {
						int(m_Window->GetMouseX() / N_TILE_SIZE + fOffsetX + 0.5f),
						int(m_Window->GetMouseY() / N_TILE_SIZE + fOffsetY + 0.5f)
					};

					if (this->GetTile(viTilePos.x, viTilePos.y) == L'.'
							&& m_Player->getVirtualBlocks() > 0)
					{
						if (viTilePos.x != (int)m_Player->getPosition().x || viTilePos.y != (int)m_Player->getPosition().y)
						{
							this->SetTile(viTilePos.x, viTilePos.y, L'V');
							this->m_Player->getVirtualBlocks() -= 1;
							this->boom({ int(m_Window->GetMouseX()), int(m_Window->GetMouseY()) }, 50);
						}	
					}
				}
			}
		}

		this->m_Player->handleInput(this->m_Window);	
	}

	void GameState::update(float fDeltaTime)
	{
		State::update(fDeltaTime);
		if (this->m_bPaused == true) { return; }

		this->m_Player->update(fDeltaTime);

		// update particles
		for (auto& p : this->m_vParticles)
		{
			p.x += p.vx * fDeltaTime;
			p.y += p.vy * fDeltaTime;
		}

		this->m_fCameraPosX = this->m_Player->getPosition().x;
		this->m_fCameraPosY = this->m_Player->getPosition().y;

		// check if dead
		if (this->m_nLives <= 0)
		{ // dead
			if (!this->m_bLocked)
				this->startFading();
		} 

		if (m_Player->getVelocity().x > 0.0f)
		{ // moving right
			this->m_fBackgroundOffset1 -= m_Player->getVelocity().x * fDeltaTime;
			this->m_fBackgroundOffset2 -= m_Player->getVelocity().x * fDeltaTime;
		}
		else if (m_Player->getVelocity().x < 0.0f)
		{ // moving left
			this->m_fBackgroundOffset1 -= m_Player->getVelocity().x * fDeltaTime;
			this->m_fBackgroundOffset2 -= m_Player->getVelocity().x * fDeltaTime;
		}

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

		// delete particles
		auto it = std::remove_if(m_vParticles.begin(), m_vParticles.end(),
			[&](Particle p) 
			{ 
				return p.x < 0 || p.x > m_Window->ScreenWidth() ||
					p.y < 0 || p.y > m_Window->ScreenHeight();
			}
		);

		if (it != this->m_vParticles.end())
		{
			this->m_vParticles.erase(it);
		}
	}

	void GameState::render()
	{
		// clear screen
		this->m_Window->Clear(olc::BLUE);	
		this->m_Window->SetPixelMode(olc::Pixel::MASK);	

		// render background
		this->m_Window->DrawSprite(olc::vi2d(0 + (int)m_fBackgroundOffset1, 0), m_sprBackground, 4U);
		this->m_Window->DrawSprite(olc::vi2d(0 + (int)m_fBackgroundOffset2, 0), m_sprBackground, 4U);

		// Draw level
		nVisibleTilesX = this->m_Window->ScreenWidth() / N_TILE_SIZE;
		nVisibleTilesY = this->m_Window->ScreenHeight() / N_TILE_SIZE;

		fOffsetX = m_fCameraPosX - (float)nVisibleTilesX / 2.f;
		fOffsetY = m_fCameraPosY - (float)nVisibleTilesY / 2.f;

		math::clamp(0, N_LEVEL_WIDTH - nVisibleTilesX, fOffsetX);
		math::clamp(0, N_LEVEL_HEIGHT - nVisibleTilesY, fOffsetY);

		fTileOffsetX = (fOffsetX - (int)fOffsetX) * N_TILE_SIZE;
		fTileOffsetY = (fOffsetY - (int)fOffsetY) * N_TILE_SIZE;	

		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);

				int nTilePosX = float(x * N_TILE_SIZE) - fTileOffsetX;
				int nTilePosY = float(y * N_TILE_SIZE) - fTileOffsetY;

				switch (sTileID)
				{
				case L'.':
					//this->m_Window.FillRect({ nTilePosX, nTilePosY }, { N_TILE_SIZE, N_TILE_SIZE }, olc::BLUE);
					break;
				case L'#':
					this->m_Window->DrawSprite({ nTilePosX, nTilePosY }, m_sprBlock);
					//this->m_Window.FillRect({ nTilePosX, nTilePosY }, { N_TILE_SIZE, N_TILE_SIZE }, olc::VERY_DARK_BLUE);
					break;
				case L'-':
					this->m_Window->DrawSprite({ nTilePosX, nTilePosY }, m_sprStopBlock);
					//this->m_Window.FillRect({ nTilePosX, nTilePosY }, { N_TILE_SIZE, N_TILE_SIZE }, olc::RED);
					break;
				case L'V':
					if (this->m_Player->glassesAreOn())
						this->m_Window->DrawSprite({ nTilePosX, nTilePosY }, m_sprVirtualBlock);
					break;
				case L'O':
					this->m_Window->DrawSprite(olc::vi2d(nTilePosX, nTilePosY), m_sprPortal);
					//this->m_Window.FillRect({ nTilePosX, nTilePosY }, { N_TILE_SIZE, N_TILE_SIZE }, olc::YELLOW);
					break;
				default:
					this->m_Window->FillRect({ nTilePosX, nTilePosY }, { N_TILE_SIZE, N_TILE_SIZE }, olc::BLACK);
					break;
				}
			}
		}
		
		// render player
		this->m_Player->render(this->m_Window, fOffsetX, fOffsetY);

		for (auto& p : this->m_vParticles)
		{
			this->m_Window->FillRect(p.x, p.y, 3, 3);
		}
		
		// render lives
		for (int i = 0; i < this->m_nLives; ++i)
			this->m_Window->DrawSprite(olc::vi2d(10 + (16 + 1) * i, 10), this->m_sprHeart, 2U);

		if (this->m_Player->glassesAreOn())
		{
			this->m_Window->DrawSprite(olc::vi2d(m_Window->ScreenWidth() - 20 * 3, m_Window->ScreenHeight() - 10 * 3), m_sprVRIndicator, 3U);
		}

		if (this->m_bPaused == true)
		{
			this->m_Window->SetPixelMode(olc::Pixel::Mode::ALPHA);
			this->m_Window->FillRect(0, 0, m_Window->ScreenWidth(), m_Window->ScreenHeight(), olc::Pixel(109, 109, 109, 200));

			this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 35, m_Window->ScreenHeight() / 2 - 30, "PAUSE", olc::WHITE, 2U);
			this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 70, m_Window->ScreenHeight() - 100, "ENTER - restart");
			this->m_Window->DrawString(m_Window->ScreenWidth() / 2 - 70, m_Window->ScreenHeight() - 70, "SPACE - exit");
		}

		State::render();	
	}
}

