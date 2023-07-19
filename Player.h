#if !defined (PLAYER_H)
#define PLAYER_H

#include "olcPixelGameEngine.h"

namespace engine
{
	class GameState;
}

class Player
{
private:
	// pointer to a game state
	engine::GameState* m_GameState = nullptr;

	olc::vf2d m_vfPos;
	olc::vf2d m_vfVel;	

	int m_nVirtualBlocks;
	
	float m_fGlassesLimit = 10.f;
	float m_fCurrentTime;

	bool m_bOnGround = false;
	bool m_bGlassesOn = false;
	
	olc::Sprite* m_sprGlasses = nullptr;

	// animation logic
	bool m_bFaceRight = true;
	float m_fAnimationFrame = 0.f;
	std::vector<olc::Sprite*> m_vAnimationFrames;

public:
	Player(engine::GameState* gameState, olc::vf2d position, int nVirtualBlocks, float fGlassesLimit);
	~Player();

	// Returns reference to the number of virtual blocks
	int& getVirtualBlocks();

	// Returns true if player wears glasses
	bool glassesAreOn() const;

	// Sets player position
	void setPosition(const olc::vf2d position);

	// Returns player position
	olc::vf2d getPosition() const;

	// Sets player velocity
	void setVelocity(const olc::vf2d velocity);
	void setVelocityX(const float velocity_x);
	void setVelocityY(const float velocity_y);

	// Returns player velocity
	olc::vf2d getVelocity() const;

	void handleInput(olc::PixelGameEngine* target);

	void update(float fDeltaTime);
	void render(olc::PixelGameEngine* target, float fOffsetX, float fOffsetY);

};

#endif // PLAYER_H
