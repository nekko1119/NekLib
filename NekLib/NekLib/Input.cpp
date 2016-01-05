#include <NekLib/Input.h>
#include <NekLib/Common.h>
#include <NekLib/ComPtr.h>

#include <boost/smart_ptr/shared_array.hpp>
using boost::shared_array;

#include <dinput.h>
using namespace NekLib;

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(push)
#pragma warning(disable : 4800)//'int' : �u�[���l�� 'true' �܂��� 'false' �ɋ����I�ɐݒ肵�܂�

//���z�{�^���\����
struct Button;

struct Input::Impl
{
	static const int MAX_INPUT_DEVICE_NUM = 16;//�ő哯���ڑ���

	static ComPtr<IDirectInput8> pDI;//Direct Input�C���^�[�t�F�C�X
	static ComPtr<IDirectInputDevice8> pDIDPad[MAX_INPUT_DEVICE_NUM];//�Q�[���p�b�h�̃f�o�C�X
	static ComPtr<IDirectInputDevice8> pDIDKey;	//�L�[�{�[�h�̃f�o�C�X
	static ComPtr<IDirectInputDevice8> pDIDMouse;	//�}�E�X�̃f�o�C�X

	static unsigned int maxInputDevice;	//���݂̍ő�ڑ���
	static unsigned int deviceCount;	//�ڑ���
	static bool isFirstInited;			//DirectInput�̏�������1�񂾂��s���΂����̂ŁA2��ڈȍ~�̓X�L�b�v����

	DIDEVCAPS didc;					//�f�o�C�X�̔\�͂��i�[����\����
	shared_array<Button> pButton;	//���z�{�^���i�{�^���̐������쐬�j
	const unsigned int id;			//���̃C���X�^���X������f�o�C�X�̔ԍ�

	int lX;			//�A�i���O�X�e�B�b�Nx���W�i�A�i���O�X�e�B�b�N������΁j
	int lY;			//�A�i���O�X�e�B�b�Ny���W�i����΁j
	int rX;			//�E�A�i���O�X�e�B�b�Nx���W�i����΁j
	int rY;			//�E�A�i���O�X�e�B�b�Ny���W�i����΁j

	Impl(int id);	//�R���X�g���N�^
	~Impl();		//�f�X�g���N�^

	bool CheckDirection(PAD p, DWORD d);//�����L�[�̃`�F�b�N


	static BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE lpdi, LPVOID lpvRef);//�f�o�C�X�̐���񋓂���֐�
	static BOOL CALLBACK EnumAxisesCallback(LPCDIDEVICEOBJECTINSTANCE lpdoi, LPVOID lpvRef);//���̐���񋓂���֐�
};

ComPtr<IDirectInput8> Input::Impl::pDI;
ComPtr<IDirectInputDevice8> Input::Impl::pDIDPad[Input::Impl::MAX_INPUT_DEVICE_NUM];
ComPtr<IDirectInputDevice8> Input::Impl::pDIDKey;
ComPtr<IDirectInputDevice8> Input::Impl::pDIDMouse;

unsigned int Input::Impl::maxInputDevice = 0U;
unsigned int Input::Impl::deviceCount = 0U;
bool Input::Impl::isFirstInited = false;


//���z�{�^���\����
struct Button
{
	BUTTON button;	//���̃{�^�����ǂ̉��z�{�^����\���Ă��邩
	KEY key;		//���̃{�^���ɐݒ肳��Ă���L�[�{�[�h�̃L�[
	PAD pad;		//���̃{�^���ɐݒ肳��Ă���p�b�h�̃{�^��
	DWORD time;		//���̃{�^�������~���b�����ꂽ����

	bool isDown;	//���̃{�^����������Ă��邩�ǂ���
	bool isPushed;	//�����ꂽ�u�Ԃ��ǂ���
	bool isReleased;//�����ꂽ�u�Ԃ��ǂ���

	Button();	//�R���X�g���N�^
};

Button::Button() :
button(BUTTON_NONE),
	key(KEY_NONE),
	pad(PAD_NONE),
	time(0U),
	isDown(false),
	isPushed(false),
	isReleased(false)
{
}

Input::Impl::Impl(int id) :
pButton(nullptr),
	id(id),
	lX(0),
	lY(0),
	rX(0),
	rY(0)
{}

Input::Impl::~Impl()
{
	for(unsigned int i=0; i < deviceCount; i++)
	{
		pDIDPad[i]->Unacquire();
	}

	pDIDKey->Unacquire();
	pDIDMouse->Unacquire();
}

//�����L�[���`�F�b�N����
bool Input::Impl::CheckDirection(PAD p, DWORD d)
{
	switch(p)
	{
	case PAD_DOWN:
		{
			return (d == 13500 || d == 18000 || d == 22500);
			break;
		}
	case PAD_UP:
		{
			return (d == 31500 || d == 0 || d == 4500);
			break;
		}
	case PAD_LEFT:
		{
			return (d == 22500 || d == 27000 || d == 31500);
			break;
		}
	case PAD_RIGHT:
		{
			return (d == 4500 || d == 9000 || d == 13500);
			break;
		}
	default:
		{
			return false;
		}
	}

	//��΂����ɂ͗��Ȃ����ǁA�ꉞ
	return false;
}

//�f�o�C�X�̐��𐔂���֐�
BOOL CALLBACK Input::Impl::EnumDevicesCallback(LPCDIDEVICEINSTANCE lpdi, LPVOID lpvRef)
{	
	Input* input = reinterpret_cast<Input*>(lpvRef);
	input->m_pImpl->didc.dwSize =  sizeof(DIDEVCAPS);
	Impl::pDI->CreateDevice(lpdi->guidInstance, Impl::pDIDPad[Impl::deviceCount].ToCreate(), nullptr);
	pDIDPad[deviceCount]->GetCapabilities(&(input->m_pImpl->didc));
	
	deviceCount++;//�J�E���g�𑝂₷
	
	//�����J�E���g�����݂̍ő�ڑ����ɂȂ�����A���ׂẴf�o�C�X���m�F�����Ƃ������ƂȂ̂�
	//�f�o�C�X�̗񋓂��I���
	if(deviceCount >= maxInputDevice)
	{
		return DIENUM_STOP;
	}

	//�����łȂ���Α�����
	return DIENUM_CONTINUE;
}

//���̐��𐔂���֐�
BOOL CALLBACK Input::Impl::EnumAxisesCallback(LPCDIDEVICEOBJECTINSTANCE lpdoi, LPVOID lpvRef)
{
	ComPtr<IDirectInputDevice8> pDevice(*(reinterpret_cast<ComPtr<IDirectInputDevice8>*>(lpvRef)));

	//���̐ݒ�
	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(dipr);			//���̍\���̂̃T�C�Y
	dipr.diph.dwHeaderSize = sizeof(dipr.diph);	//���̍\���̂̃w�b�_�idiph�j�̃T�C�Y
	dipr.diph.dwHow = DIPH_DEVICE;				//dwObj������\���Ă��邩
	dipr.diph.dwObj = 0U;						//����͉����\���Ă��Ȃ�
	dipr.lMax = +1000;							//���̒l�̍ő�l
	dipr.lMin = -1000;							//���̒l�̍ŏ��l
	
	//�ݒ�Ɏ��s������񋓂���߂�
	if(FAILED(pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

Input::Input(int id) : m_pImpl(new Impl(id))
{
}

Input* Input::Get(int id)
{
	int num = id-1;
	//�s���Ȉ����Ȃ�NULL��Ԃ�
	if(num < 0 || Impl::MAX_INPUT_DEVICE_NUM <= num)
	{
		return nullptr;
	}

	static std::shared_ptr<Input> instance[Impl::MAX_INPUT_DEVICE_NUM] = {nullptr};

	if(instance[num] != nullptr)
	{
		return instance[num].get();
	}
	else
	{
		instance[num].reset(new Input(num));
		return instance[num].get();
	}
}

bool Input::Init()
{
	//�{�^���̐��������z�{�^���\���̂��쐬
	m_pImpl->pButton.reset(new Button[PAD_NONE]);

	//��������Init���Ă΂ꂽ���Ƃ�����Ȃ�ȉ��͎��s���Ȃ�
	if(Impl::isFirstInited)
	{
		return true;
	}

	//���ݐڑ�����Ă���f�o�C�X�̐��𓾂�
	m_pImpl->maxInputDevice = joyGetNumDevs();

	//Direct Input�C���^�[�t�F�C�X�̍쐬
	DirectInput8Create
		(
		GetModuleHandle(nullptr),				//�Ώۂ̃A�v���P�[�V�����C���X�^���X�n���h��
		DIRECTINPUT_VERSION,					//DirectInput�o�[�W����
		IID_IDirectInput8,						//IDirectInput8��GUID�̃|�C���^
		reinterpret_cast<LPVOID*>(Impl::pDI.ToCreate()),//IDirectInput8�^�̃|�C���^
		nullptr									//�ʏ�NULL
		);

	if(!Impl::pDI)
	{
		return false;
	}

	//�L�[�{�[�h�f�o�C�X�̍쐬
	Impl::pDI->CreateDevice(GUID_SysKeyboard, Impl::pDIDKey.ToCreate(), nullptr);
	if(!Impl::pDIDKey)
	{
		return false;
	}
	//�}�E�X�f�o�C�X�̍쐬
	Impl::pDI->CreateDevice(GUID_SysMouse, Impl::pDIDMouse.ToCreate(), nullptr);
	if(!Impl::pDIDMouse)
	{
		return false;
	}

	//���ݐڑ�����Ă���f�o�C�X�̐��𒲂ׂ�
	if(FAILED(Impl::pDI->EnumDevices(
		DI8DEVCLASS_GAMECTRL,			//������f�o�C�X�̎�ށB�Q�[���p�b�h
		Impl::EnumDevicesCallback,		//���ۂɃf�o�C�X�𐔂���֐��̃|�C���^
		reinterpret_cast<LPVOID>(this),	//���̊֐��̑�2�����ɓn���l
		DIEDBSFL_ATTACHEDONLY			//������f�o�C�X�̏����B�C���X�g�[���ς݂ł����g�����Ԃ̂��̂̂�
		)))
	{
		return false;
	}

	//���ꂼ��̃f�o�C�X�ɂ��ď����ݒ���s��
	
	//�X�e�B�b�N�̎��̐ݒ�i��Ύ��A���Ύ��j
	DIPROPDWORD dipd;
	dipd.diph.dwSize = sizeof(dipd);			//���̍\���̂̃T�C�Y
	dipd.diph.dwHeaderSize = sizeof(dipd.diph);	//���̍\���̂̃w�b�_�iDIPROHEADER�^�j�̃T�C�Y
	dipd.diph.dwHow = DIPH_DEVICE;				//dwObj������\���Ă��邩
	dipd.diph.dwObj = 0U;						//����͉����\���Ă��Ȃ��Ƃ����Ӗ�
	dipd.dwData = DIPROPAXISMODE_ABS;			//���͐�Ύ����g��

	//���ׂẴf�o�C�X�̐ݒ�
	//�f�o�C�X���������z�{�^���z������
	for(unsigned int i=0; i<m_pImpl->deviceCount; i++)
	{
		//�f�o�C�X�̎�ނ̓p�b�h�ł�
		Impl::pDIDPad[i]->SetDataFormat(&c_dfDIJoystick);
		
		//���̃f�o�C�X�ւ̋������x���i���́j�B�őO�ʂɂ���Ƃ������L���ŁA����ȊO�̎��͑��̃v���Z�X�ɓn��
		Impl::pDIDPad[i]->SetCooperativeLevel(Window::Get()->HWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		//�f�o�C�X�̓���̃I�u�W�F�N�g�̐���񋓂��Đݒ肷��i����͎��j
		Impl::pDIDPad[i]->EnumObjects
			(
			Impl::EnumAxisesCallback,						//���ۂɎ���񋓂��Đݒ肷��֐��̃|�C���^
			reinterpret_cast<LPVOID>(&Impl::pDIDPad[i]),	//���̊֐��̑�2����
			DIDFT_AXIS										//����񋓂���
			);

		//�f�o�C�X�̎����[�h�̐ݒ�
		Impl::pDIDPad[i]->SetProperty(DIPROP_AXISMODE, &dipd.diph);

		//���͂̎擾�J�n
		Impl::pDIDPad[i]->Acquire();
	}

	//�L�[�{�[�h�̐ݒ�
	Impl::pDIDKey->SetDataFormat(&c_dfDIKeyboard);
	Impl::pDIDKey->SetCooperativeLevel(Window::Get()->HWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Impl::pDIDKey->Acquire();
	//�}�E�X�f�o�C�X�̐ݒ�
	Impl::pDIDMouse->SetDataFormat(&c_dfDIMouse);
	Impl::pDIDMouse->SetCooperativeLevel(Window::Get()->HWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Impl::pDIDMouse->Acquire();

	Impl::isFirstInited = true;

	return true;
}

bool Input::Update()
{
	//�f�o�C�XID���������Ȃ���΃f�o�C�X�̓`�F�b�N���Ȃ�
	bool hasDeviceID = (m_pImpl->id < Impl::deviceCount);

	DIJOYSTATE dijs;
	ZeroMemory(&dijs, sizeof(dijs));//�S��0�ŏ�����
	BYTE key[256] = {'\0'};

	//�E�C���h�E���A�N�e�B�u�Ȃ�
	if(Window::Get()->IsActive())
	{
		HRESULT hr[2];

		//�f�o�C�X�̎擾
		hr[0] = hasDeviceID ? Impl::pDIDPad[m_pImpl->id]->Acquire()
							: DI_OK;
		hr[1] = Impl::pDIDKey->Acquire();

		//�f�o�C�X���擾�ł�����
		if((hr[0] == DI_OK || hr[0] == S_FALSE) && (hr[1] == DI_OK || hr[1] == S_FALSE))
		{
			if(hasDeviceID)
			{
				Impl::pDIDPad[m_pImpl->id]->Poll();	//�p�b�h�����ɍs��
				hr[0] = hasDeviceID ? Impl::pDIDPad[m_pImpl->id]->GetDeviceState(sizeof(DIJOYSTATE), &dijs)
									: DI_OK;		//�{�^�����͂̎擾
			}

			hr[1] = Impl::pDIDKey->GetDeviceState(256, key);//�L�[�{�[�h���͂̎擾

			//���͂̎擾�ɐ���������
			if(hr[0] == DI_OK && hr[1] == DI_OK)
			{
				//�A�i���O�X�e�B�b�N�����ɍs��
				m_pImpl->lX = dijs.lX;
				m_pImpl->lY = dijs.lY;
				m_pImpl->rX = dijs.lZ;
				m_pImpl->rY = dijs.lRz;

				//�S�Ẵ{�^���ɂ��ē��͂�����
				for(int i=0; i<PAD_NONE; i++)
				{
					//�f�o�C�X���L���Ȃ�`�F�b�N�A�����Ȃ�L�[��������
					bool isHitKey = hasDeviceID ? (m_pImpl->CheckDirection(m_pImpl->pButton[i].pad, dijs.rgdwPOV[0])) ||//�p�b�h�̕����L�[
						((m_pImpl->pButton[i].pad < PAD_UP) && (dijs.rgbButtons[m_pImpl->pButton[i].pad] & 0x80)) ||	//�p�b�h�̃{�^��
						(key[m_pImpl->pButton[i].key] & 0x80)															//�L�[�{�[�h
												: (key[m_pImpl->pButton[i].key] & 0x80);								//�L�[�{�[�h

					//�L�[�������ꂽ���ǂ���
					if(isHitKey)
					{
						//�^�C�}�[��0�Ȃ獡�����ꂽ�u�ԂƂ�������
						if(m_pImpl->pButton[i].time == 0U)
						{
							m_pImpl->pButton[i].isPushed = true;
							m_pImpl->pButton[i].isReleased = false;
							m_pImpl->pButton[i].time = timeGetTime();
						}
						else
						{
							m_pImpl->pButton[i].isPushed = false;
						}

						m_pImpl->pButton[i].isDown = true;
					}
					//������Ă��Ȃ�
					else
					{
						//�^�C�}�[��0����Ȃ��Ƃ������Ƃ͍������ꂽ�Ƃ�������
						if(m_pImpl->pButton[i].time != 0)
						{
							m_pImpl->pButton[i].isReleased = true;
							m_pImpl->pButton[i].isPushed = false;
							m_pImpl->pButton[i].time = 0U;
						}
						else
						{
							m_pImpl->pButton[i].isReleased = false;
						}

						m_pImpl->pButton[i].isDown = false;
					}
				}//�i�S�Ẵ{�^���ɂ��ē��͂�����j
			}//�i���͂̎擾�ɐ���������j
		}//�i�f�o�C�X���擾�ł�����j
	}
	//�E�C���h�E����A�N�e�B�u�Ȃ�
	else
	{
		//�A�N�Z�X�����J������
		if(hasDeviceID)
		{
			Impl::pDIDPad[m_pImpl->id]->Unacquire();
		}
		Impl::pDIDKey->Unacquire();
		
		//�S�������Ă��Ȃ�����ɂ���
		for(int i=0; i<PAD_NONE; i++)
		{
			m_pImpl->pButton[i].isReleased = false;
			m_pImpl->pButton[i].isPushed = false;
			m_pImpl->pButton[i].isDown = false;
			m_pImpl->pButton[i].time = 0U;
		}
	}

	return true;
}

bool Input::SetButton(BUTTON button, KEY key, PAD pad)
{
	m_pImpl->pButton[button].button = button;
	m_pImpl->pButton[button].key = key;
	m_pImpl->pButton[button].pad = pad;
	
	return true;
}

bool Input::IsDown(BUTTON button) const
{
	return m_pImpl->pButton[button].isDown;
}

bool Input::IsPushed(BUTTON button) const
{
	return m_pImpl->pButton[button].isPushed;
}

bool Input::IsReleased(BUTTON button) const
{
	return m_pImpl->pButton[button].isReleased;
}

unsigned int Input::GetTime(BUTTON button) const
{
	return m_pImpl->pButton[button].time ? timeGetTime() - m_pImpl->pButton[button].time : 0U;
}

void Input::GetAnalogPad(int* pOutX, int* pOutY) const
{
	*pOutX = m_pImpl->lX;
	*pOutY = m_pImpl->lY;
}

void Input::GetAnalogPad2(int* pOutX, int* pOutY) const
{
	*pOutX = m_pImpl->rX;
	*pOutY = m_pImpl->rY;
}

#pragma warning(pop)