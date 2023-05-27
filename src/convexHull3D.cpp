#include "convexHull3D.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

namespace GeometryEx2 {

string printFacet(facet &f) {
    string s = "(";
    s += std::to_string(f.a) + ", " + std::to_string(f.b);
    s += ", " + std::to_string(f.c);
    s += ")";
    return s;
}

string printOrientation(orientation r) {
    if (r == ab) return "ab";
    if (r == bc) return "bc";
    return "ca";
}

// check from with side does current connects to next from the next point of view.
orientation adjecancyDirection(facet &current, facet &next) {
    // check from with side next sees current;
    if (next.ab == &current) return ab;
    if (next.bc == &current) return bc;
    if (next.ca == &current) return ca;
    cerr << endl;
    cerr << "current enabled ? " << current.enabled << " :: next.enabled ? " << next.enabled << endl;
    cerr << " Error : current = " << printFacet(current) << endl;
    cerr << " Error : next = " << printFacet(next) << endl;
    cerr << " Error : next.ab = " << printFacet(*next.ab) << endl;
    cerr << " Error : next.bc = " << printFacet(*next.bc) << endl;
    cerr << " Error : next.ca = " << printFacet(*next.ca) << endl;
    throw std::invalid_argument("facets are not neibours!");
}

orientation nextOrientation(orientation current) {
    if (current == ab) return bc;
    if (current == bc) return ca;
    return ab;
}

facet *nextFacet(facet &f, orientation direction) {
    if (direction == ab) return f.ab;
    if (direction == bc) return f.bc;
    return f.ca;
}

facetStep ConvexHull3D::nextHorizonFacet(facet &f, orientation edge, int npoint) {
    // cout << " nextHorizonFacet";
    //  cout << "start : " << printFacet(f) << endl;
    //  Given an facet, that is not in conflict with
    facetStep step;
    step.current = nextFacet(f, edge);
    step.prev = adjecancyDirection(f, *step.current);
    step.next = nextOrientation(step.prev);

    // cout << "next facet: " << printFacet(*step.current) << endl;
    // cout << "prev : " << printOrientation(step.prev) << " | next : " << printOrientation(step.next) << endl;

    while (isConflicted(*step.current, npoint)) {
        // cout << "is conflicted" << endl;
        facet &c = *step.current;
        step.current = nextFacet(c, step.next);
        step.prev = adjecancyDirection(c, *step.current);
        step.next = nextOrientation(step.prev);
        // cout << "next facet: " << printFacet(*step.current) << endl;
        // cout << "prev : " << printOrientation(step.prev) << " | next : " << printOrientation(step.next) << endl;
    }
    // cout << "returns!\n"
    //      << endl;

    return step;
}

double randZeroToOne() {
    return rand() / (RAND_MAX + 1.);
}

float det3(float *v1, float *v2, float *v3) {
    // calculate the detrminant of a 3x3 matrix where each column is given by v1,v2,v3.
    return v1[0] * (v2[1] * v3[2] - v3[1] * v2[2]) - v2[0] * (v1[1] * v3[2] - v3[1] * v1[2]) + v3[0] * (v1[1] * v2[2] - v2[1] * v1[2]);

    // return -v3[0] * v2[1] * v1[2] + v2[0] * v3[1] * v1[2] + v3[0] * v1[1] * v2[2] - v1[0] * v3[1] * v2[2] - v2[0] * v1[1] * v3[2] + v1[0] * v2[1] * v3[2];
}

int orient(float *a, float *b, float *c, float *v) {
    // calculate the orientation of 4 vectors, that is check if v sees the triangle a,b,c clockwise or anti-clockwise.
    float det = det3(b, c, v) - det3(a, c, v) + det3(a, b, v) - det3(a, b, c);
    return det > 0 ? 1 : det == 0 ? 0
                                  : -1;
}

ConvexHull3D::ConvexHull3D(std::string filename) {
    string line;
    ifstream myfile(filename);

    getline(myfile, line);
    std::istringstream ss(line);

    ss >> this->numVectors;

    vertices = new float[3 * this->numVectors];
    colors = new float[3 * this->numVectors];

    for (size_t i = 0; i < this->numVectors * 3; i++) {
        colors[i] = randZeroToOne();
    }

    // Random-colors
    int j = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            std::istringstream ss(line);
            for (size_t i = 0; i < 3; i++) {
                int next;
                ss >> next;
                vertices[j * 3 + i] = next;
            }
            j++;
        }
        myfile.close();
    }
}

void ConvexHull3D::getNextPoint(float *out) {
    int n = nextVert;
    if (nextVert == numVectors) {
        nextVert--;
    }
    auto nextIndex = order[nextVert]->pointID;
    for (size_t i = 0; i < 3; i++) {
        out[i] = vertices[nextIndex * 3 + i];
    }
}

void ConvexHull3D::getPoint(float *out, int point) {
    auto nextIndex = order[point]->pointID;
    for (size_t i = 0; i < 3; i++) {
        out[i] = vertices[nextIndex * 3 + i];
    }
}

void ConvexHull3D::start(int seed) {
    if (seed == -1) {
        std::srand(unsigned(std::time(0)));
    } else {
        std::srand(unsigned(seed));
    }
    order.clear();
    for (int i = 0; i < this->numVectors; i++) {
        order.push_back(new point{i});
    }

    // for (size_t i = 0; i < this->numVectors; i++) {
    //     cout << order[i].pointID << endl;
    // }

    std::random_shuffle(order.begin(), order.end());

    this->nextVert = 4;
    numberOfTriangles = 4;
    triangles = new unsigned int[numberOfTriangles * 3];

    int arr[4][4] = {
        {0, 1, 2, 3},
        {0, 1, 3, 2},
        {0, 2, 3, 1},
        {1, 2, 3, 0}};

    // Sort the array in ascending order

    facets.clear();
    for (size_t i = 0; i < 4; i++) {
        facets.push_back(new facet{});

        float *a = vertices + order[arr[i][0]]->pointID * 3;
        float *b = vertices + order[arr[i][1]]->pointID * 3;
        float *c = vertices + order[arr[i][2]]->pointID * 3;
        float *d = vertices + order[arr[i][3]]->pointID * 3;

        if (orient(a, b, c, d) < 0) {
            facets.back()->a = arr[i][0];
            facets.back()->b = arr[i][1];
            facets.back()->c = arr[i][2];

            // cout << "facet??  " << printFacet(*facets.back()) << endl;
        } else {
            facets.back()->a = arr[i][0];
            facets.back()->b = arr[i][2];
            facets.back()->c = arr[i][1];
            // cout << "facet??  " << printFacet(*facets.back()) << endl;
        }

        triangles[i * 3 + 0] = order[facets.back()->a]->pointID;
        triangles[i * 3 + 1] = order[facets.back()->b]->pointID;
        triangles[i * 3 + 2] = order[facets.back()->c]->pointID;
    }

    for (auto it1 = facets.begin(); it1 != facets.end(); ++it1) {
        facet &f1 = *(*it1);
        for (auto it2 = facets.begin(); it2 != facets.end(); ++it2) {
            facet &f2 = *(*it2);
            if (&f1 == &f2) continue;
            if (f1.a == f2.a || f1.a == f2.b || f1.a == f2.c) {
                if (f1.b == f2.a || f1.b == f2.b || f1.b == f2.c) {
                    f1.ab = &f2;
                } else {
                    f1.ca = &f2;
                }
            } else {
                f1.bc = &f2;
            }
            /* code */
        }

        // cout << f1.ab << " : " << f1.bc << " : " << f1.ca << endl;

        for (size_t j = nextVert; j < this->numVectors; j++) {
            // auto p = order[i];
            addConflict(f1, j);
        }
    }
}

bool ConvexHull3D::isConflicted(facet &f, int pointID) {
    auto &p = order[pointID];
    float *a = vertices + order[f.a]->pointID * 3;
    float *b = vertices + order[f.b]->pointID * 3;
    float *c = vertices + order[f.c]->pointID * 3;
    float *v = vertices + p->pointID * 3;

    return orient(a, b, c, v) == 1;
}

void ConvexHull3D::addConflict(facet &f, int pointID) {
    auto &p = order[pointID];
    float *a = vertices + order[f.a]->pointID * 3;
    float *b = vertices + order[f.b]->pointID * 3;
    float *c = vertices + order[f.c]->pointID * 3;
    float *v = vertices + p->pointID * 3;

    if (orient(a, b, c, v) == 1) {
        p->conflictedFacets.insert(&f);
        f.confilectedPoints.push_back(pointID);
    }
}

void ConvexHull3D::removeConflict(facet &f, int pointID) {
    order[pointID]->conflictedFacets.erase(&f);
}

int ConvexHull3D::next() {
    if (nextVert == this->numVectors) {
        // cout << "No more points" << endl;
        return 0;
    }

    auto &p = *order[nextVert];

    int triangles_added = 0;

    // cout << "check for conflicting pairs!" << endl;
    //  Check for conflicted pairs, and if there is "remove them".
    bool conflicted = false;
    for (auto itr = p.conflictedFacets.begin(); itr != p.conflictedFacets.end(); itr++) {
        facet &current = *(*itr);
        conflicted = conflicted | current.enabled;
        if (current.enabled) {
            // current.enabled = false;
            triangles_added--;
        }
    }
    if (!conflicted) {
        // cout << "no conflicting pairs!" << endl;
        nextVert++;
        return -1;
    }

    facet *firstBorderFacet;

    orientation side;
    facetStep currentBorder;
    // cout << "find border horizon" << endl;
    // Find first horizon Facet.
    for (auto itr = p.conflictedFacets.begin();
         // Find horizon edges.
         itr != p.conflictedFacets.end(); itr++) {
        facet &current = *(*itr);
        // cout << "is enabled ? : " << current.enabled << endl;
        if (current.enabled == false) {
            continue;
        }

        if (!isConflicted(*current.ab, nextVert)) {
            // cout << "border !! " << printFacet(current) << endl;
            currentBorder.current = current.ab;
            currentBorder.prev = adjecancyDirection(current, *currentBorder.current);
            break;
        }

        if (!isConflicted(*current.bc, nextVert)) {
            // cout << "border !! " << printFacet(current) << endl;
            currentBorder.current = current.bc;
            currentBorder.prev = adjecancyDirection(current, *currentBorder.current);
            break;
        }

        if (!isConflicted(*current.ca, nextVert)) {
            // cout << "border !! " << printFacet(current) << endl;
            currentBorder.current = current.ca;
            currentBorder.prev = adjecancyDirection(current, *currentBorder.current);
            break;
        }
    }

    // cout << printFacet(*currentBorder.current) << endl;

    vector<facetStep> edges;
    facetStep start = currentBorder;
    // cout << "find next edge?" << endl;
    do {
        // Find all edges.
        edges.push_back(currentBorder);
        // cout << "current edge : " << printFacet(*currentBorder.current) << " :: " << printOrientation(currentBorder.prev) << endl;
        currentBorder = nextHorizonFacet(*currentBorder.current, currentBorder.prev, nextVert);
        // cout << "start ? " << printFacet(*start.current) << " :: " << printOrientation(start.prev) << endl;
        // cout << "next edge : " << printFacet(*currentBorder.current) << " :: " << printOrientation(currentBorder.prev) << endl;

        // cout << printFacet(*currentBorder.current) << " :: " << printOrientation(currentBorder.prev) << endl;
        // cout << printFacet(*currentBorder.current) << endl;

        // cout << (currentBorder.current != start.current) << " ? " << (start.prev != currentBorder.prev) << endl;
    } while (currentBorder.current != start.current || start.prev != currentBorder.prev);

    // cout << "\ncreating new facets" << endl;
    // Create new facets and connect them
    facet *prev = NULL;
    facet *first = NULL;
    for (size_t i = 0; i < edges.size(); i++) {
        facets.push_back(new facet{});
        facet &cn = *facets.back();
        triangles_added++;
        cn.c = nextVert;
        cn.ab = edges[i].current;

        // cout << "current edge : " << printFacet(*edges[i].current) << endl;
        // cout << "Old neibours : ab = " << printFacet(*edges[i].current->ab)
        //      << "| bc = " << printFacet(*edges[i].current->bc)
        //      << "| ca = " << printFacet(*edges[i].current->ca) << endl;

        /* code */
        if (edges[i].prev == ab) {
            cn.a = edges[i].current->b;
            cn.b = edges[i].current->a;

            edges[i].current->ab = &cn;
        } else if (edges[i].prev == bc) {
            cn.a = edges[i].current->c;
            cn.b = edges[i].current->b;

            edges[i].current->bc = &cn;
        } else {
            cn.a = edges[i].current->a;
            cn.b = edges[i].current->c;

            edges[i].current->ca = &cn;
        }

        // cout << "new facet is : " << printFacet(cn) << endl;
        // cout << "new neibours : ab = " << printFacet(*edges[i].current->ab)
        //      << "| bc = " << printFacet(*edges[i].current->bc)
        //      << "| ca = " << printFacet(*edges[i].current->ca) << endl;
        // cout << "\n"
        //      << endl;

        if (first == NULL) {
            first = &cn;
        } else {
            prev->bc = &cn;
            cn.ca = prev;
        }
        prev = &cn;

        // Add facet conflicts.
        for (int j = nextVert + 1; j < this->numVectors; j++) {
            addConflict(cn, j);
        }
    }
    // cout << "updating last two edges" << endl;
    prev->bc = first;
    first->ca = prev;

    // for (size_t i = 0; i < edgeVertices.size(); i++) {
    //     cout << edgeVertices[i] << " ";
    // }
    // cout << endl;

    // Create edges
    // facets.push_back(facet{});
    // facet &first = facets[facets.size()];

    nextVert++;
    numberOfTriangles += triangles_added;
    // cout << " done -- " << endl;

    for (auto itr = p.conflictedFacets.begin(); itr != p.conflictedFacets.end(); itr++) {
        facet &current = *(*itr);
        current.enabled = false;
    }
    return 1;
}

void ConvexHull3D::createMeshData() {
    this->numberOfTriangles = 0;
    vector<unsigned int> triangles;
    // triangles = new unsigned int[numberOfTriangles * 3];
    // cout << "creating mesh !!" << endl;
    for (auto it = facets.begin(); it != facets.end(); ++it) {
        facet &current = *(*it);
        if (current.enabled) {
            // cout << "\n ------------- " << endl;
            // cout << printFacet(current) << endl;
            // cout << " : ab - " << printFacet(*current.ab) << " :: " << printOrientation(adjecancyDirection(current, *current.ab)) << endl;
            // cout << " : bc - " << printFacet(*current.bc) << " :: " << printOrientation(adjecancyDirection(current, *current.bc)) << endl;
            // cout << " : ca - " << printFacet(*current.ca) << " :: " << printOrientation(adjecancyDirection(current, *current.ca)) << endl;

            triangles.push_back(order[current.a]->pointID);
            triangles.push_back(order[current.b]->pointID);
            triangles.push_back(order[current.c]->pointID);
            this->numberOfTriangles++;
            // triangles[i * 3 + 0] = order[current.a]->pointID;
            // triangles[i * 3 + 1] = order[current.b]->pointID;
            // triangles[i * 3 + 2] = order[current.c]->pointID;
        }
    }
    // cout << "number of triangles ?" << this->numberOfTriangles << endl;
    this->triangles = new unsigned int[triangles.size()];
    for (int i = 0; i < triangles.size(); i++) {
        this->triangles[i] = triangles[i];
    }
}

ConvexHull3D::~ConvexHull3D() {
    for (auto p : facets) {
        delete p;
    }
    facets.clear();

    for (auto p : this->order) {
        delete p;
    }
    order.clear();
}

} // namespace GeometryEx2