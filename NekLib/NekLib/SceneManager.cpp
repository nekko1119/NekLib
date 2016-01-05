#include <NekLib/SceneManager.h>
#include <NekLib/IScene.h>
#include <NekLib/Common.h>
#include <stack>
using namespace NekLib;

struct SceneManager::Impl
{
	std::stack<IScene*> sceneStack;	//�V�[���̃X�^�b�N
};

SceneManager::SceneManager() : m_pImpl(new Impl)
{}

SceneManager::~SceneManager()
{
	Clear();
}

void SceneManager::Push(IScene* pNext)
{
	m_pImpl->sceneStack.push(pNext);
}

void SceneManager::Pop()
{
	Delete(m_pImpl->sceneStack.top());
	m_pImpl->sceneStack.pop();
}

void SceneManager::Clear()
{
	while(!m_pImpl->sceneStack.empty())
	{
		Delete(m_pImpl->sceneStack.top());
		m_pImpl->sceneStack.pop();
	}
}

void SceneManager::Update()
{
	if(!m_pImpl->sceneStack.empty())
	{
		m_pImpl->sceneStack.top()->Update(this);
	}
}

void SceneManager::Draw()
{
	if(!m_pImpl->sceneStack.empty())
	{
		m_pImpl->sceneStack.top()->Draw(this);
	}
}