#pragma once
// #include <bits/stdc++.h>
#include <list>
#include <set>
#include <string>
#include <vector>
// #include <iostream>

namespace GeometryEx2 {

enum orientation {
    ab,
    bc,
    ca
};

int orient(float *a, float *b, float *c, float *v);

struct facet {
    int a, b, c;
    facet *ab;
    facet *bc;
    facet *ca;
    bool enabled = true;
    std::vector<int> confilectedPoints;
    /* data */

    friend bool operator==(const facet &a, const facet &b) {
        return &a == &b;
    }
};

struct facetStep {
    facet *current;
    orientation prev;
    orientation next;
};

struct point {
    int pointID;
    std::set<facet *> conflictedFacets;

    point(int id) {
        pointID = id;
        conflictedFacets.clear();
    }
};

class ConvexHull3D {
private:
    std::vector<point *> order;
    std::list<facet *> facets;
    int nextVert;

    void addConflict(facet &f, int pointID);
    void removeConflict(facet &f, int pointID);
    facetStep nextHorizonFacet(facet &f, orientation edge, int npoint);
    bool isConflicted(facet &f, int pointID);

public:
    float *vertices;
    unsigned int *triangles;
    float *colors;
    unsigned int numberOfTriangles;
    unsigned int numVectors;

public:
    ConvexHull3D(std::string fileName);
    ~ConvexHull3D();
    void getNextPoint(float *out);
    void getPoint(float *out, int point);
    void start(int seed);
    int next();
    void createMeshData();
};
} // namespace GeometryEx2