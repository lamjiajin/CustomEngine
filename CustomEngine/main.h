#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <unordered_map>
#include "Window/CustomWindow.h"
#include "Input/Input.h"
#include "gl/gl3w.h"

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_win32.h"
#include "imGUI/imgui_impl_opengl3.h"
#include "FPS/FPSController.h"
#include "Math/MathInc.h"

struct GameObject;
struct PhysicsComponent;
struct GraphicsComponent;

struct AABB
{
	// data is in world-space
	vec2 min;
	vec2 max;
};
struct Circle
{
	float radius;
	vec2 offset_pos; // this position is offset from the gameobject's position
};
struct Manifold
{
	PhysicsComponent *A;
	PhysicsComponent *B;
	float penetration;
	Vec2 normal;
};
struct Physics
{
	static bool AABBvsAABB(const AABB& a, const AABB& b);
	static bool CirclevsCircle(Manifold *m);
	static bool AABBvsCircle(Manifold *m);
};


struct PhysicsComponent
{
	AABB aabb_collision_data; // local to game object
	Circle circle_collision_data;
	vec2 velocity;
	vec2 acceleration;
	GameObject* owner;
};
struct GraphicsComponent
{
	/*
	6     12    9

	0   3     15

	*/
	// 40 pixels is one unit
	// 20 pixels is half a unit
#define ONEUNIT 40
#define HALFUNIT 20
	GLuint vbo, texture_vbo;
	int vbo_index;
	int frame_index;
	std::vector<vec3> testvertices;
	std::vector<vec2> textvertices[4];
	vec2 scale{ 1,1 }; // global
	GameObject* owner = nullptr; // potentially dangerous because object is in a vector and pointers may be wrong if vector resize. circumvented by vector reserve.
	void init(std::vector<vec3>& point_data, int type);
	void update();
	void init_texture(std::vector<vec2>& texture_data);
	void init_color(std::vector<vec3>& color_data, vec3 color);
	void change_color(GLuint color_vbo, const vec3& color, int vert);
};

struct GameObject
{
	PhysicsComponent phy;
	GraphicsComponent grph;
	vec2 position; // this is global position
	void update(float dt);
};

