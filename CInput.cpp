#include "pch.h"

CRC32_t CUserCmd::Checksum()
{
	CRC32_t crc{};

	NEWCRC32_Init(&crc);
	NEWCRC32_ProcessBuffer(&crc, &command_nr, sizeof(command_nr));
	NEWCRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
	NEWCRC32_ProcessBuffer(&crc, &view_angles, sizeof(view_angles));
	NEWCRC32_ProcessBuffer(&crc, &fwd_move, sizeof(fwd_move));
	NEWCRC32_ProcessBuffer(&crc, &side_move, sizeof(side_move));
	NEWCRC32_ProcessBuffer(&crc, &up_move, sizeof(up_move));
	NEWCRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
	NEWCRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
	NEWCRC32_ProcessBuffer(&crc, &weapon, sizeof(weapon));
	NEWCRC32_ProcessBuffer(&crc, &weapon_type, sizeof(weapon_type));
	NEWCRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
	NEWCRC32_ProcessBuffer(&crc, &mouse_dx, sizeof(mouse_dx));
	NEWCRC32_ProcessBuffer(&crc, &mouse_dy, sizeof(mouse_dy));

	NEWCRC32_ProcessBuffer(
		&crc, __pad + 5, sizeof(bool)); // I haven't checked out what this is

	NEWCRC32_Final(&crc);

	return crc;
}

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
	return &this->cmds[sequence_number & MULTIPLAYER_BACKUP];
	/*using OriginalFn = CUserCmd * (__thiscall*)(CInput* _this, int seq_num);
	return ((OriginalFn)GetVFunc3(this, 8))(this, sequence_number);*/
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
	return &this->verified_cmds[sequence_number % MULTIPLAYER_BACKUP];
}

// This works ONLY ON GMOD! Do not replicate this ass fuckery anywhere legitimate.
bool* CInput::pfCameraInThirdPerson()
{
	return (bool*)&(((bool*)this)[0x8D]);
}