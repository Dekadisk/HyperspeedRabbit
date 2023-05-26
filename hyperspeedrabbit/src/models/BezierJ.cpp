#include "stdafx.h"
#include "BezierJ.h"
#include <pxshared\foundation\PxVec3.h>
#include <algorithm>


BezierJ::vectorsXYZ BezierJ::subdivise(std::vector<float> X, std::vector<float> Y, std::vector<float> Z) {

    // nombreIteration fois
    for (int i = 0; i < m_nombreIterations; i++)
    {
        std::vector<float> newX;
        std::vector<float> newY;
        std::vector<float> newZ;

        // Dupliquer (1)
        for (int j = 0; j < X.size(); j++)
        {
            if (j == 0 || j == X.size() - 1) // Dupliquer degré fois
            {
                for (int k = 0; k < m_degres + 1; k++)
                {
                    newX.push_back(X[j]);
                    newY.push_back(Y[j]);
                    newZ.push_back(Z[j]);
                }
            }
            else // Dupliquer normalement (2 fois)
            {
                newX.push_back(X[j]);
                newX.push_back(X[j]);

                newY.push_back(Y[j]);
                newY.push_back(Y[j]);

                newZ.push_back(Z[j]);
                newZ.push_back(Z[j]);
            }
        }

        X = newX;
        Y = newY;
        Z = newZ;

        // Subdiviser degree fois (2)
        auto vectors = milieuDegreFois(X, Y, Z);
        X = std::get<0>(vectors);
        Y = std::get<1>(vectors);
        Z = std::get<2>(vectors);
    }

    return { X, Y, Z };
}

BezierJ::vectorsXYZ BezierJ::milieuDegreFois(std::vector<float> X, std::vector<float> Y, std::vector<float> Z)
{

    std::vector<float> X_ancien(X);
    std::vector<float> Y_ancien(Y);
    std::vector<float> Z_ancien(Z);

    std::vector<float> X_nouveau;
    std::vector<float> Y_nouveau;
    std::vector<float> Z_nouveau;

    // degree fois les milieux
    for (int i = 0; i < m_degres; i++)
    {
        X_nouveau.clear();
        Y_nouveau.clear();
        Z_nouveau.clear();

        for (int j = 0; j < X_ancien.size(); j++)
        {
            if (j != X_ancien.size() - 1) {
                X_nouveau.push_back(0.5f * X_ancien[j] + 0.5f * X_ancien[j + 1]);
                Z_nouveau.push_back(0.5f * Z_ancien[j] + 0.5f * Z_ancien[j + 1]);
                Y_nouveau.push_back(0.5f * Y_ancien[j] + 0.5f * Y_ancien[j + 1]);
            }
        }

        X_ancien = std::vector<float>(X_nouveau);
        Y_ancien = std::vector<float>(Y_nouveau);
        Z_ancien = std::vector<float>(Z_nouveau);

    }

    return { X_nouveau, Y_nouveau, Z_nouveau };
}

BezierJ::vectorsXYZ pxVec3toTuple(std::vector<physx::PxVec3> points) {

    std::vector<float> X;
    std::vector<float> Y;
    std::vector<float> Z;

    std::for_each(points.begin(), points.end(), [&](const physx::PxVec3& point) {
        X.push_back(point.x);
        Y.push_back(point.y);
        Z.push_back(point.z);
        });

    return { X,Y,Z };
}

void BezierJ::computeCurve(std::vector<physx::PxVec3> pt)
{

    auto tupple = pxVec3toTuple(pt);
    std::vector<float> Xres = std::get<0>(tupple);
    std::vector<float> Yres = std::get<1>(tupple);
    std::vector<float> Zres = std::get<2>(tupple);
    auto vectors = subdivise(Xres, Yres, Zres);
    Xres = std::get<0>(vectors);
    Yres = std::get<1>(vectors);
    Zres = std::get<2>(vectors);

    for (int i = 0; i < Xres.size(); ++i) {
        m_listePoints.push_back(physx::PxVec3(Xres[i], Yres[i], Zres[i]));
    }
}

std::vector<physx::PxVec3> BezierJ::getListPoints()
{
    return m_listePoints;
}
