#ifndef NEKLIB_INPUT_H
#define NEKLIB_INPUT_H

#include <memory>//for std::unique_ptr

namespace NekLib
{

enum BUTTON;
enum KEY;
enum PAD;

class Input
{
public:
	static Input* Get(int id);

	bool Init();
	bool Update();
	bool SetButton(BUTTON button, KEY key, PAD pad);

	bool IsDown(BUTTON button) const;
	bool IsPushed(BUTTON button) const;
	bool IsReleased(BUTTON button) const;
	unsigned int GetTime(BUTTON button) const;
	void GetAnalogPad(int* pOutX, int* pOutY) const;
	void GetAnalogPad2(int* pOutX, int* pOutY) const;

private:
	explicit Input(int id);
	Input(const Input& rhs);
	Input& operator=(const Input& rhs);

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
};

//���z�{�^��
enum BUTTON
{
	BUTTON_1,		//�{�^��1
	BUTTON_2,		//�{�^��2
	BUTTON_3,		//�@�E
	BUTTON_4,		//	�E
	BUTTON_5,		//	�E
	BUTTON_6,		//
	BUTTON_7,		//
	BUTTON_8,		//
	BUTTON_9,
	BUTTON_10,
	BUTTON_11,
	BUTTON_12,
	BUTTON_13,
	BUTTON_14,
	BUTTON_15,
	BUTTON_16,
	BUTTON_17,
	BUTTON_18,
	BUTTON_19,
	BUTTON_20,
	BUTTON_21,
	BUTTON_22,
	BUTTON_23,
	BUTTON_24,
	BUTTON_25,		//
	BUTTON_26,		//
	BUTTON_27,		//
	BUTTON_28,		//�@�E
	BUTTON_29,		//	�E
	BUTTON_30,		//�@�E
	BUTTON_31,		//�{�^��31
	BUTTON_32,		//�{�^��32
	BUTTON_UP,		//�\���L�[��
	BUTTON_DOWN,	//�\���L�[��
	BUTTON_LEFT,	//�\���L�[��
	BUTTON_RIGHT,	//�\���L�[�E

	BUTTON_NONE	//����`
};

//�p�b�h�̃{�^��
enum PAD
{
	PAD_1,
	PAD_2,
	PAD_3,
	PAD_4,
	PAD_5,
	PAD_6,
	PAD_7,
	PAD_8,
	PAD_9,
	PAD_10,
	PAD_11,
	PAD_12,
	PAD_13,
	PAD_14,
	PAD_15,
	PAD_16,
	PAD_17,
	PAD_18,
	PAD_19,
	PAD_20,
	PAD_21,
	PAD_22,
	PAD_23,
	PAD_24,
	PAD_25,
	PAD_26,
	PAD_27,
	PAD_28,
	PAD_29,
	PAD_30,
	PAD_31,
	PAD_32,
	PAD_UP,
	PAD_DOWN,
	PAD_LEFT,
	PAD_RIGHT,

	PAD_NONE		//���ݒ�
};

//�L�[�{�[�h�萔
enum KEY
{
	KEY_ESCAPE = 0x01,		//�G�X�P�[�v
	KEY_1 = 0x02,			//1
	KEY_2 = 0x03,			//2
	KEY_3 = 0x04,			//3
	KEY_4 = 0x05,			//4
	KEY_5 = 0x06,			//5
	KEY_6 = 0x07,			//6
	KEY_7 = 0x08,			//7
	KEY_8 = 0x09,			//8
	KEY_9 = 0x0A,			//9
	KEY_0 = 0x0B,			//0
	KEY_MINUS = 0x0C,		//-
	//KEY_EQUAL = 0x0D,		//=(���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎�)
	KEY_BACKSPACE = 0x0E,	//�o�b�N�X�y�[�X
	KEY_TAB = 0x0F,			//�^�u
	KEY_Q = 0x10,			//Q
	KEY_W = 0x11,			//W
	KEY_E = 0x12,			//E
	KEY_R = 0x13,			//R
	KEY_T = 0x14,			//T
	KEY_Y = 0x15,			//Y
	KEY_U = 0x16,			//U
	KEY_I = 0x17,			//I
	KEY_O = 0x18,			//O
	KEY_P = 0x19,			//P
	KEY_LBRACKET = 0x1A,	//[
	KEY_RBRACKET = 0x1B,	//]
	KEY_RETURN = 0x1C,		//�G���^�[
	KEY_LCONTROL = 0x1D,	//���R���g���[��
	KEY_A = 0x1E,			//A
	KEY_S = 0x1F,			//S
	KEY_D = 0x20,			//D
	KEY_F = 0x21,			//F
	KEY_G = 0x22,			//G
	KEY_H = 0x23,			//H
	KEY_J = 0x24,			//J
	KEY_K = 0x25,			//K
	KEY_L = 0x26,			//L
	KEY_SEMICOLON =0x27,	//�Z�~�R����
	//KEY_APOSTROPHE = 0x28,//'�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_GRAVE = 0x29,		//�O���[�u�i���݂��Ȃ��̂ŏ������A�ƌ������Ƃ𖾎��j
	KEY_LSHIFT = 0x2A,		//���V�t�g
	KEY_BACKSLASH = 0x2B,	//�_
	KEY_Z = 0x2C,			//Z
	KEY_X = 0x2D,			//X
	KEY_C = 0x2E,			//C
	KEY_V = 0x2F,			//V
	KEY_B = 0x30,			//B
	KEY_N = 0x31,			//N
	KEY_M = 0x32,			//M
	KEY_COMMA = 0x33,		//,
	KEY_PERIOD = 0x34,		//.
	KEY_SLASH = 0x35,		//�^
	KEY_RSHIFT = 0x36,		//�E�V�t�g
	//KEY_MULTIPLY = 0x37,	//*�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_LALT = 0x38,		//���I���g
	KEY_SPACE = 0x39,		//�X�y�[�X
	KEY_CAPSLOCK = 0x3A,	//CAPSLOCK�p��
	KEY_F1 = 0x3B,			//F1
	KEY_F2 = 0x3C,			//F2
	KEY_F3 = 0x3D,			//F3
	KEY_F4 = 0x3E,			//F4
	KEY_F5 = 0x3F,			//F5
	KEY_F6 = 0x40,			//F6
	KEY_F7 = 0x41,			//F7
	KEY_F8 = 0x42,			//F8
	KEY_F9 = 0x43,			//F9
	KEY_F10 = 0x44,			//F10
	KEY_NUMLOCK = 0x45,		//NumLock
	KEY_SCROLL = 0x46,		//ScrollLock
	KEY_NUMPAD7 = 0x47,		//�e���L�[7
	KEY_NUMPAD8 = 0x48,		//�e���L�[8
	KEY_NUMPAD9 = 0x49,		//�e���L�[9
	KEY_SUBTRACT = 0x4A,	//�e���L�[-
	KEY_NUMPAD4 = 0x4B,		//�e���L�[4
	KEY_NUMPAD5 = 0x4C,		//�e���L�[5
	KEY_NUMPAD6 = 0x4D,		//�e���L�[6
	KEY_ADD = 0x4E,			//�e���L�[+
	KEY_NUMPAD1 = 0x4F,		//�e���L�[1
	KEY_NUMPAD2 = 0x50,		//�e���L�[2
	KEY_NUMPAD3 = 0x51,		//�e���L�[3
	KEY_NUMPAD0 = 0x52,		//�e���L�[0
	KEY_DECIMAL = 0x53,		//�e���L�[.
	//KEY_OEM_102 = 0x56,	//���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎�
	KEY_F11 = 0x57,			//F11
	KEY_F12 = 0x58,			//F12
	//KEY_F13 = 0x64,		//F13�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_F14 = 0x65,		//F14�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_F15 = 0x66,		//F15�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_KANA = 0x70,		//�J�i
	//KEY_ABNT_C1 = 0x73,	//�u���W���L�[�{�[�h��'?'�i���݂��Ȃ��̂ŏ������j
	KEY_CONVERT = 0x79,		//�ϊ�
	KEY_NOCONVERT = 0x7B,	//���ϊ�
	KEY_YEN = 0x7D,			//'\'
	//KEY_ABNT_C2 = 0x7E,	//�u���W���L�[�{�[�h�̃e���L�[�i���݂��Ȃ��̂ŏ������j
	KEY_NUMPADEQUALS = 0x8D,//�e���L�[=
	KEY_PREVTRACK = 0x90,	//^
	KEY_AT = 0x91,			//@
	KEY_COLON = 0x92,		//:
	KEY_UNDERLINE = 0x93,	//_�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_KANJI = 0x94,		//����
	//KEY_STOP = 0x95,		//�X�g�b�v�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_AX = 0x96,		//�A�b�N�X�i���݂��Ȃ��̂�k�����A�Ƃ������Ƃ𖾎��j
	//KEY_UNLABELED = 0x97,	//�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_NEXTTRACK = 0x99,	//�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_NUMPADENTER = 0x9C,	//�e���L�[�G���^�[
	KEY_RCONTROL = 0x9D,	//�E�R���g���[��
	//KEY_MUTE = 0xA0,		//�~���[�g�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_CALCULATOR = 0xA1,//�J���L�����[�^�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_PLAYPAUSE = 0xA2,	//�v���C�A�~���[�g�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_MEDIASTOP = 0xA4,	//���f�B�A�X�g�b�v�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_VOLUMEDOWN = 0xAE,//�{�����[���_�E���i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_VOLUMEUP = 0xB0,	//�{�����[���A�b�v�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_WEBHOME = 0xB2,	//web�z�[���i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	//KEY_NUMPADCOMMA = 0xB3,//�e���L�[,�i���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_DIVIDE = 0xB5,		//�e���L�[/
	KEY_SYSRQ = 0xB7,		//PrintScreen
	KEY_RMENU = 0xB8,		//�E���j���[���݂��Ȃ��̂ŏ������A�Ƃ������Ƃ𖾎��j
	KEY_PAUSE = 0xC5,		//PauseBreak
	KEY_HOME = 0xC7,		//�z�[��
	KEY_UP = 0xC8,			//��
	KEY_PGUP = 0xC9,		//PGUP PRIOR
	KEY_LEFT = 0xCB,		//��
	KEY_RIGHT = 0xCD,		//�E
	KEY_END	= 0xCF,			//�G���h
	KEY_DOWN = 0xD0,		//��
	KEY_PGDN = 0xD1,		//PGDOWN NEXT
	KEY_INSERT = 0xD2,		//�C���T�[�g
	KEY_DELETE = 0xD3,		//�f���[�g

	KEY_NONE =0x00			//�ǂ̃L�[�ł��Ȃ�
};

}//namespace NekLib

#endif