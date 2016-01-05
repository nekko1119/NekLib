#ifndef NEKLIB_SCENEMANAGER_H
#define NEKLIB_SCENEMANAGER_H

#include <memory>//shared_ptr�̈�

namespace NekLib
{

class IScene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//�V�����V�[����ǉ��폜����
	void Push(IScene* pNext);
	void Pop();
	void Clear();

	//�V�[���̍X�V�`��
	void Update();
	void Draw();

private:
	struct Impl;
	std::shared_ptr<Impl> m_pImpl;
};

}//namespace NekLib

#endif