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

//仮想ボタン
enum BUTTON
{
	BUTTON_1,		//ボタン1
	BUTTON_2,		//ボタン2
	BUTTON_3,		//　・
	BUTTON_4,		//	・
	BUTTON_5,		//	・
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
	BUTTON_28,		//　・
	BUTTON_29,		//	・
	BUTTON_30,		//　・
	BUTTON_31,		//ボタン31
	BUTTON_32,		//ボタン32
	BUTTON_UP,		//十字キー上
	BUTTON_DOWN,	//十字キー下
	BUTTON_LEFT,	//十字キー左
	BUTTON_RIGHT,	//十字キー右

	BUTTON_NONE	//未定義
};

//パッドのボタン
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

	PAD_NONE		//未設定
};

//キーボード定数
enum KEY
{
	KEY_ESCAPE = 0x01,		//エスケープ
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
	//KEY_EQUAL = 0x0D,		//=(存在しないので消した、ということを明示)
	KEY_BACKSPACE = 0x0E,	//バックスペース
	KEY_TAB = 0x0F,			//タブ
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
	KEY_RETURN = 0x1C,		//エンター
	KEY_LCONTROL = 0x1D,	//左コントロール
	KEY_A = 0x1E,			//A
	KEY_S = 0x1F,			//S
	KEY_D = 0x20,			//D
	KEY_F = 0x21,			//F
	KEY_G = 0x22,			//G
	KEY_H = 0x23,			//H
	KEY_J = 0x24,			//J
	KEY_K = 0x25,			//K
	KEY_L = 0x26,			//L
	KEY_SEMICOLON =0x27,	//セミコロン
	//KEY_APOSTROPHE = 0x28,//'（存在しないので消した、ということを明示）
	//KEY_GRAVE = 0x29,		//グレーブ（存在しないので消した、と言うことを明示）
	KEY_LSHIFT = 0x2A,		//左シフト
	KEY_BACKSLASH = 0x2B,	//＼
	KEY_Z = 0x2C,			//Z
	KEY_X = 0x2D,			//X
	KEY_C = 0x2E,			//C
	KEY_V = 0x2F,			//V
	KEY_B = 0x30,			//B
	KEY_N = 0x31,			//N
	KEY_M = 0x32,			//M
	KEY_COMMA = 0x33,		//,
	KEY_PERIOD = 0x34,		//.
	KEY_SLASH = 0x35,		//／
	KEY_RSHIFT = 0x36,		//右シフト
	//KEY_MULTIPLY = 0x37,	//*（存在しないので消した、ということを明示）
	KEY_LALT = 0x38,		//左オルト
	KEY_SPACE = 0x39,		//スペース
	KEY_CAPSLOCK = 0x3A,	//CAPSLOCK英数
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
	KEY_NUMPAD7 = 0x47,		//テンキー7
	KEY_NUMPAD8 = 0x48,		//テンキー8
	KEY_NUMPAD9 = 0x49,		//テンキー9
	KEY_SUBTRACT = 0x4A,	//テンキー-
	KEY_NUMPAD4 = 0x4B,		//テンキー4
	KEY_NUMPAD5 = 0x4C,		//テンキー5
	KEY_NUMPAD6 = 0x4D,		//テンキー6
	KEY_ADD = 0x4E,			//テンキー+
	KEY_NUMPAD1 = 0x4F,		//テンキー1
	KEY_NUMPAD2 = 0x50,		//テンキー2
	KEY_NUMPAD3 = 0x51,		//テンキー3
	KEY_NUMPAD0 = 0x52,		//テンキー0
	KEY_DECIMAL = 0x53,		//テンキー.
	//KEY_OEM_102 = 0x56,	//存在しないので消した、ということを明示
	KEY_F11 = 0x57,			//F11
	KEY_F12 = 0x58,			//F12
	//KEY_F13 = 0x64,		//F13（存在しないので消した、ということを明示）
	//KEY_F14 = 0x65,		//F14（存在しないので消した、ということを明示）
	//KEY_F15 = 0x66,		//F15（存在しないので消した、ということを明示）
	KEY_KANA = 0x70,		//カナ
	//KEY_ABNT_C1 = 0x73,	//ブラジルキーボードの'?'（存在しないので消した）
	KEY_CONVERT = 0x79,		//変換
	KEY_NOCONVERT = 0x7B,	//無変換
	KEY_YEN = 0x7D,			//'\'
	//KEY_ABNT_C2 = 0x7E,	//ブラジルキーボードのテンキー（存在しないので消した）
	KEY_NUMPADEQUALS = 0x8D,//テンキー=
	KEY_PREVTRACK = 0x90,	//^
	KEY_AT = 0x91,			//@
	KEY_COLON = 0x92,		//:
	KEY_UNDERLINE = 0x93,	//_（存在しないので消した、ということを明示）
	KEY_KANJI = 0x94,		//漢字
	//KEY_STOP = 0x95,		//ストップ（存在しないので消した、ということを明示）
	//KEY_AX = 0x96,		//アックス（存在しないのでkした、ということを明示）
	//KEY_UNLABELED = 0x97,	//（存在しないので消した、ということを明示）
	KEY_NEXTTRACK = 0x99,	//（存在しないので消した、ということを明示）
	KEY_NUMPADENTER = 0x9C,	//テンキーエンター
	KEY_RCONTROL = 0x9D,	//右コントロール
	//KEY_MUTE = 0xA0,		//ミュート（存在しないので消した、ということを明示）
	//KEY_CALCULATOR = 0xA1,//カリキュレータ（存在しないので消した、ということを明示）
	//KEY_PLAYPAUSE = 0xA2,	//プレイ、ミュート（存在しないので消した、ということを明示）
	//KEY_MEDIASTOP = 0xA4,	//メディアストップ（存在しないので消した、ということを明示）
	//KEY_VOLUMEDOWN = 0xAE,//ボリュームダウン（存在しないので消した、ということを明示）
	//KEY_VOLUMEUP = 0xB0,	//ボリュームアップ（存在しないので消した、ということを明示）
	//KEY_WEBHOME = 0xB2,	//webホーム（存在しないので消した、ということを明示）
	//KEY_NUMPADCOMMA = 0xB3,//テンキー,（存在しないので消した、ということを明示）
	KEY_DIVIDE = 0xB5,		//テンキー/
	KEY_SYSRQ = 0xB7,		//PrintScreen
	KEY_RMENU = 0xB8,		//右メニュー存在しないので消した、ということを明示）
	KEY_PAUSE = 0xC5,		//PauseBreak
	KEY_HOME = 0xC7,		//ホーム
	KEY_UP = 0xC8,			//上
	KEY_PGUP = 0xC9,		//PGUP PRIOR
	KEY_LEFT = 0xCB,		//左
	KEY_RIGHT = 0xCD,		//右
	KEY_END	= 0xCF,			//エンド
	KEY_DOWN = 0xD0,		//下
	KEY_PGDN = 0xD1,		//PGDOWN NEXT
	KEY_INSERT = 0xD2,		//インサート
	KEY_DELETE = 0xD3,		//デリート

	KEY_NONE =0x00			//どのキーでもない
};

}//namespace NekLib

#endif