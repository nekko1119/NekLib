#include <Common.h>
#include <LostResourceContainer.h>
#include <ILostResource.h>
#include <Frame.h>

#include <list>

#include <d3d9.h>
using namespace NekLib;

#pragma comment(lib, "d3d9.lib")

struct LostResourceContainer::Impl
{
	std::list<std::shared_ptr<ILostResource> > lostResourceList;	//���X�g
	~Impl();
};

LostResourceContainer::Impl::~Impl()
{
	lostResourceList.clear();
}

LostResourceContainer::LostResourceContainer() : m_pImpl(new Impl)
{
}

LostResourceContainer* LostResourceContainer::GetInstance()
{
	static LostResourceContainer* instance = new LostResourceContainer();
	return instance;
}

void LostResourceContainer::Add(std::shared_ptr<ILostResource> pLostResource)
{
	m_pImpl->lostResourceList.push_back(pLostResource);
}

void LostResourceContainer::Recover(D3DPRESENT_PARAMETERS& d3dpp)
{
	std::list<std::shared_ptr<ILostResource> >::iterator it;
	for(it = m_pImpl->lostResourceList.begin(); it != m_pImpl->lostResourceList.end(); it++)
	{
		(*it)->Backup();
	}

	//���A�s�\�Ȃ牽�������߂�
	if(Frame::GetInstance()->GetDevice()->TestCooperativeLevel() == D3DERR_DEVICELOST)
	{
		return;
	}

	//���A�Ɏ��s������I��
	if(FAILED(Frame::GetInstance()->GetDevice()->Reset(&d3dpp)))
	{
		EXIT("�f�o�C�X�̃��Z�b�g���ł��܂���ł���");
	}

	for(it = m_pImpl->lostResourceList.begin(); it != m_pImpl->lostResourceList.end(); it++)
	{
		(*it)->Recover();
	}
}

void LostResourceContainer::Update()
{
	std::list<std::shared_ptr<ILostResource> >::iterator it;
	for(it = m_pImpl->lostResourceList.begin(); it != m_pImpl->lostResourceList.end();)
	{
		if((*it)->GetCount() == 1)
		{
			it = m_pImpl->lostResourceList.erase(it);
			continue;
		}
		it++;
	}
}