#include "Objet3DPhysique.h"
#include "CatmullRom.h"
#include <string>

class GhostShip : public Objet3D
{
public :
	GhostShip(PM3D::CDispositifD3D11* _pDispositif);

	void loadCurve(const std::string& _fileName);
	void anime(float) override;
private:
	std::vector<PointInfo> m_curve;
	PM3D::CDispositifD3D11* m_pDispositif;
};