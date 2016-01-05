#ifndef NEKLIB_IMAGE_H
#define NEKLIB_IMAGE_H

#include <NekLib/IResource.h>
#include <NekLib/Vector.h>
#include <memory>//for shared_ptr

namespace NekLib
{
enum BLENDMODE;
enum ALPHAMODE;

class Image : public IResource
{
public:
	Image();
	explicit Image(const char* fileName);
	Image(const Image& rhs);
	Image& operator=(const Image& rhs);
	virtual ~Image();
	void Swap(Image& rhs);

	//�t�@�C�����J������
	bool Open(const char* fileName, int xNum, int yNum);
	bool Open(const char* fileName);
	bool Close();

	//�����摜�̕`��
	bool Draw(int num, const Vector& position, float angle, float rate, const Vector& center, bool isTrans = false);
	bool Draw(int num, const Vector& position, float angle, float rate, bool isTrans = false);
	bool Draw(int num, const Vector& position, bool isTrans = false);

	//�`��
	bool Draw(const Vector& position, float angle, float rate, const Vector& center, bool isTrans = false);
	bool Draw(const Vector& position, float angle, float rate, bool isTrans = false);
	bool Draw(const Vector& position, bool isTrans = false);

	//�A�N�Z�T�[

	void BlendMode(BLENDMODE blendMode);
	void Alpha(unsigned char alpha);
	void AlphaMode(ALPHAMODE AlphaMode);
	void TransColor(unsigned int color);

	int Width() const;
	int Height() const;

private:
	struct Impl;
	std::shared_ptr<Impl> m_pImpl;
};

//�`�悷��摜�Ƃ��̉摜�̌�ɂ���摜�Ƃ̃u�����h�̎d��
enum BLENDMODE
{
	BLENDMODE_NONE,		//�u�����h���Ȃ�
	BLENDMODE_ALPHA,	//�摜���̂̃A���t�@�l�Ńu�����h
	BLENDMODE_ALPHA2,	//�A���t�@�l��C�ӂŐݒ肵�ău�����h
	BLENDMODE_ADD,		//���Z����
	BLENDMODE_SUB,		//���Z����
	BLENDMODE_MUL,		//��Z����
};

//�摜���̂̓ǂݍ��ݕ��i�A���t�@�j
enum ALPHAMODE
{
	ALPHAMODE_NONE,			//�A���t�@�`�����l���͂Ȃ�
	ALPHAMODE_DEFAULT,		//�摜���̂̃A���t�@�l���g��(ARGBorRGBA�ɑΉ����Ă���g���q�̂�(.png�Ƃ�)�j
	ALPHAMODE_TRANS,		//���ߐF���g�p����
	ALPHAMODE_ALPHAGRAPH,	//���̉摜�Ƃ͕ʂɃA���t�@�l�p�̔����摜���g���i�������j
};

}//namespace NekLib

#endif