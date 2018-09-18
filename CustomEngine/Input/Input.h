#ifndef INPUT_H
#define INPUT_H

#include "Keys.h"
#include <Windows.h>
#include <functional>
#include <Xinput.h>
#include <vector>
#include "Math/MathInc.h"

#include <iostream>
#include <algorithm>



#define HIGH_BIT 0x80 // 10000000 
#define LOW_BIT  0x01 // 00000001
#if defined( _M_X64 )
#define _AMD64_
#elif defined ( _M_IX86 )
#define _X86_
#elif defined( _M_ARM )
#define _ARM_
#endif
// available for public to use



namespace xbox_keys
{
  enum xbox_keys {
    XC_UP = XINPUT_GAMEPAD_DPAD_UP,
    XC_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
    XC_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
    XC_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
    XC_START = XINPUT_GAMEPAD_START,
    XC_BACK = XINPUT_GAMEPAD_BACK,
    XC_LSTICK = XINPUT_GAMEPAD_LEFT_THUMB,
    XC_RSTICK = XINPUT_GAMEPAD_RIGHT_THUMB,
    XC_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
    XC_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XC_A = XINPUT_GAMEPAD_A,
    XC_B = XINPUT_GAMEPAD_B,
    XC_X = XINPUT_GAMEPAD_X,
    XC_Y = XINPUT_GAMEPAD_Y,
  };
}
// dont touch this. usage only for input internal functions
namespace xbox_controls
{
  enum xbox_controls {
    //XC_UP = XINPUT_GAMEPAD_DPAD_UP,
    //XC_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
    //XC_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
    //XC_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
    //XC_START = XINPUT_GAMEPAD_START,
    //XC_BACK = XINPUT_GAMEPAD_BACK,
    //XC_LSTICK = XINPUT_GAMEPAD_LEFT_THUMB,
    //XC_RSTICK = XINPUT_GAMEPAD_RIGHT_THUMB,
    //XC_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
    //XC_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
    //XC_A = XINPUT_GAMEPAD_A,
    //XC_B = XINPUT_GAMEPAD_B,
    //XC_X = XINPUT_GAMEPAD_X,
    //XC_Y = XINPUT_GAMEPAD_Y,
    XC_UP = 0,//1
    XC_DOWN,//2
    XC_LEFT,//3
    XC_RIGHT,//4
    XC_START,//5
    XC_BACK,//6
    XC_LSTICK,//7
    XC_RSTICK,//8
    XC_LB,//9
    XC_RB,//10
    XC_LT,//11 
    XC_RT,//12
    XC_A,//13
    XC_B,//14
    XC_X,//15
    XC_Y,//16
    XC_L_UP,
    XC_L_DOWN,
    XC_L_LEFT,
    XC_L_RIGHT,
    maxCount
  };
}
enum input_state { game, pause_menu, main_menu };

enum input_type: int { default_name, key_released, key_triggered, key_held, max_num };


namespace extraControls
{
	enum extraControls { LT = 0, RT, L_UP, L_DOWN, L_LEFT, L_RIGHT, maxCount };
}

class Controller {
public:
	// These values represent the buttons on the gamepad.

	/* If you have multiple gamepads, create multiple instances of the class using different indexes.
	The connected gamepads are numbered 0-4. The default argument is 0, meaning this instance
	will take control of the first (or only one) gamepad connected to the system.*/
	Controller(int index = 0) : gpIndex{ index }, buttonDownCallback{ nullptr }, buttonUpCallback{ nullptr }{}

	/* Set a function (of type 'void', with a 'button_t' argument) to be called each time a button is pressed on the gamepad.
	The value of the argument should be checked against values of the 'button_t' enum to determine which button was pressed.*/
	void setButtonDownCallback(std::function<void(xbox_controls::xbox_controls)> fn);
	/* Set a function (of type 'void', with a 'button_t' argument) to be called each time a button is released on the gamepad.
	The value of the argument should be checked against values of the 'button_t' enum to determine which button was released.*/
	void setButtonUpCallback(std::function<void(xbox_controls::xbox_controls)> fn);
	/* Ths update function should be called every cycle of your app (before any other member calls) to keep things up to date.
	NOTE: All data that other member functions return is actually read from the device at the time you call this function, and will not be updated until you call 'update()' again.*/
	void Update();
	/* Indicates if the gamepad is connected or not. This should be checked before fetching any data.*/
	static bool isConnected();
	/* Indicates if a button (specified by the argument) is currently pressed or not (well, at the time of the last 'update()' call anyway).*/
	bool isButtonPressed(xbox_keys::xbox_keys b);
	/* Returns the position of the left analog stick. '.x' and '.y' range from -1.0 to 1.0.*/
	static Vector2 getLStickPosition();
	/* Returns the position of the right analog stick. '.x' and '.y' range from -1.0 to 1.0.*/
	static Vector2 getRStickPosition();
	/* Returns the position of the left trigger. Range is 0.0 (not pressed) to 1.0 (pressed all the way).*/
	static float getLTrigger();
	/* Returns the position of the right trigger. Range is 0.0 (not pressed) to 1.0 (pressed all the way).*/
	static float getRTrigger();
	/* Vibrates the gamepad. 'L' and 'R' define the vibration intensity of the left and right motors.
	They should be in the 0.0 to 1.0 range. NOTE: to turn vibrations off, you must call this function again with both arguments being 0.0.*/
	void vibrate(float L, float R);
	/* If set to true, the Y-axis of the left analog stick will be inverted.*/
	void setInvertLStickY(bool b);
	/* If set to true, the Y-axis of the right analog stick will be inverted.*/
	void setInvertRStickY(bool b);

	// Only use this if you want to use some XInput functionality directly. It returns a pointer to the current state.
	XINPUT_STATE* getState();
	// Returns the gamepad's index (the argument the constructor was given). Kind of pointless, but whatever.
	int getIndex();

  static bool isTriggered(xbox_keys::xbox_keys);
  static bool isHeld(xbox_keys::xbox_keys);
  static bool isReleased(xbox_keys::xbox_keys);
  static bool isTriggered(extraControls::extraControls c) { return currKey[c] && !prevKey[c]; }
  static bool isHeld(extraControls::extraControls c ) { return currKey[c] && prevKey[c]; }
  static bool isReleased(extraControls::extraControls c) { return !currKey[c] && prevKey[c]; }
private:

	static BYTE currKey[extraControls::maxCount];
	static BYTE prevKey[extraControls::maxCount];
	std::function<void(xbox_controls::xbox_controls)> buttonDownCallback;
	std::function<void(xbox_controls::xbox_controls)> buttonUpCallback;
	static XINPUT_STATE gpState;
	int gpIndex;
  static WORD prevbuttonState;
	static WORD buttonState;
	static bool connected;
	static bool invertLSY;
	static bool invertRSY;
};

class Event
{
public:
	std::string type;
	Event();
	Event(std::string name);
	void operator()(int i);
	std::function<void()> fn_ptr;
};

template<typename t, typename n>
struct my_pair
{
	my_pair(t fir, n sec) :first(fir), second(sec) {}

	t first;
	n second;
};



class Action
{
public:
	Action()  {}
	std::vector<my_pair<int,int> > key_state;

};


enum ActionsFn
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	CROUCH,
	INTERACT,
	FLASHBANG,
	SPIRITMODE,
	SPIRITPULSE,
	FLASHLIGHT,
	JOURNAL,
	FULLSCREEN,
	ESCAPE,
	MaxNum
};

struct Actions
{
	//static Action actions_array[input_type::max_num][ActionsFn::MaxNum];
	//static void CallAction(input_type it, ActionsFn af) { Actions::actions_array[it][af].fn_ptr(); }
};





struct Keyboard
{
	static HWND hwnd;
	static const int MAX_KEYS = 256;
	static BYTE currentKeys[MAX_KEYS];
	static BYTE prevKeys[MAX_KEYS];
	static bool anyKeyPressed;
	static short wheelDelta;
	static POINT prev_mousePos;
	static POINT curr_mousePos;
    static POINT mouse_delta;
    static POINT original_mousePos;
	static bool IsTriggered(unsigned char);
	static bool IsReleased(unsigned char key);
	static bool IsHeld(unsigned char key);
	static void Init();
	static void Update();
};

class input_mgr
{
public:
#define CONTROLLER_CROUCH xbox_keys::XC_LSTICK 
#define KEYBOARD_CROUCH	  Key::LCTRL
#define CONTROLLER_JOURNAL xbox_keys::XC_X 
#define KEYBOARD_JOURNAL	  Key::J
#define CONTROLLER_INTERACT xbox_keys::XC_A 
#define KEYBOARD_INTERACT	  Key::E
#define CONTROLLER_FLASHLIGHT xbox_keys::XC_Y 
#define KEYBOARD_FLASHLIGHT	  Key::Q
#define CONTROLLER_TOGGLESPIRITMODE  xbox_keys::XC_RB 
#define KEYBOARD_TOGGLESPIRITMODE	  Key::TAB
#define KEYBOARD_FLASHBANG  Key::T
#define CONTROLLER_FLASHBANG extraControls::RT
#define CONTROLLER_PULSE  xbox_keys::XC_LB 
#define KEYBOARD_PULSE	  Key::SPACE
#define CONTROLLER_TORCH  xbox_keys::XC_Y 
#define KEYBOARD_TORCH	  Key::Q
	Action keyboard_actions_array[input_type::max_num][ActionsFn::MaxNum];
	Action controller_actions_array[input_type::max_num][ActionsFn::MaxNum];
	static Keyboard keyboardMouse;
	static Controller xboxController;
	static Event input_event_container[3][1024];
	static Event controller_input_event_container[3][xbox_controls::maxCount*4];          
	static void Init();
	static void Update();
	static void UnRegisterFunc(std::function<void()> ai_fn, ActionsFn, input_type);

	static void UnRegisterControllerFunc(std::function<void()> ai_fn, ActionsFn action_enum, input_type type);
	static void RegisterControllerFunc(std::function<void()> ai_fn, ActionsFn, input_type);
	static void RegisterFunc(std::function<void()> ai_fn, ActionsFn, input_type);
	static void RegisterKey(input_state, input_type, unsigned char key, std::function<void()> fn_ptr);
	static Vector2 getRStickPos();
	static void RegisterKeyV2(input_state, input_type, unsigned char key, ActionsFn fn_ptr);
	static void RegisterKeyVController(input_state, input_type, unsigned char key, ActionsFn fn_ptr);
	static bool IsHeld(unsigned char key);
	static bool IsTriggered(unsigned char key);
	static bool IsReleased(unsigned char key);
	static short GetWheelDelta();
	static int GetMouseX();
	static int GetMouseY();
	static POINT GetRightThumbDelta()
	{
		Vector2 tmp = Controller::getRStickPosition(); POINT b; b.x = static_cast<LONG>(tmp.x * -80.f); b.y = static_cast<LONG>(tmp.y * 80.f); 
		return b;	
	}
    inline static POINT GetMouseDelta() { return 0 < fps_mode && fps_mode < 3 ? POINT() : Keyboard::mouse_delta; }

	static input_state current_state;
    static bool active;

    void SetWindowSize( int width, int height )
	{
        WindowSize( width, height ); 
    }

    static void SetFPSMode( bool mode );

    inline static input_mgr* Get() { return input_mgr_ptr; }


	input_mgr() noexcept;



    friend Keyboard;

	template<typename FN>
	static void CallFn(FN func) { func(); }

    static input_mgr* input_mgr_ptr;
    static void WindowSize(int width, int height);

    static int fps_mode;
};


#endif



