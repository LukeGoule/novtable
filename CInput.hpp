#pragma once

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

class bf_read;
class bf_write;
class QAngle;

struct CUserCmd
{
	std::int32_t command_nr;
	std::int32_t tick_count;
	QAngle        view_angles;
	float        fwd_move;
	float        side_move;
	float        up_move;
	std::int32_t buttons;
	std::uint8_t impulse;
	std::size_t  weapon;
	std::int32_t weapon_type;
	std::int32_t random_seed;
	std::int16_t mouse_dx;
	std::int16_t mouse_dy;
	bool         has_been_predicted;

private:
	std::uint8_t __pad[0x108];

public:
	CRC32_t Checksum();
};

class CVerifiedUserCmd
{
public:
	CUserCmd  m_pCmd;
	CRC32_t   m_CRC32;
};

class CInput
{
	char __pad[0x31 * sizeof(uint32_t)];

public:
	CUserCmd* GetUserCmd(int sequence_number);
	CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
	
	bool* pfCameraInThirdPerson();

	CUserCmd* cmds;
	CVerifiedUserCmd* verified_cmds;
};