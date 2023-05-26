#pragma once
#include "stdafx.h"
#include "Material.h"
#include <vector>
#include <string>


namespace PM3D {
	class MaterialManager
	{
		using value_type = std::shared_ptr<Material>;
		std::vector<value_type> m_materialList;
		MaterialManager() = default;
	public:
		static MaterialManager& getMaterialManager() {
			static MaterialManager materilaManager;
			return materilaManager;
		}

		MaterialManager(MaterialManager&) = delete;
		~MaterialManager() = default;
		MaterialManager& operator=(MaterialManager&) = delete;

		value_type getMaterial(const std::string& _materialName) const {
			auto it = find_if(m_materialList.begin(), m_materialList.end(), [&](const value_type _material) {
				return (_material->getNomMateriau() == _materialName);
			});

			if (it != m_materialList.end())
				return *it;
			return nullptr;
		}

		void addMaterial(value_type _material) {
			m_materialList.emplace_back(_material);
		}
	};
}

