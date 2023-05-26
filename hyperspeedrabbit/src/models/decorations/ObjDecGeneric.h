#include "ObjDecoration.h"
class ObjDecGeneric : public ObjDecoration
{
public:
	ObjDecGeneric(PM3D::CDispositifD3D11* _pDispositif, const std::string& _chemin,const std::string& _fileName,
		const DirectX::XMFLOAT3 _position = {},
		const DirectX::XMFLOAT4 _rotation = { 0.f,0.f,0.f,1.f },
		const DirectX::XMFLOAT3 _scale = {1.f,1.f,1.f});

	virtual ~ObjDecGeneric() = default;

	// Hérité via ObjDecoration
	virtual void anime(float) override;
};
