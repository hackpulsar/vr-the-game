#include "Player.h"

#include "Physics.h"
#include "GameState.h"

#include "LEVEL_SETTINGS.h"
#include "LOG.h"
#include <cmath>
#include <string>

Player::Player(engine::GameState* gameState, olc::vf2d position, int nVirtualBlocks, float fGlassesLimit)
	: m_GameState(gameState), m_vfPos(position), m_nVirtualBlocks(nVirtualBlocks), m_fGlassesLimit(fGlassesLimit)
{  
	this->m_fCurrentTime = fGlassesLimit;

	this->m_sprGlasses = new olc::Sprite("./assets/glasses.png");

	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player0_right.png"));
	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player1_right.png"));
	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player0_left.png"));
	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player1_left.png"));
	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player_jump_right.png"));
	this->m_vAnimationFrames.push_back(new olc::Sprite("./assets/player_jump_left.png"));
}

Player::~Player()
{
	for (int i = 0; i < (int)m_vAnimationFrames.size(); ++i)
		delete this->m_vAnimationFrames[i];
	this->m_vAnimationFrames.clear();
}

int& Player::getVirtualBlocks() { return this->m_nVirtualBlocks; }
bool Player::glassesAreOn() const { return this->m_bGlassesOn; }

void Player::setPosition(const olc::vf2d position) { this->m_vfPos = position; }
void Player::setVelocity(const olc::vf2d velocity) { this->m_vfVel = velocity; }

void Player::setVelocityX(const float velocity_x) { this->m_vfVel.x = velocity_x; }
void Player::setVelocityY(const float velocity_y) { this->m_vfVel.y = velocity_y; }

olc::vf2d Player::getPosition() const { return this->m_vfPos; }
olc::vf2d Player::getVelocity() const { return this->m_vfVel; }

void Player::handleInput(olc::PixelGameEngine* target)
{
	if (target->IsFocused())
	{
		// movement
		if (target->GetKey(olc::Key::D).bHeld)
		{
			if (this->m_bOnGround == false)
				this->setVelocityX(4.f);
			else
				this->setVelocityX(6.f);

			if (this->m_bFaceRight == false)
			{
				this->m_bFaceRight = true;
				this->m_fAnimationFrame = 0.f;
			}
		}

		if (target->GetKey(olc::Key::A).bHeld)
		{
			if (this->m_bOnGround == false)
				this->setVelocityX(-4.f);
			else
				this->setVelocityX(-6.f);

			if (this->m_bFaceRight == true)
			{
				this->m_bFaceRight = false;
				this->m_fAnimationFrame = 3.f;
			}
		}

		if (target->GetKey(olc::Key::W).bHeld)
		{
			if (this->m_bOnGround)
			{
				this->setVelocityY(-F_JUMP_FORCE);
				this->m_bOnGround = false;
			}
		}

		if (target->GetKey(olc::Key::S).bHeld)
		{
			this->setVelocityY(6.f);
		}

		if (target->GetKey(olc::Key::X).bPressed)
		{
			if (this->m_fCurrentTime > 0.f)
			{
				// wear glasses
				this->m_bGlassesOn = !this->m_bGlassesOn;
			}
		}	
	}
}

void Player::update(float fDeltaTime)
{
	this->m_vfVel.y += F_GRAVITY * fDeltaTime;

	// decelerate player movement
	if (this->m_bOnGround)
	{
		this->m_vfVel.x += -6.f * this->m_vfVel.x * fDeltaTime;
		if (fabs(this->m_vfVel.x) < 0.15f)
			this->m_vfVel.x = 0.f;

		if (this->m_bFaceRight && this->getVelocity().x > 0.0f)
		{
			this->m_fAnimationFrame += 5.f * fDeltaTime;

			if (this->m_fAnimationFrame > 1.99f)
				this->m_fAnimationFrame = 0.f;
		}
		else if (this->getVelocity().x < 0.0f)
		{
			this->m_fAnimationFrame -= 5.f * fDeltaTime;

			if (this->m_fAnimationFrame <= 2.0f)
				this->m_fAnimationFrame = 3.99f;
		}
		
		if (this->getVelocity().x == 0.0f)
			this->m_fAnimationFrame = 0.f;

	}

	float fNewPlayerPosX = this->getPosition().x + this->getVelocity().x * fDeltaTime;
	float fNewPlayerPosY = this->getPosition().y + this->getVelocity().y * fDeltaTime;
	
	if (this->m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY + 0.f) == L'O')
		this->m_GameState->loadNextLevel();

	if (this->m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY + 1.f) == L'O')
			this->m_GameState->loadNextLevel();

	if (this->m_GameState->GetTile(fNewPlayerPosX + 1.f, fNewPlayerPosY + 0.f) == L'O')
		this->m_GameState->loadNextLevel();

	if (this->m_GameState->GetTile(fNewPlayerPosX + 1.f, fNewPlayerPosY + 1.f) == L'O')
			this->m_GameState->loadNextLevel();

	if (this->getVelocity().x <= 0.f) // left
	{
		if (m_GameState->GetTile(fNewPlayerPosX + 0.f, this->getPosition().y + 0.f) != L'.'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.f, this->getPosition().y + 0.9f) != L'.')
		{
			fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				this->setVelocityX(0.f);
		}
	}
	else // right
	{
		if (m_GameState->GetTile(fNewPlayerPosX + 1.f, this->getPosition().y + 0.f) != L'.'
				|| m_GameState->GetTile(fNewPlayerPosX + 1.f, this->getPosition().y + 0.9f) != L'.')
		{
			fNewPlayerPosX = (int)fNewPlayerPosX;
			this->setVelocityX(0.f);
		}

	}

	if (this->getVelocity().y <= 0.f) // up
	{
		if (m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY) != L'.'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'.')
		{
			fNewPlayerPosY = (int)fNewPlayerPosY + 1;
			this->setVelocityY(0.f);
		}
	}
	else // down
	{
		if (m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY + 1.f) == L'#'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.f) == L'#'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY + 1.f) == L'V'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.f) == L'V')
		{
			fNewPlayerPosY = (int)fNewPlayerPosY;
			this->setVelocityY(0.f);
			this->m_bOnGround = true;	
		}
		else if (m_GameState->GetTile(fNewPlayerPosX + 0.f, fNewPlayerPosY + 1.f) == L'-'
				|| m_GameState->GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.f) == L'-')
		{
			// LOG("reload");
			this->m_GameState->reloadCurrentLevel();
		}
		else
			this->m_bOnGround = false;
	}

	//this->m_vfPos.x += this->m_vfVel.x * fDeltaTime;
	//this->m_vfPos.y += this->m_vfVel.y * fDeltaTime;

	this->setPosition({ fNewPlayerPosX, fNewPlayerPosY });

	// update glasses limit
	if (this->m_bGlassesOn && this->m_fCurrentTime != 0.f)
	{
		this->m_fCurrentTime -= 1.f * fDeltaTime;

		if (this->m_fCurrentTime <= 0.f)
		{
			this->m_fCurrentTime = 0.f;
			this->m_bGlassesOn = false;
		}
	}

	if (this->m_bOnGround == false)
	{
		this->m_fAnimationFrame = m_bFaceRight ? 4.f : 5.f;
	}
}

void Player::render(olc::PixelGameEngine* target, float fOffsetX, float fOffsetY)
{
	//target->FillRect({ int((m_vfPos.x - fOffsetX) * N_TILE_SIZE), int((m_vfPos.y - fOffsetY) * N_TILE_SIZE) }, { 16, 16 }, olc::GREEN);
	
	olc::vi2d viPlayerPos = { int((m_vfPos.x - fOffsetX) * N_TILE_SIZE), int((m_vfPos.y - fOffsetY) * N_TILE_SIZE) };
	target->DrawSprite(viPlayerPos, m_vAnimationFrames[(int)m_fAnimationFrame]);

	if (this->m_bGlassesOn)
		target->DrawSprite(viPlayerPos.x + 4, viPlayerPos.y + 3, m_sprGlasses);

	target->DrawString(olc::vi2d(target->ScreenWidth() - 50, 10), std::to_string(this->m_fCurrentTime).substr(0, 4) + "s"); // render timer
	target->DrawString(olc::vi2d(target->ScreenWidth() - 50, 30), std::to_string(m_nVirtualBlocks)); // render number of blocks
}

