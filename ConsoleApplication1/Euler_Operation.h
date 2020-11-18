#pragma once
#define data_structure

#include <cstdlib>
#include <cstring>
#include <iostream>
#include<vector>
using namespace std;

class solid {

public:
	face* sface;
	solid* pre;
	solid* next;
	vertex* svertex;
	edge* sedge;
public:
	solid ();
	vertex* getVertex(double coord[3]);
	edge* getEdge(vertex* v1,vertex* v2);
};
class face {
public:
	face* pre;
	face* next;
	solid* fsolid;
	loop* floop;
public:
	face();
};
class loop {
public:
	face* lface;
	loop* pre;
	loop* next;
	halfedge* lhalfedge;
	vertex* lvertex;
public:
	loop();
	vertex* getVertex(double vcoord[3]);
};
class halfedge {
public:
	halfedge* pre;
	halfedge* next;
	edge* hedge;
	vertex* startv;
	vertex* endv;
	loop* hloop;
	halfedge* adjacent;
public:
	halfedge();

};
class vertex {
public:
	int id;
	double coord[3];
	vertex* pre;
	vertex* next;
	halfedge* vhalfedge;
	vertex(double vcoord[3]) {
		coord[0] = vcoord[0];
		coord[1] = vcoord[1];
		coord[2] = vcoord[2];
	}
	bool equal_Coor(double coord[3]);//判断输入的点与该点是否相同
};
class edge {
public:
	edge* pre;
	edge* next;
	solid* esolid;
	halfedge* halfedge1;
	halfedge* halfedge2;
	edge();
};
class Euler_Operation {
public:
	solid* solid1;
	solid* mvsf(double vcoord[3]);
	edge* mev(double vcoord1[3], double vcoord2[3],loop* alloop);
	loop* mef(double vcoord1[3], double vcoord2[3],loop* alloop);
	loop* kemr(double vcoord1[3],double vcoord2[3],loop* loop);
	loop* kfmrh(loop* innerloop,loop* outloop);
	void kev(double coord[3],double vcoord[3],loop* alloop);
	void kvfs(double coord[3],solid* s);
	void kef(double coord[3],double vcoord[3],loop* alloop);
	void mekr(double coord[3], double vcoord[3], loop* innerloop, loop* outerloop);
	void mfkrh(loop* innerloop,loop* outerloop);
	solid* sweep(face* f,double vector[3]);
};