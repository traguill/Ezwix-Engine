#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);

	Position = vec(0.0f, 0.0f, 5.0f);
	Reference = vec(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	vec newPos(0,0,0);
	float speed = 8.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 20.0f * dt;
	
	//if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += X * speed;

	float wheel_speed = 200 * dt;

	if(App->input->GetMouseZ() > 0) newPos -= Z * speed * wheel_speed;
	if (App->input->GetMouseZ() < 0) newPos += Z * speed * wheel_speed;

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		newPos += X * dx * speed;
		newPos += Y * dy * speed;
	}

	Position += newPos;
	Reference += newPos;


	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.025f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			Quat quaternion;
			quaternion = quaternion.RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX);
			X = quaternion * X;
			Y = quaternion * Y;
			Z = quaternion * Z;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat quaternion2;
			quaternion2 = quaternion2.RotateAxisAngle(X, DeltaY);
			Y = quaternion2 * Y;
			Z = quaternion2 * Z;

			if(Y.y < 0.0f)
			{
				Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec &Position, const vec &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference);
	Z.Normalize();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z);
	X.Normalize();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec &Spot)
{
	Reference = Spot;

	Z = (Position - Reference);
	Z.Normalize();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z);
	X.Normalize();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}
// ------------------------------------------------------------------
void ModuleCamera3D::Move(Direction d, float speed)
{
	vec newPos(0, 0, 0);
	switch (d)
	{
	case GO_RIGHT:
		newPos += X * speed;
		break;
	case GO_LEFT:
		newPos -= X * speed;
		break;
	case GO_UP:
		newPos.y += speed;
		break;
	case GO_DOWN:
		newPos.y -= speed;
		break;
	default:
		break;
	}

	Position += newPos;
	Reference += newPos;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return *ViewMatrix.v;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot( Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

void ModuleCamera3D::Rotate(float x, float y)
{
	int dx = -x;
	int dy = -y;

	float Sensitivity = 0.25f;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;
	
		Quat quaternion;
		quaternion = quaternion.RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX);

		X = quaternion * X;
		Y = quaternion * Y;
		Z = quaternion * Z;
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Quat quaternion;
		quaternion = quaternion.RotateAxisAngle(X, DeltaY);
		Y = quaternion * Y;
		Z = quaternion * Z;

		if (Y.y < 0.0f)
		{
			Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = Z.Cross(X);
		}
	}

	Position = Reference + Z * Position.Length();

	CalculateViewMatrix();
}
// -----------------------------------------------------------------

void ModuleCamera3D::WorldToScreen(vec point, int& x, int& y)
{
	//Calculate perspective
	float4x4 perspective;
	float fov = 60.0f;
	float _near = 0.125f;
	float _far = 512.0f;

	perspective.SetIdentity();
	float tan_theta_over2 = tan(fov * pi / 360.0f);

	perspective[0][0] = 1.0f / tan_theta_over2;
	perspective[1][1] = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) / tan_theta_over2;
	perspective[2][2] = (_near + _far) / (_near - _far);
	perspective[3][2] = 2 * _near * _far / (_near - _far);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	float4x4 projection = perspective;

	float4 screen_mat = ViewMatrix * float4(point, 1);
	vec screen = screen_mat.xyz();
	screen = float4(projection * screen_mat).xyz();

	screen.x /= screen.z;
	screen.y /= screen.z;

	x = (screen.x +1) * (SCREEN_WIDTH /2);
	y = (screen.y + 1) * (SCREEN_HEIGHT /2);
}

// -----------------------------------------------------------------
//TODO: Review
vec ModuleCamera3D::ScreenToWorld(int x, int y)
{
	float2 mouse_pos(App->input->GetMouseX(), App->input->GetMouseY());

	float4x4 projection = ViewMatrix * App->renderer3D->ProjectionMatrix;
	projection.Inverse();

	float4 vector;
	vector.x = ((2.0f * mouse_pos.x) / SCREEN_WIDTH) -1.0f;
	vector.y = ((2.0f * mouse_pos.y) / SCREEN_HEIGHT) -1.0f;
	vector.z = ((2.0f * 0.125f) / 512.0f) -1.0f; //Depth value. Is this correct?
	vector.w = 1.0f;

	float4 point = vector * projection;

	point.w = 1.0f / point.w;

	point.x *= point.w;
	point.y *= point.w;
	point.z *= point.w;

	return vec(point.x, point.y, point.z);
}


vec ModuleCamera3D::Raycast()
{
	float4 ray_ndc; //Normalized Device Coordinates
	ray_ndc.x = (2.0f * App->input->GetMouseX()) / SCREEN_WIDTH - 1.0f;
	ray_ndc.y = 1.0f - (2.0f * App->input->GetMouseY()) / SCREEN_HEIGHT;
	ray_ndc.z = -1.0f;
	ray_ndc.w = 1.0f;

	float4 ray_eye = App->renderer3D->ProjectionMatrix.Inverted() * ray_ndc;

	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;


	vec ray_world = (ViewMatrix.Inverted() * ray_eye).xyz();
	ray_world.Normalize();

	return ray_world;
}