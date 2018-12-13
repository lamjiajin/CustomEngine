
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "main.h"



struct mousepos
{
	int x, y;
};

void mouseclick(int x, int y)
{
	INPUT Inputs[3] = { 0 };

	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dx = x; // desired X coordinate
	Inputs[0].mi.dy = y; // desired Y coordinate
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	Inputs[1].type = INPUT_MOUSE;
	Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	Inputs[2].type = INPUT_MOUSE;
	Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(3, Inputs, sizeof(INPUT));
}


struct tex_frame_coord
{
	vec2 data[6];
};

// animationdata can be Victoria
struct animationdata
{
	// Victoria contain information on 'move left', 'move right' etc
	// those are keys to the frame coords
	std::unordered_map<std::string, std::vector<tex_frame_coord>> data;
};

struct animationsystem
{
	std::unordered_map<std::string, animationdata> animationCollection;
};

struct animationcomponent
{
	// start from 0
	int current_frame;
	float current_dt;

};

tex_frame_coord generateTexData(float curr_x, float curr_y, float max_col, float max_row)
{
	tex_frame_coord t;
	t.data[0]={ curr_x/ max_col						, curr_y/ max_row };
	t.data[1]={ curr_x/ max_col	+ 1.f / max_col		, curr_y/ max_row };
	t.data[2]={ curr_x/ max_col + 1.f / max_col		, curr_y/ max_row + 1.f / max_row };
	t.data[3]={ curr_x/ max_col + 1.f / max_col		, curr_y/ max_row + 1.f / max_row };
	t.data[4]={ curr_x/ max_col						, curr_y/ max_row + 1.f / max_row };
	t.data[5]={ curr_x/ max_col						, curr_y/ max_row };
	return t;
}

// this function reads a sprite sheet and returns an animation data object
animationdata readSpriteMetadata(const std::string& filename)
{
	animationdata res;
	std::ifstream objectfile(filename);
	if (objectfile.is_open())
	{
		// number of animations in this file
		int num_of_animation;
		objectfile >> num_of_animation;
		//	objectfile.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

		// number of rows in the spritesheet
		int row;
		objectfile >> row;	

		// max number of columns, does not have to represent all rows' columns
		int column;
		objectfile >> column;	
		
		std::string animationname;
		objectfile >> animationname;

		for (int i = 0; i < num_of_animation; ++i)
		{
			res.data[animationname]; // creates the key in the data
			vec2 framepos;
			
			objectfile >> framepos.x; 
			objectfile >> framepos.y;
			res.data[animationname].push_back(generateTexData(framepos.x, framepos.y, column, row));
			
			// this string is to check if we are to go to next animation or grab more frames
			std::string check;
			objectfile >> check;
			if (check == "-")
			{
				vec2 endframepos;
				objectfile >> endframepos.x;
				objectfile >> endframepos.y; // no intention of dealing with sprites that span across multiple rows
				// taking in all frame data
				for (float j = framepos.x + 1; j <= endframepos.x; ++j)
				{
					res.data[animationname].push_back(generateTexData(j, endframepos.y, column, row));
				}

				// sets animationname to the actual next name
				objectfile >> animationname;

			}
			else
				animationname = check;
		}
	}
	return res;
}	

int main(void)
{
	GameWindow mainWindow;	
	mainWindow.init("FirstEngine",0,0,1600,900);
	input_mgr::Init();
	input_mgr::active = true;
	FPSController::GetInstance().Init();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(mainWindow.hwnd);
	ImGui_ImplOpenGL3_Init();
	// Setup style
	ImGui::StyleColorsDark();



	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	std::vector<vec3> point_data;
	std::vector<vec2> texture_data;
	std::vector<float> framenum_data;
	
	std::vector<GameObject> go_container;
	
	go_container.reserve(100);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, point_data.size() * sizeof(vec3), point_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	GLuint texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, texture_data.size() * sizeof(vec2), texture_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	GLuint framenum_vbo = 0;
	glGenBuffers(1, &framenum_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, framenum_vbo);
	glBufferData(GL_ARRAY_BUFFER, framenum_data.size() * sizeof(float), framenum_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("idle.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//readSpriteMetadata("VictoriaSpritesheet-Alpha.txt");


	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vp;"
		"layout(location = 1) in vec2 aTexCoord;"
		"layout(location = 2) in float framenum;"
		"out vec2 TexCoord;"
		"void main() {"
		"gl_Position = vec4(vp, 1.0);"
		"TexCoord = aTexCoord;"
		"TexCoord.x = TexCoord.x + framenum / 8.f;"
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"in vec2 TexCoord;"
		"out vec4 frag_colour;"
		"uniform sampler2D ourTexture;"
		"void main() {"
		"frag_colour = texture(ourTexture, TexCoord);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	// insert location binding code here
	glBindAttribLocation(shader_programme, 0, "vp");
	glBindAttribLocation(shader_programme, 1, "aTexCoord");
	glBindAttribLocation(shader_programme, 2, "framenum");
	glLinkProgram(shader_programme);

	
	int state = 0;
	while (true)
	{
		FPSController::GetInstance().Start();
		float dt = FPSController::GetInstance().GetFrameTime();
		mainWindow.Update(0.f);
		input_mgr::Update();


		glViewport(0, 0, mainWindow.m_width, mainWindow.m_height);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		/* graphics draw here */
		float x = input_mgr::GetMouseX();
		float y = input_mgr::GetMouseY();

		vec3 mouse_pos{ x,y,1 };
		mat3 m{ 1, 0 , -800 , 0 ,-1, 430, 0, 0, 1 };
		//m.Inverse();
		mouse_pos = m * mouse_pos;


		// character control
		bool changeframe = false;
		int movement = false;


		// graphics update
		static float framenum = 0;
		static float frametime = 0;

		
		frametime += dt / 1000.f;

		if (frametime >= 0.125f)
		{
			frametime = 0;

			++framenum;
			if (framenum > 7.f)
			{
				framenum = 0;
			}
		}

		
		for (int i = 0; i < go_container.size(); ++i)
		{
			go_container[i].update(dt);

			std::vector<float> data;
			for (auto&elem : go_container[i].grph.textvertices[0])
			{
				data.push_back(framenum);
			}
			glBindBuffer(GL_ARRAY_BUFFER, framenum_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, go_container[i].grph.frame_index * sizeof(float), data.size() * sizeof(float), data.data());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);

		// pass in the number of points
		int number_of_vertices = point_data.size();
		glDrawArrays(GL_TRIANGLES, 0, number_of_vertices);


		static bool imgui_active = true;
		if (input_mgr::IsTriggered(Key::BACKQUOTE))
		{
			imgui_active = !imgui_active;
		}
		if (imgui_active)
		{
			// Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			{
				ImGui::InputInt("movement", &movement);
				ImGui::InputFloat("frametime:", &framenum);
				ImGui::InputFloat("frametime:", &frametime);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", dt, 1000.f/dt);
				ImGui::Text("Point data's size: %d", point_data.size());
				if (ImGui::Button("New Object!"))
				{
					go_container.push_back({});
					auto& go = go_container.back();
					go.position.x = 0;
					go.grph.owner = &go;
					go.grph.scale = { 120,120 };

					go.grph.vbo = pos_vbo;

					// this function call will set the vertices data on screen space
					go.grph.init(point_data, 2);

					glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
					glBufferData(GL_ARRAY_BUFFER, point_data.size() * sizeof(vec3), point_data.data(), GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);


					go.grph.texture_vbo = texture_vbo;
					
					// global data - needs this for the ONE vbo 
					texture_data.push_back({ 0, 0 });
					texture_data.push_back({ 1.f/8.f, 0 });
					texture_data.push_back({ 1.f/8.f,1.f });
					texture_data.push_back({ 1.f/8.f,1.f });
					texture_data.push_back({ 0, 1.f });
					texture_data.push_back({ 0, 0 });

					// local data - need this to reset back data
					go.grph.textvertices[0].push_back({ 0, 0 });
					go.grph.textvertices[0].push_back({ 1.f / 8.f, 0 });
					go.grph.textvertices[0].push_back({ 1.f / 8.f,1.f });
					go.grph.textvertices[0].push_back({ 1.f / 8.f,1.f });
					go.grph.textvertices[0].push_back({ 0, 1.f });
					go.grph.textvertices[0].push_back({ 0, 0 });
					

					glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
					glBufferData(GL_ARRAY_BUFFER, texture_data.size() * sizeof(vec2), texture_data.data(), GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					go.grph.frame_index = framenum_data.size();
					for (int i = 0; i < 8; ++i)
					{
						framenum_data.push_back(0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, framenum_vbo);
					glBufferData(GL_ARRAY_BUFFER, framenum_data.size() * sizeof(float), framenum_data.data(), GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				for (int i = 0; i < go_container.size(); ++i)
				{
					auto&elem = go_container[i];
					ImGui::PushID(i);
					ImGui::Text("Object");
					ImGui::SliderFloat("pos x", &elem.position.x,-800,800);
					ImGui::SliderFloat("pos y", &elem.position.y, -430, 430);
					ImGui::PopID();
				}		
			}

			
			ImGui::Render();

			
			/* imgui draw here */
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		SwapBuffers(mainWindow.hDC);

		FPSController::GetInstance().End();
	}
	// delete graphics buffers?

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 1;
}


 void GameObject::update(float dt)
{
	grph.update();
}
