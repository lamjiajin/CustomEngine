#pragma once

//all mouse inputs
struct xboxController final
{
	static const unsigned char XC_UP             = 0x00;
	static const unsigned char XC_DOWN           = 0x01;
	static const unsigned char XC_LEFT           = 0x02;
	static const unsigned char XC_RIGHT          = 0x03;
	static const unsigned char XC_START          = 0x04;
	static const unsigned char XC_BACK           = 0x05;
	static const unsigned char XC_LSTICK         = 0x06;
	static const unsigned char XC_RSTICK         = 0x07;
	static const unsigned char XC_LB             = 0x08;
	static const unsigned char XC_RB             = 0x09;
	static const unsigned char XC_LT             = 0x0A;
	static const unsigned char XC_RT             = 0x0B;
	static const unsigned char XC_A              = 0x0C;
	static const unsigned char XC_B              = 0x0D;
	static const unsigned char XC_X              = 0x0E;
	static const unsigned char XC_Y              = 0x0F;
	static const unsigned char L_UP      = 0x10;
	static const unsigned char L_DOWN    = 0x11;
	static const unsigned char L_LEFT    = 0x12;
	static const unsigned char L_RIGHT   = 0x13;
};										   

//all mouse inputs
struct Button final
{
  static const unsigned char LEFTMOUSE = 0x01;
  static const unsigned char RIGHTMOUSE = 0x02;
  static const unsigned char MIDDLEMOUSE = 0x04;
};

//all keyboard inputs
struct Key final
{
  static const unsigned char BACKSPACE = 0x08;
  static const unsigned char TAB = 0x09;
  static const unsigned char ENTER = 0x0D;
  static const unsigned char LSHIFT = 0xA0;
  static const unsigned char RSHIFT = 0xA1;
  static const unsigned char LCTRL = 0xA2;
  static const unsigned char RCTRL = 0xA3;
  static const unsigned char LALT = 0xA4;
  static const unsigned char RALT = 0xA5;
  static const unsigned char PRINTSCREEN = 0x2C;
  static const unsigned char SCROLLLOCK = 0x91;
  static const unsigned char PAUSE = 0x13;
  static const unsigned char CAPSLOCK = 0x14;

  static const unsigned char ESCAPE = 0x1B;

  static const unsigned char SPACE = 0x20;
  static const unsigned char PAGEUP = 0x21;
  static const unsigned char PAGEDOWN = 0x22;
  static const unsigned char END = 0x23;
  static const unsigned char HOME = 0x24;
  static const unsigned char LEFT = 0x25;
  static const unsigned char UP = 0x26;
  static const unsigned char RIGHT = 0x27;
  static const unsigned char DOWN = 0x28;
  static const unsigned char INSERT = 0x2D;
  static const unsigned char DEL = 0x2E;

  static const unsigned char Key_0 = 0x30;
  static const unsigned char Key_1 = 0x31;
  static const unsigned char Key_2 = 0x32;
  static const unsigned char Key_3 = 0x33;
  static const unsigned char Key_4 = 0x34;
  static const unsigned char Key_5 = 0x35;
  static const unsigned char Key_6 = 0x36;
  static const unsigned char Key_7 = 0x37;
  static const unsigned char Key_8 = 0x38;
  static const unsigned char Key_9 = 0x39;

  static const unsigned char A = 0x41;
  static const unsigned char B = 0x42;
  static const unsigned char C = 0x43;
  static const unsigned char D = 0x44;
  static const unsigned char E = 0x45;
  static const unsigned char F = 0x46;
  static const unsigned char G = 0x47;
  static const unsigned char H = 0x48;
  static const unsigned char I = 0x49;
  static const unsigned char J = 0x4A;
  static const unsigned char K = 0x4B;
  static const unsigned char L = 0x4C;
  static const unsigned char M = 0x4D;
  static const unsigned char N = 0x4E;
  static const unsigned char O = 0x4F;
  static const unsigned char P = 0x50;
  static const unsigned char Q = 0x51;
  static const unsigned char R = 0x52;
  static const unsigned char S = 0x53;
  static const unsigned char T = 0x54;
  static const unsigned char U = 0x55;
  static const unsigned char V = 0x56;
  static const unsigned char W = 0x57;
  static const unsigned char X = 0x58;
  static const unsigned char Y = 0x59;
  static const unsigned char Z = 0x5A;

  static const unsigned char NUMPAD0 = 0x60;
  static const unsigned char NUMPAD1 = 0x61;
  static const unsigned char NUMPAD2 = 0x62;
  static const unsigned char NUMPAD3 = 0x63;
  static const unsigned char NUMPAD4 = 0x64;
  static const unsigned char NUMPAD5 = 0x65;
  static const unsigned char NUMPAD6 = 0x66;
  static const unsigned char NUMPAD7 = 0x67;
  static const unsigned char NUMPAD8 = 0x68;
  static const unsigned char NUMPAD9 = 0x69;

  static const unsigned char NUM_MULTIPLY = 0x6A;
  static const unsigned char NUM_PLUS = 0x6B;
  static const unsigned char NUM_ENTER = 0x6C;
  static const unsigned char NUM_MINUS = 0x6D;
  static const unsigned char NUM_PERIOD = 0x6E;
  static const unsigned char NUM_DIVIDE = 0x6F;
  static const unsigned char NUMLOCK = 0x90;

  static const unsigned char F1 = 0x70;
  static const unsigned char F2 = 0x71;
  static const unsigned char F3 = 0x72;
  static const unsigned char F4 = 0x73;
  static const unsigned char F5 = 0x74;
  static const unsigned char F6 = 0x75;
  static const unsigned char F7 = 0x76;
  static const unsigned char F8 = 0x77;
  static const unsigned char F9 = 0x78;
  static const unsigned char F10 = 0x79;
  static const unsigned char F11 = 0x7A;
  static const unsigned char F12 = 0x7B;

  static const unsigned char SEMICOLON = 0xBA;
  static const unsigned char SLASH = 0xBF;
  static const unsigned char BACKQUOTE = 0xC0;
  static const unsigned char LBRACKET = 0xDB;
  static const unsigned char BACKSLASH = 0XDC;
  static const unsigned char RBRACKET = 0xDD;
  static const unsigned char QUOTE = 0xDE;

  static const unsigned char EQUAL = 0xBB;
  static const unsigned char MINUS = 0xBD;
  static const unsigned char PERIOD = 0xBE;
  static const unsigned char COMMA = 0xBC;
};