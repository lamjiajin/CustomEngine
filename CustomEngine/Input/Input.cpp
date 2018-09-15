

#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "Input.h"
#include "../Window/CustomWindow.h"
#include <algorithm>
#define clamp(v, _min, _max) (max(min(v, _max), _min))

#pragma comment(lib, "Xinput9_1_0.lib")


static input_mgr inputSystem;

input_mgr* input_mgr::input_mgr_ptr = nullptr;
int input_mgr::fps_mode = 0;
bool input_mgr::active = false;

static int window_width = 1280;
static int window_height = 720;
static int midx = window_width / 2;
static int midy = window_height / 2;
static HCURSOR cursor;
static bool show_cursor = false;
// for controller only
static bool prev_cursor_visibility = false;
bool Controller::connected;
BYTE Controller::currKey[extraControls::maxCount] = {};
BYTE Controller::prevKey[extraControls::maxCount] = {};
WORD Controller::prevbuttonState = { 0x0 };
WORD Controller::buttonState = { 0x0 };
void input_default()
{
}

Event::Event() : fn_ptr(&input_default)
{
}

void test_func()
{

}


Event::Event(std::string typ) : type(typ)
{
}

std::string name_container[4]{ "default_name", "key_released", "key_triggered", "key_held" };

void Event::operator()(int i)
{
	//if(type != name_container[0])
	//std::cout << i << "is " << type << std::endl;
	fn_ptr();
}


void input_mgr::UnRegisterFunc(std::function<void()> ai_fn, ActionsFn action_enum, input_type type)
{
	auto& container = inputSystem.keyboard_actions_array[type][action_enum].key_state;
	unsigned __int64 size = container.size();
	for (unsigned __int64 i = 0; i < size; ++i)
	{
		int key = inputSystem.keyboard_actions_array[type][action_enum].key_state[i].first;
		int state = inputSystem.keyboard_actions_array[type][action_enum].key_state[i].second;
		input_event_container[state][4 * key + type].fn_ptr = &input_default;
	}
	//container.DeleteAllEntries();
	UnRegisterControllerFunc(ai_fn, action_enum, type);
}

void input_mgr::UnRegisterControllerFunc(std::function<void()> ai_fn, ActionsFn action_enum, input_type type)
{
	auto& container = inputSystem.controller_actions_array[type][action_enum].key_state;
	unsigned __int64 size = container.size();
	for (unsigned __int64 i = 0; i < size; ++i)
	{
		int key = inputSystem.controller_actions_array[type][action_enum].key_state[i].first;
		int state = inputSystem.controller_actions_array[type][action_enum].key_state[i].second;
		controller_input_event_container[state][4 * key + type].fn_ptr = &input_default;
	}
	//container.DeleteAllEntries();
}


// for other systems to use
void input_mgr::RegisterFunc(std::function<void()> ai_fn, ActionsFn action_enum, input_type type)
{
	// ai_fn is the function that you want to bind.
	auto& container = inputSystem.keyboard_actions_array[type][action_enum].key_state; // finding the keys that are registered to this action
	
	unsigned __int64 size = container.size();
	for (unsigned __int64 i = 0; i < size; ++i) // going through each action and type, finding the right input_event_container element and assign the function ptr there
	{
		int key = inputSystem.keyboard_actions_array[type][action_enum].key_state[i].first;
		int state = inputSystem.keyboard_actions_array[type][action_enum].key_state[i].second;
		input_event_container[state][4 * key + type].fn_ptr = ai_fn;
	}
	RegisterControllerFunc(ai_fn, action_enum, type);
}

// for other systems to use
void input_mgr::RegisterControllerFunc(std::function<void()> ai_fn, ActionsFn action_enum, input_type type)
{
	// ai_fn is the function that you want to bind.
	auto& container = inputSystem.controller_actions_array[type][action_enum].key_state; // finding the keys that are registered to this action

	unsigned __int64 size = container.size();
	for (unsigned __int64 i = 0; i < size; ++i) // going through each action and type, finding the right input_event_container element and assign the function ptr there
	{
		int key = inputSystem.controller_actions_array[type][action_enum].key_state[i].first;
		int state = inputSystem.controller_actions_array[type][action_enum].key_state[i].second;
		controller_input_event_container[state][4 * key + type].fn_ptr = ai_fn;
	}
}

void input_mgr::RegisterKey(input_state state, input_type type, unsigned char key, std::function<void()> fn_ptr)
{
	//X_LOG("InputMgr::Key " << key << " -" << name_container[type] << "- mapped");
	input_event_container[state][4 * key + type].fn_ptr = fn_ptr;
}


// for input manager only. To bind function pointers to input
void input_mgr::RegisterKeyV2(input_state state, input_type type, unsigned char key, ActionsFn action_enum)
{

	// pass key into action
	inputSystem.keyboard_actions_array[type][action_enum].key_state.push_back(my_pair<int,int>(key, state));

	// no function is passed. this is a promise that the respective register function will be called by 
	// someone else.
}

void input_mgr::RegisterKeyVController(input_state state, input_type type, unsigned char key, ActionsFn action_enum)
{

	// pass key into action
	inputSystem.controller_actions_array[type][action_enum].key_state.push_back(my_pair<int, int>(key, state));

	// no function is passed. this is a promise that the respective register function will be called by 
	// someone else.
}

//Action Actions::actions_array[input_type::max_num][ActionsFn::MaxNum];
Event input_mgr::input_event_container[3][1024];
Event input_mgr::controller_input_event_container[3][xbox_controls::maxCount * 4];
Event Actions[input_type::max_num][ActionsFn::MaxNum];
Keyboard input_mgr::keyboardMouse;
Controller input_mgr::xboxController;
short Keyboard::wheelDelta;
HWND Keyboard::hwnd;
POINT Keyboard::prev_mousePos;
POINT Keyboard::curr_mousePos;
POINT Keyboard::mouse_delta = { 0, 0 };
POINT Keyboard::original_mousePos;
BYTE Keyboard::prevKeys[MAX_KEYS];
BYTE Keyboard::currentKeys[MAX_KEYS];
input_state input_mgr::current_state;
XINPUT_STATE Controller::gpState;;
bool Controller::invertLSY;
bool Controller::invertRSY;

bool Keyboard::anyKeyPressed = false;

void Keyboard::Init()
{
	hwnd = GetGameWindow()->hwnd;
	if (hwnd == nullptr)
	{
		//std::cout << "failed HWND - active " << std::endl;
		hwnd = GetForegroundWindow();
		GetWindow(hwnd, GW_HWNDPREV);
		//if (hwnd == nullptr)
		//	std::cout << "failed HWND - foreground" << std::endl;
	}
}

void ReadKeyboard(char* keys)
{
	for (int x = 0; x < 256; x++)
		keys[x] = (char)(GetAsyncKeyState(x) >> 8);
}

void Keyboard::Update()
{

	//if(wheelDelta)
	//std::cout << wheelDelta << std::endl;
	prev_mousePos = curr_mousePos;
	anyKeyPressed = false;
	GetCursorPos(&curr_mousePos); // not local coordinates, this is relative to the monitor, 0,0 at top left
	if (!ScreenToClient(hwnd, &curr_mousePos))
		std::cout << "failed conversion to local" << std::endl;

    
    mouse_delta.x = prev_mousePos.x - curr_mousePos.x;
    mouse_delta.y = prev_mousePos.y - curr_mousePos.y;

    if ( input_mgr::fps_mode ) {

        //X_LOG( "curr x: " << curr_mousePos.x << ", prev x: " << prev_mousePos.x << ", curr y: " << curr_mousePos.y << ", prev y: " << prev_mousePos.y );
        //X_LOG( "x: " << mouse_delta.x << ", y: " << mouse_delta.y );

        POINT midMouse{ midx, midy };
        prev_mousePos = curr_mousePos = midMouse;

        ClientToScreen( hwnd, &midMouse );
        SetCursorPos( midMouse.x, midMouse.y );
        
        if ( input_mgr::fps_mode < 3 ) {
            ++input_mgr::fps_mode;

            while ( ::ShowCursor( FALSE ) >= 0 );
        }
    }

    if (show_cursor) {
        while ( ::ShowCursor( TRUE ) < 0 );
        show_cursor = false;
    }


	//if (Controller::isConnected() && !ImGuiEditor::GetInstance().isPlaying)
	//	return;


	//float w = (float)gw->GetWidth();
	//float h = (float)gw->GetHeight();
	//
	//m_xPos = (int)(GET_X_LPARAM(lp) - (float)w / 2.0f);
	//m_yPos = (int)-(GET_Y_LPARAM(lp) - (float)h / 2.0f);
	//if (isHeld(VK_SPACE))
	//	std::cout << "x: " << curr_mousePos.x << "y: " << curr_mousePos.y << std::endl;

	std::copy(currentKeys, currentKeys + 256, prevKeys);
	GetKeyboardState(currentKeys);
	//ReadKeyboard(reinterpret_cast<char*>(currentKeys));
	int num;
	int j = 0;
	for (int i = 0; i < MAX_KEYS; ++i) // this loops through keyboard only
	{
		currentKeys[i] &= HIGH_BIT;
		currentKeys[i] /= 128;
		anyKeyPressed = currentKeys[i] ? true : anyKeyPressed;

		num = j + prevKeys[i] + 2 * currentKeys[i];
		j += 4;
		input_mgr::input_event_container[input_mgr::current_state][num](i);
	}	
}

bool Keyboard::IsTriggered(unsigned char key)
{
	return (currentKeys[key] && !prevKeys[key]);
}

bool Keyboard::IsReleased(unsigned char key)
{
	return (!currentKeys[key] && prevKeys[key]);
}

bool Keyboard::IsHeld(unsigned char key)
{
	return (currentKeys[key]) && (prevKeys[key]);
}

XINPUT_STATE* Controller::getState()
{
	return &gpState;
}
int Controller::getIndex()
{
	return gpIndex;
}
bool Controller::isConnected()
{
	return connected;
}
void Controller::setButtonDownCallback(std::function<void(xbox_controls::xbox_controls)> fn)
{
	buttonDownCallback = fn;
}
void Controller::setButtonUpCallback(std::function<void(xbox_controls::xbox_controls)> fn)
{
	buttonUpCallback = fn;
}
bool Controller::isButtonPressed(xbox_keys::xbox_keys b)
{
	return !!(buttonState & b);
}
bool Controller::isTriggered(xbox_keys::xbox_keys b)
{
  return !!((buttonState & b) && !(prevbuttonState & b));
}
bool Controller::isHeld(xbox_keys::xbox_keys b)
{
  return !!((buttonState & b) && (prevbuttonState & b));
}
bool Controller::isReleased(xbox_keys::xbox_keys b)
{
  return !!(!(buttonState & b) && (prevbuttonState & b));
}
void Controller::setInvertLStickY(bool b)
{
	invertLSY = b;
}
void Controller::setInvertRStickY(bool b)
{
	invertRSY = b;
}
float limit = 0.4f;
void Controller::Update() {
	ZeroMemory(&gpState, sizeof(XINPUT_STATE));
	bool prev_connected = connected;
	connected = (XInputGetState(gpIndex, &gpState) == ERROR_SUCCESS);
	if (prev_connected != connected)
	{
		if (prev_connected == false) // connect
		{
			input_mgr::SetFPSMode(true);
			/*input_mgr::fps_mode = 1;
			prev_cursor_visibility = true;*/
		}
		else if (prev_connected == true) // disconnect
		{
			
			if (!prev_cursor_visibility)
			{
				input_mgr::SetFPSMode(false);
				/*show_cursor = true;
				input_mgr::fps_mode = 0;
				prev_cursor_visibility = false;*/
			}
		}
			
	}
	if (connected)
	{
		std::copy(currKey, currKey + extraControls::maxCount, prevKey);

		currKey[extraControls::LT] = !!getLTrigger();
		int num = 40 + prevKey[extraControls::LT] + 2 * currKey[extraControls::LT];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);

		currKey[extraControls::RT] = !!getRTrigger();
		num = 44 + prevKey[extraControls::RT] + 2 * currKey[extraControls::RT];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);
		Vector2 tmp = getLStickPosition();
		currKey[extraControls::L_UP] = tmp.y > limit;
		num = 16*4 + prevKey[extraControls::L_UP] + 2 * currKey[extraControls::L_UP];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);

		currKey[extraControls::L_DOWN] = tmp.y< -limit;
		num = 17 * 4 + prevKey[extraControls::L_DOWN] + 2 * currKey[extraControls::L_DOWN];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);

		currKey[extraControls::L_LEFT] = tmp.x < -limit;
		num = 18 * 4 + prevKey[extraControls::L_LEFT] + 2 * currKey[extraControls::L_LEFT];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);

		currKey[extraControls::L_RIGHT] = tmp.x > limit;
		num = 19 * 4 + prevKey[extraControls::L_RIGHT] + 2 * currKey[extraControls::L_RIGHT];
		input_mgr::controller_input_event_container[input_mgr::current_state][num](0);

		

		//if (buttonState != gpState.Gamepad.wButtons)
		{												// button state is different from previous state
														// BITWISE XOR
														// getting what's different
			WORD currbuttonStateCopy = gpState.Gamepad.wButtons;
			WORD prevbuttonStateCopy = buttonState;
      prevbuttonState = buttonState;
			// looping through all button
			int counter = 0;
			for (WORD i{ 0x1 };; i <<= 0x1)
			{
				int num = counter + (prevbuttonStateCopy & 0x1) + 2 * (currbuttonStateCopy & 0x1);
	//			std::cout << num << std::endl;
				input_mgr::controller_input_event_container[input_mgr::current_state][num](i);
				if (i == 0x8000)
					break;

				currbuttonStateCopy >>= 0x1;
				prevbuttonStateCopy >>= 0x1;
				counter += 4;
			}
		}
	}

	// update of the state
	buttonState = gpState.Gamepad.wButtons;
}

Vector2 Controller::getLStickPosition() {
	float x = gpState.Gamepad.sThumbLX;
	float y = gpState.Gamepad.sThumbLY;
	if (abs(x) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		abs(y) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		x = 0.f; y = 0.f;
	}
	else 
	{
		x = float{ static_cast<float>(x) / static_cast<float>(SHRT_MAX) };
		y = float{ (invertLSY ? -1.0f : 1.0f) * static_cast<float>(y) / static_cast<float>(SHRT_MAX) };
		x = clamp(x, -1.0f, 1.0f);
		y = clamp(y, -1.0f, 1.0f); 
		
	};
	return Vector2(x, y);
}

Vector2 input_mgr::getRStickPos()
{
	return Controller::getRStickPosition();
}


Vector2 Controller::getRStickPosition() {
	float x = gpState.Gamepad.sThumbRX;
	float y = gpState.Gamepad.sThumbRY;
	
	if (abs(x) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		abs(y) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		x = 0.0f; y = 0.0f;
	}
	
	else {
	    x = float{ static_cast<float>(x) / static_cast<float>(SHRT_MAX) };
	    y = float{ (invertRSY ? -1.0f : 1.0f) * static_cast<float>(y) / static_cast<float>(SHRT_MAX) };
		{ clamp(x, -1.0f, 1.0f); clamp(y, -1.0f, 1.0f); }
	};
	return Vector2(x, y);
}

float Controller::getLTrigger() {
	auto x = gpState.Gamepad.bLeftTrigger;
	return (x > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? static_cast<float>(x) / 255.0f : 0.0f;
}

float Controller::getRTrigger() {
	auto x = gpState.Gamepad.bRightTrigger;
	return (x > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? static_cast<float>(x) / 255.0f : 0.0f;
}

void Controller::vibrate(float L, float R) {
	L = clamp(L, 0.0f, 1.0f);
	R = clamp(R, 0.0f, 1.0f);
	XINPUT_VIBRATION vState;
	ZeroMemory(&vState, sizeof(XINPUT_VIBRATION));
	int iL = static_cast<WORD>(L * 65535.0f + 0.4999f);
	int iR = static_cast<WORD>(R * 65535.0f + 0.4999f);
	vState.wLeftMotorSpeed = iL;
	vState.wRightMotorSpeed = iR;
	XInputSetState(gpIndex, &vState);
}



void input_mgr::Init()
{
	// sets all fn_ptr to default
	keyboardMouse.Init();
	int j = 0;
	int num_of_states = 3;
	for (int n = 0; n < num_of_states; ++n)
	{
		for (int i = 0; i < 1024; ++i)
		{
			input_event_container[n][i].type = name_container[j++];
			if (j > 3)
				j = 0;
		}
		j = 0;
		for (int i = 0; i < 76; ++i)
		{
			controller_input_event_container[n][i].type = name_container[j++];
			if (j > 3)
				j = 0;
		}
	}
	
	// mapping actual input to functions
	for (int i = 1; i <= 3; ++i)
	{
		input_type tp = input_type(i);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::A, ActionsFn::LEFT);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::D, ActionsFn::RIGHT);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::W, ActionsFn::UP);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::S, ActionsFn::DOWN);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::LCTRL, ActionsFn::CROUCH);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::TAB, ActionsFn::SPIRITMODE);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::SPACE, ActionsFn::SPIRITPULSE);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::T, ActionsFn::FLASHBANG);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::Q, ActionsFn::FLASHLIGHT);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::E, ActionsFn::INTERACT);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::J, ActionsFn::JOURNAL);
		input_mgr::RegisterKeyV2(input_state::game, tp, Key::F11, ActionsFn::FULLSCREEN);

		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::L_LEFT, ActionsFn::LEFT);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::L_RIGHT, ActionsFn::RIGHT);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::L_UP, ActionsFn::UP);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::L_DOWN, ActionsFn::DOWN);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_LSTICK, ActionsFn::CROUCH);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_RB, ActionsFn::SPIRITMODE);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_LB, ActionsFn::SPIRITPULSE);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_RT, ActionsFn::FLASHBANG);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_Y, ActionsFn::FLASHLIGHT);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_A, ActionsFn::INTERACT);
		input_mgr::RegisterKeyVController(input_state::game, tp, xboxController::XC_X, ActionsFn::JOURNAL);

	}
}
short input_mgr::GetWheelDelta()
{
	return keyboardMouse.wheelDelta;
}
void input_mgr::Update()
{
	// Update Windows
	Keyboard::wheelDelta = 0;

	if (active)
  {
		keyboardMouse.Update();
		//xboxController.Update();
	}
}

bool input_mgr::IsHeld(unsigned char key)
{
	return keyboardMouse.IsHeld(key);
}

bool input_mgr::IsTriggered(unsigned char key)
{
	return keyboardMouse.IsTriggered(key);
}

bool input_mgr::IsReleased(unsigned char key)
{
	return keyboardMouse.IsReleased(key);
}
int input_mgr::GetMouseX()
{
	return keyboardMouse.curr_mousePos.x;
}

int input_mgr::GetMouseY()
{
	return keyboardMouse.curr_mousePos.y;
}

input_mgr::input_mgr() noexcept
{
	// We want this job to run in the main thread
    input_mgr_ptr = this;
}



void input_mgr::WindowSize( int width, int height ) {

    window_width = width;
    window_height = height;

    midx = window_width / 2;
    midy = window_height / 2;
}

void input_mgr::SetFPSMode(bool mode) {

		prev_cursor_visibility = fps_mode > 0;
        fps_mode = mode ? 1 : 0;

        if ( mode ) {

            Keyboard::original_mousePos = Keyboard::curr_mousePos;

            Keyboard::prev_mousePos.x = Keyboard::curr_mousePos.x = midx;
            Keyboard::prev_mousePos.y = Keyboard::curr_mousePos.y = midy;
		
            Keyboard::mouse_delta.x = Keyboard::mouse_delta.y = 0;

        }
        else {

            ClientToScreen( Keyboard::hwnd, &Keyboard::original_mousePos );
            SetCursorPos( Keyboard::original_mousePos.x, Keyboard::original_mousePos.y );
            
            show_cursor = true;
        }
     

}
