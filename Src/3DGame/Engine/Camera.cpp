#include "Camera.h"

Camera::Camera(winGame *game, float fov, float nearClip, float farClip)
	: game(game), bound(new bool(false))
{
	game->AddComponent(this);
	game->SetProjection_Frustrum(fov, game->width / (float)game->height, nearClip, farClip);
}

Camera::Camera(winGame * game, float width, float height, float nearClip, float farClip)
	: game(game), bound(new bool(false))
{
	game->AddComponent(this);
	game->SetProjection_Orthographic(width, height, nearClip, farClip);
}

Camera::~Camera(void)
{
	UnBind();
	delete bound;
	game = NULL;
}

void Camera::Bind(vect3 * pos, vect3 * rot)
{
	if (!(pos && rot)) throw std::invalid_argument("pos and rot cannot be NULL!");

	if (!*bound)
	{
		*bound = true;
		delete position;
		delete rotation;
	}

	position = pos;
	rotation = rot;
}

void Camera::UnBind(void)
{
	if (*bound)
	{
		*bound = false;
		position = new vect3(*position);
		rotation = new vect3(*rotation);
	}
}

void Camera::Update(GameTime gameTime, const KeyboardState & kstate, const MouseState & mstate)
{
	if (*enabled)
	{
		game->SetView(View::Update());
	}
}
