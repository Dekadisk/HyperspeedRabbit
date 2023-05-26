#pragma once 

namespace PM3D {

	class CInfoDispositif {
	public:

		explicit CInfoDispositif(int NoAdaptateur);
		CInfoDispositif(DXGI_MODE_DESC modeDesc);

		enum INFODISPO_TYPE { 
			ADAPTATEUR_COURANT 
		};

		bool isValid() const;
		int getWidth() const;
		int getHeight() const;
		int getMemory() const;
		const wchar_t* getMapName() const;
		void GetDesc(DXGI_MODE_DESC& modeDesc);
		
	private:
		bool valide;
		int largeur, hauteur, memoire;
		wchar_t m_mapName[100];
		DXGI_MODE_DESC mode;
	};

}