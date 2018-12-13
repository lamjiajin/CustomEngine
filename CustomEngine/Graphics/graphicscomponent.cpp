#include "main.h"

//1-circle, 2-square
void GraphicsComponent::init(std::vector<vec3>& point_data, int type)
{
	vbo_index = point_data.size();
	auto& position = owner->position;
	mat3 scale_mat{ scale.x, 0, 0, 0, scale.y,0,0,0,1 };
	mat3 trans_mat{ 1,0,position.x,0,1,position.y,0,0,1 };
	mat3 rot_mat;
	mat3 modelToWorld = trans_mat * rot_mat *scale_mat;

	mat3 cam;

	mat3 modelToView = cam * modelToWorld;
	mat3 clip{ 2.f / 1600.f, 0 , 0 , 0 ,2.f / 900.f, 0, 0, 0, 1 };
	mat3 modelToClip = clip * modelToView;


	// circle
	if (type == 1)
	{
		for (int i = 0; i < 45; ++i)
		{
			vec3 p{ 0.5f,0,1 };
			mat3 rot; mat3 rot2;
			vec3 p0 = mat3::RotateZ(i * 2 * PI / 45.f, rot) * p;
			vec3 p1 = mat3::RotateZ((i + 1) * 2 * PI / 45.f, rot) * p;
			vec3 p2 = vec3{ 0,0,1.f };
			testvertices.push_back(p0);
			testvertices.push_back(p1);
			testvertices.push_back(p2);
		}

	}
	else if (type == 2)
	{
		// square
		{
			testvertices.push_back({ -0.5f, -0.5f, 1.f });
			testvertices.push_back({ 0.5f, -0.5f,  1.f });
			testvertices.push_back({ 0.5f, 0.5f,   1.f });
			testvertices.push_back({ 0.5f, 0.5f,   1.f });
			testvertices.push_back({ -0.5f, 0.5f,  1.f });
			testvertices.push_back({ -0.5f, -0.5f, 1.f });
		}
	}
	else
	{
		assert(false);
	}



	for (int i = 0; i < testvertices.size(); ++i)
	{
		vec3 clipdata = modelToClip * testvertices[i];
		point_data.push_back(clipdata);
	}
}

void GraphicsComponent::update()
{
	// get position of object
	auto& position = owner->position;

	// get the ads
	mat3 scale_mat{ scale.x, 0, 0, 0, scale.y,0,0,0,1 };
	mat3 trans_mat{ 1,0,position.x,0,1,position.y,0,0,1 };
	mat3 rot_mat;
	mat3 modelToWorld = trans_mat * rot_mat *scale_mat;

	mat3 cam;

	mat3 modelToView = cam * modelToWorld;
	mat3 ViewToclip{ 2.f / 1600.f, 0 , 0 , 0 ,2.f / 900.f, 0, 0, 0, 1 };
	mat3 modelToClip = ViewToclip * modelToView;

	std::vector<vec3> clip_data;

	for (int i = 0; i < testvertices.size(); ++i)
	{
		clip_data.push_back(modelToClip * testvertices[i]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, vbo_index * sizeof(vec3), clip_data.size() * sizeof(vec3), clip_data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsComponent::init_color(std::vector<vec3>& color_data, vec3 color)
{
	for (int i = 0; i < testvertices.size(); ++i)
	{
		color.x = (rand() % 255) / 255.f;
		color.y = (rand() % 255) / 255.f;
		color.z = (rand() % 255) / 255.f;
		color_data.push_back(color);
	}


}

void GraphicsComponent::init_texture(std::vector<vec2>& texture_data)
{
	texture_data.push_back({ 0, 0 });
	texture_data.push_back({ 1, 0 });
	texture_data.push_back({ 1,1 });
	texture_data.push_back({ 1,1 });
	texture_data.push_back({ 0, 1 });
	texture_data.push_back({ 0,0 });
}

void GraphicsComponent::change_color(GLuint color_vbo, const vec3 & color, int vert)
{
	float newcolor[3]{ color.x,color.y,color.z };
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (vbo_index + (vert - 1) * 3) * sizeof(float), 3 * sizeof(float), newcolor);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


