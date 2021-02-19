#include "stdafx.h"
#include "esp.h"

bool ESP::IsTeamGame()
{
	if ((*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 11 || *gameMode == 13 ||
		*gameMode == 14 || *gameMode == 16 || *gameMode == 17 || *gameMode == 20 || *gameMode == 21))
		return true;
	else return false;
}

bool ESP::IsEnemy(ent* e)
{
	if (localPlayer->team == e->team)
		return false;
	else return true;
}

bool ESP::IsValidEnt(ent* ent)
{
	if (ent)
	{
		if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0)
			return true;
	}
	return false;
}

void ESP::DrawESPBox(ent* e, vec3 screen, GL::Font& font)
{
	const GLubyte* color = nullptr;

	if (IsTeamGame() && !IsEnemy(e))
		color = rgb::green;
	else color = rgb::red;

	float dist = localPlayer->pos.Distance(e->pos);

	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	float x = screen.x - scale;
	float y = screen.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;

	GL::DrawOutline(x, y, width, height, 2.0f, color);

	float textX = font.centerText(x, width, strlen(e->name) * ESP_FONT_WIDTH);
	float textY = y - ESP_FONT_HEIGHT / 2;
	font.Print(textX, textY, color, "%s", e->name);
}

void ESP::Draw(GL::Font& font)
{
	glGetIntegerv(GL_VIEWPORT, viewport);

	for (int i = 0; i < (*numOfPlayers); i++)
	{
		if (entlist && entlist->ents && IsValidEnt(entlist->ents[i]))
		{
			ent* e = entlist->ents[i];
			vec3 center = e->head;
			center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

			vec3 screenCoords;

			if (WorldToScreen(center, screenCoords, matrix, viewport[2], viewport[3]))
			{
				DrawESPBox(e, screenCoords, font);
			}
		}
	}
}