#include "GameManager.h"

namespace PM3D {
	bool GameManager::preInitScenes(CDispositifD3D11* _pDispositif, XMMATRIX* _proj, XMMATRIX* _view, XMMATRIX* _viewProj)
	{
		m_sceneList.push_back(std::make_unique<Scene>(_pDispositif, _proj, _view, _viewProj));
		return true;
	}

	bool GameManager::initScenes()
	{
		for (auto& scene : m_sceneList)
			scene->init();
		return true;
	}

	bool GameManager::renderScene() {
		return m_sceneList.at(m_currentSceneIndex)->render();
	}

	bool GameManager::animeScene(float _tempEcoule) {
		return m_sceneList.at(m_currentSceneIndex)->anime(_tempEcoule);
	}

	void GameManager::cleanUp() noexcept {
		m_sceneList.clear();
	}
}
