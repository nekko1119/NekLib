#ifndef NEKLIB_SCENEMANAGER_H
#define NEKLIB_SCENEMANAGER_H

#include <memory>//shared_ptrの為

namespace NekLib
{

class IScene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//新しいシーンを追加削除する
	void Push(IScene* pNext);
	void Pop();
	void Clear();

	//シーンの更新描画
	void Update();
	void Draw();

private:
	struct Impl;
	std::shared_ptr<Impl> m_pImpl;
};

}//namespace NekLib

#endif