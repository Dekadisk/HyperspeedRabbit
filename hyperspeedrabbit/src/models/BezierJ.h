#pragma once
#include "stdafx.h"
#include <pxshared\foundation\PxVec3.h>
#include <vector>

class BezierJ
{
public:
    using vectorsXYZ = std::tuple<std::vector<float>, std::vector<float>, std::vector<float>>;

    BezierJ(int _degres, int _nombreIterations) : m_degres(_degres), m_nombreIterations(_nombreIterations) {}
    // std::vectore des points composant la courbe
    std::vector<physx::PxVec3> m_listePoints;

    std::vector<float> m_pointsControleX;
    std::vector<float> m_pointsControleY;
    std::vector<float> m_pointsControleZ;

    // degres des polynomes par morceaux
    int m_degres;

    // nombre d'itération de subdivision
    int m_nombreIterations;

	vectorsXYZ subdivise(std::vector<float> X, std::vector<float> Y, std::vector<float> Z);
	vectorsXYZ milieuDegreFois(std::vector<float> X, std::vector<float> Y, std::vector<float> Z);
	void computeCurve(std::vector<physx::PxVec3>);
    std::vector<physx::PxVec3> getListPoints();
};

