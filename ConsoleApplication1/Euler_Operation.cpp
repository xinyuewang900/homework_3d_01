#define _CRT_SECURE_NO_WARNINGS
#include "Euler_Operation.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>
using namespace std;

vertex* solid::getVertex(double coord[3]) {
	vertex* v = svertex;
	while (v != NULL) {
		if (v->equal_Coor(coord)) {
			return v;
		}
		v = v->next;
	}
	return NULL;
}
edge* solid::getEdge(vertex* v1,vertex* v2) {
	edge* e = sedge;
	while (e != NULL) {
		if ((e->halfedge1->startv == v1 && e->halfedge1->endv == v2) || (e->halfedge1->endv == v1 && e->halfedge1->startv == v2)) {
			return e;
		}
		e = e->next;
	}
	return NULL;
}
solid::solid() {
	sface = NULL;
	pre = NULL;
	next = NULL;
	svertex = NULL;
	sedge = NULL;
}
face::face() {
	fsolid = NULL;
	pre = NULL;
	next = NULL;
	floop = NULL;
}

vertex* loop::getVertex(double vcoord[3]) {
	halfedge* h = lhalfedge;
	while (h != NULL) {
		if (h->endv->equal_Coor(vcoord)) {
			return h->endv;
		}
		if (h->startv->equal_Coor(vcoord)) {
			return h->startv;
		}
		h = h->next;
	}
	return NULL;
}
loop::loop() {
	lface = NULL;
	lhalfedge = NULL;
	pre = NULL;
	next = NULL;

}
halfedge::halfedge() {
	pre = NULL;
	next = NULL;
	startv = NULL;
	endv = NULL;
	hloop = NULL;
	hedge = NULL;
}
bool vertex::equal_Coor(double vcoord[3]) {
	if (coord[0] == vcoord[0] && coord[1] == vcoord[1] && coord[2] == vcoord[2]) {
		return true;
	}
	return false;
}
edge::edge() {
	pre = NULL;
	next = NULL;
	esolid = NULL;
	halfedge1 = NULL;
	halfedge2 = NULL;

}
edge* Euler_Operation::mev(double coord[3], double vcoord[3], loop* alloop) {
	//用于从最初开始生成边
	solid* solid = alloop->lface->fsolid;
	vertex* newvertex = new vertex(vcoord);
	vertex* alvertex = alloop->getVertex(coord);
	if (alvertex == NULL) {
		return NULL;//不存在已知点
	}
	halfedge* he1, *he2;
	he1 = new halfedge();
	he2 = new halfedge();
	he1->startv = newvertex;
	he1->endv = alvertex;
	he2->startv = alvertex;
	he2->endv = newvertex;
	edge* newedge = new edge();
	he1->hedge = newedge;
	he2->hedge = newedge;
	he1->adjacent = he2;
	he2->adjacent = he1;
	newedge->halfedge1 = he1;
	newedge->halfedge2 = he2;
	he1->hloop = alloop;
	he2->hloop = alloop;
	he1->next = he2;
	he2->pre = he1;
	if (alloop->lhalfedge == NULL) {
		//说明是从第一个点开始生成边
		he1->pre = he2;
		he2->next = he1;
		alloop->lhalfedge = he1;
	}
	else {
		halfedge* e;
		for (e = alloop->lhalfedge; !(e->next->startv->equal_Coor(coord)); e = e->next);
		he2->next = e->next;
		he1->pre = e;
		e->next = he1;
		he2->next->pre = he2;

	}
	//把实体边加入solid
	edge* ed = solid->sedge;
	if (ed == NULL) {
		solid->sedge = ed;
	}
	else {
		for (; ed ->next!=NULL; ed = ed->next);
		ed->next = newedge;
		newedge->pre = ed;

	}
	newedge->esolid = solid;

	//把新生成的点加入到solid中
	vertex* vv = solid->svertex;
	if (vv == NULL) {
		solid->svertex = newvertex;
	}
	else {
		while (vv->next != NULL&&!(vv->equal_Coor(vcoord))) {
			vv = vv->next;
		}
		if (!(vv->equal_Coor(vcoord))) {
           vv->next = newvertex;
		   newvertex->pre = vv;
		}
		
	}
	return newedge;
}

loop* Euler_Operation::mef(double coord[3],double vcoord[3],loop* alloop) {
	//构造长方形的第四条边,同时构造一个新环
	edge* newedge = new edge();
	halfedge* he1 = new halfedge();
	halfedge* he2 = new halfedge();
	vertex* v1 = alloop->getVertex(coord);
	vertex* v2 = alloop->getVertex(vcoord);
	if (v1 == NULL) {
		return NULL;
	}
	if (v2 == NULL) {
		return NULL;
	}
	loop* newloop = new loop();
	face* newface = new face();
	solid* solid = alloop->lface->fsolid;
	he2->startv = v1;
	he2->endv = v2;
	he1->startv = v2;
	he1->endv = v1;
	he1->adjacent = he2;
	he2->adjacent = he1;
	he1->next = he2;
	he2->pre = he1;
	he1->hedge = newedge;
	he2->hedge = newedge;
	newedge->halfedge1 = he1;
	newedge->halfedge2 = he2;
	halfedge* h1, *h2;
	halfedge* cur = alloop->lhalfedge;
	for (; !(cur->startv->equal_Coor(v1->coord)); cur = cur->next);
	h1 = cur;
	cur = alloop->lhalfedge;
	for (; !(cur->startv->equal_Coor(v2->coord)); cur = cur->next);
	h2 = cur;
	he1->next = h1;
	h2->pre->next = he1;
	he1->pre = h2->pre;
	he2->next = h2;
	h2->pre = he2;
	h1->pre->next = he2;
	he2->pre = h1->pre;
	h1->pre = he1;
	newloop->lhalfedge = he2;
	newloop->lface = newface;
	newface->floop = newloop;
	newface->fsolid = solid;
	he1->hloop = alloop;
	he2->hloop = newloop;
	//添加新面
	face* f = solid->sface;
	if (f == NULL) {
		solid->sface = newface;
	}
	else {
		while (f->next!= NULL) {
			f = f->next;
		}
		f->next = newface;
		newface->pre = f;
	}
	//添加新边
	edge* e = solid->sedge;
	if (e == NULL) {
		solid->sedge = newedge;
	}
	else {
		while (e->next != NULL) {
			e = e->next;
		}
		e->next = newedge;
		newedge->pre = e;
	}
	return newloop;
}
loop* Euler_Operation::kemr(double coord[3], double vcoord[3], loop* alloop) {
	//消去一个边生成一个新的内环
	solid* solid = alloop->lface->fsolid;
	face* f = alloop->lface;
	loop* innerloop = new loop();
	halfedge* he1, *he2;
	vertex* v1 = alloop->getVertex(coord);
	vertex* v2 = alloop->getVertex(vcoord);
	if (v1 == NULL || v2 == NULL) {
		return NULL;
	}
	edge* e = solid->getEdge(v1, v2);

	if (e == NULL) {
		printf("不存在这条边\n");
		return NULL;
	}
	if (e->halfedge1->startv->equal_Coor(coord) && e->halfedge1->endv->equal_Coor(vcoord)) {
		he1 = e->halfedge1;
		he2 = e->halfedge2;
	}
	else {
		he2 = e->halfedge1;
		he1 = e->halfedge2;
	}
	he1->next->pre = he2->pre;
	he2->pre->next = he1->next;
	he1->pre->next = he2->next;
	he2->next->pre = he1->pre;
	innerloop->lface = f;
	innerloop->lhalfedge = he1->next;
	alloop->lhalfedge = he1->pre->next;
	innerloop->lvertex = v2;
	//把新环加入进去
	if (f->floop == NULL) {
		f->floop = innerloop;
	}
	else {
		loop* l = f->floop;
		while (l->next != NULL) {
			l = l->next;
		}
		l->next = innerloop;
		innerloop->pre = l;
	}
	//删除一条物理边
	edge* ee = solid->sedge;
	while (ee != e) {
		ee = ee->next;
	}
	if (ee->next == NULL) {
		ee->pre->next == NULL;
		
	}
	else if (ee->pre == NULL) {
		ee->next->pre = NULL;
		solid->sedge = ee->next;
	}
	else {
		ee->next->pre = ee->pre;
		ee->pre->next = ee->next;
	}
	delete(ee);
	delete(he1);
	delete(he2);
	return innerloop;
}
loop* Euler_Operation::kfmrh(loop* innerloop,loop* outloop) {
	if (outloop == NULL || innerloop == NULL) {
		return NULL;
	}
	face* f1 = outloop->lface;
	face* f2 = innerloop->lface;
	solid* solid = f1->fsolid;
	if (f1->floop == NULL) {
		f1->floop = innerloop;
	}
	else {
		loop* l = f1->floop;
		while (l->next != NULL) {
			l = l->next;
		}
		l->next = innerloop;
		innerloop->pre = l;
	}
	innerloop->lface = f1;
	//删除面
	face* ff = solid->sface;
	while (ff != f2) {
		ff = ff->next;
	}
	if (ff == solid->sface) {
		solid->sface = ff->next;
		ff->next->pre = NULL;
	}
	else if (ff->next == NULL) {
		ff->pre->next = NULL;
	}
	else {
		ff->pre->next = ff->next;
		ff->next->pre = ff->pre;

	}
	delete(f2);
}
solid* Euler_Operation::mvsf(double coord[3]) {
	    //输入一个点，构造一个体，一个面，一个外环   
	vertex* v = new vertex(coord);
	solid* s = new solid();
	loop* l = new loop();
	face* f = new face();
	s->sface = f;
	f->fsolid = s;
	f->floop = l;
	s->svertex = v;
	l->lvertex = v;
	l->lface = f;
	l->lhalfedge = NULL;
	return s;

}
void Euler_Operation::kev(double coord[3], double vcoord[3], loop* alloop) {
	solid* solid = alloop->lface->fsolid;
	face* f = alloop->lface;
	vertex* v1 = alloop->getVertex(coord);
	vertex* v2 = alloop->getVertex(vcoord);//要被删除的点
	if (v1 == NULL) {
		printf("不存在v1该点\n");
		return;
	}
	if (v2 == NULL) {
		printf("不存在点v2\n");
		return;
	}
	edge* del_edge = solid->getEdge(v1,v2);//要被删除的边
	halfedge* he1, *he2;
	halfedge* cur = alloop->lhalfedge;
	//cur = cur->next;
	while (cur->next != alloop->lhalfedge) {
		if (cur->startv->equal_Coor(v1->coord) && cur->endv->equal_Coor(v2->coord)) {
			he1 = cur;
			he2 = he1->adjacent;
			break;
		}
		if (cur->startv->equal_Coor(v2->coord) && cur->endv->equal_Coor(v1->coord)) {
			he2 = cur;
			he1 = he2->adjacent;
			break;
		}
		cur = cur->next;
	}
	halfedge* del_he1 = he1;
	halfedge* del_he2 = he2;
	del_he1->hloop->lhalfedge = del_he1->next;
	del_he2->hloop->lhalfedge = del_he2->next;
	del_he1->pre->next = del_he1->next;
	del_he1->next->pre = del_he1->pre;
	del_he2->next->pre = del_he2->pre;
	del_he2->pre->next = del_he2->next;
	delete(del_he1);
	delete(del_he2);
	//删除物理边
	if (del_edge->next != NULL && del_edge->pre != NULL) {
		del_edge->next->pre = del_edge->pre;
		del_edge->pre->next = del_edge->next;
	}else if (del_edge->next==NULL&&del_edge->pre!=NULL) {
		del_edge->pre->next = NULL;
		solid->sedge = del_edge->pre;
	}
	else if (del_edge->next != NULL && del_edge->pre == NULL) {
		del_edge->next->pre = NULL;
		solid->sedge = del_edge->next;
	}
	else {
		solid->sedge = NULL;
	}
	delete(del_edge);
	//删除点
	if (v2->next != NULL && v2->pre != NULL) {
		v2->next->pre = v2->pre;
		v2->pre->next = v2->next;
	}
	else if (v2->next == NULL && v2->pre != NULL) {
		v2->pre->next = NULL;
	//	solid->svertex = v2->pre;

		//alloop->lvertex = 
	}
	else if (v2->next != NULL && v2->pre == NULL) {
		v2->next->pre = NULL;
		solid->svertex = v2->next;
	}
	else {
		solid->svertex = NULL;
	}
	delete(v2);

}
void Euler_Operation::kvfs(double coord[3],solid* s) {
	vertex* v = s->getVertex(coord);
	if (v == NULL) {
		printf("no point\n");
		return;
	}
	face* f = s->sface;
	loop* l = f->floop;
	delete(v);
	delete(l);
	delete(f);
	delete(s);
}
void Euler_Operation::kef(double coord[3], double vcoord[3], loop* alloop) {
	solid* s = alloop->lface->fsolid;
	vertex* v1 = alloop->getVertex(coord);
	vertex* v2 = alloop->getVertex(vcoord);
	if (v1 == NULL) {
		cout << "No point v1" << endl;
		return;
	}
	if (v2 == NULL) {
		cout << "No point v2" << endl;
		return;
	}
	halfedge* he1, *he2;
	halfedge* cur = alloop->lhalfedge;
	while (cur->next != alloop->lhalfedge) {
		if (cur->startv->equal_Coor(v1->coord) && cur->endv->equal_Coor(v2->coord)) {
			he1 = cur;
			he2 = he1->adjacent;
			break;
		}
		if (cur->startv->equal_Coor(v2->coord) && cur->endv->equal_Coor(v1->coord)) {
			he2 = cur;
			he1 = he2->adjacent;
			break;
		}
		cur = cur->next;
	}
	if (he1 == NULL || he2 == NULL) {
		cout << "No line between v1 and v2" << endl;
		return;
	}
	edge* del_edge = he1->hedge;
	loop* l = he1->hloop;
	face* f = l->lface;
	cur = he2->next;
	while (cur != NULL) {
		if (cur == he2) { 
			break;
		}
		cur->hloop = alloop;
		cur = cur->next;
	}
	he1->next->pre = he1->pre;
	he1->pre->next = he1->next;
	he2->pre->next = he2->next;
	he2->next->pre = he2->pre;
	delete(he1);
	delete(he2);
	alloop->lhalfedge = he2->next;
	if (del_edge->next != NULL && del_edge->pre != NULL) {
		del_edge->next->pre = del_edge->pre;
		del_edge->pre->next = del_edge->next;
	}
	else if (del_edge->next == NULL && del_edge->pre != NULL) {
		del_edge->pre->next = NULL;
		s->sedge = del_edge->pre;
	}
	else {
		del_edge->next->pre = NULL;
	}
	delete(del_edge);
	//删除环
	if (l->next != NULL && l->pre != NULL) {
		l->next->pre = l->pre;
		l->pre->next = l->next;
	}
	else if (l->next == NULL && l->pre != NULL) {
		l->pre->next = NULL;
		l->lface->floop = l->pre;
	}
	else {
		l->next->pre = NULL;
		l->lface->floop = l->next;
	}
	delete(l);
	//删除面
	if (f->next!= NULL && f->pre != NULL) {
		f->next->pre = f->pre;
		f->pre->next = f->next;
	}
	else if (f->next == NULL && f->pre != NULL) {
		f->pre->next = NULL;

	}
	else {
		f->next->pre = NULL;
		f->fsolid->sface = f->next;
	}
	delete(f);

}
void Euler_Operation::mekr(double coord[3], double vcoord[3], loop* innerloop, loop* outerloop) {
	//生成一条边，连接两个环，使之成为一个环，然后删掉innerloop
	//coord为外环上面的点，vcoord为内环上面的点
	vertex* v1 = outerloop->getVertex(coord);
	vertex* v2 = innerloop->getVertex(vcoord);
	if (v1 == NULL) {
		cout << "No point v1" << endl;
		return;
	}
	if (v2 == NULL) {
		cout << "No point v2" << endl;
		return;
	}
	solid* s = outerloop->lface->fsolid;
	face* f = outerloop->lface;
	edge* e = new edge();
	halfedge* he1 = new halfedge();
	halfedge* he2 = new halfedge();
	he1->startv = v1;
	he1->endv = v2;
	he2->startv = v2;
	he2->endv = v1;
	he1->adjacent = he2;
	he2->adjacent = he1;
	e->halfedge1 = he1;
	e->halfedge2 = he2;
	he1->hedge = e;
	he2->hedge = e;
	halfedge* cur = outerloop->lhalfedge;
	for (; !(cur->endv->equal_Coor(v1->coord)); cur = cur->next);
	halfedge* h1 = cur;
	cur = innerloop->lhalfedge;
	for (; !(cur->startv->equal_Coor(v2->coord)); cur = cur->next);
	halfedge* h2 = cur;
	cur = h2;
	while (cur->next != h2) {
		cur->hloop = outerloop;
		cur = cur->next;
	}
	he1->next = h2;
	h2->pre->next = he2;
	he2->pre = h2->pre;
	h2->pre = he1;

	he2->next = h1->next;
	h1->next->pre = he2;
	he1->pre = h1;
	h1->next = he1;

	edge* se = s->sedge;
	if (se == NULL) {
		s->sedge = e;
	}
	else {
		while (se->next != NULL) {
			se = se->next;
		}
		se->next = e;
		e->pre = se;
	}
	//删除innerloop
	if (innerloop->next != NULL && innerloop->pre != NULL) {
		innerloop->next->pre = innerloop->pre;
		innerloop->pre->next = innerloop->next;
	}
	else if (innerloop->next == NULL && innerloop->pre != NULL) {
		innerloop->pre->next = NULL;
	}
	else {
		innerloop->next->pre = NULL;
		f->floop = innerloop->next;
	}
	delete(innerloop);

}
void Euler_Operation::mfkrh(loop* innerloop,loop* outerloop) {
	if (innerloop == NULL || outerloop == NULL) {
		cout << "No loop" << endl;
		return;
	}
	solid* s = outerloop->lface->fsolid;
	face* f = new face();
	if (innerloop->next != NULL && innerloop->pre != NULL) {
		innerloop->next->pre = innerloop->pre;
		innerloop->pre->next = innerloop->next;
	}
	else if (innerloop->next==NULL&&innerloop->pre!=NULL) {
		innerloop->pre->next = NULL;

	}
	else {
		innerloop->next->pre = NULL;
		innerloop->lface->floop = innerloop->next;
	}
	innerloop->lface = f;
	f->floop = innerloop;
	f->fsolid = s;
	face* ff = s->sface;
	if (ff == NULL) {
		s->sface = f;
	}
	else {
		while (ff->next != NULL) {
			ff = ff->next;
		}
		ff->next = f;
		f->pre = ff;
	}
	
}
solid* Euler_Operation::sweep(face* f, double vector[3]) {
	loop* loopPoints[1000];
	loop * firstLoop = f->floop;
	loop* l = firstLoop;
	int num = 0;
	while (l != NULL) {
		loopPoints[num++] = l;
		l = l->next;
	}
	for (int i = 0; i < num; i++) {
		l = loopPoints[i];
		halfedge* he = l->lhalfedge;
		vertex* firstv = he->startv;
		double coord[3];
		for (int j = 0; j < 3; j++) {
			coord[j] = firstv->coord[j] + vector[j];
		}
		vertex* firstup = new vertex(coord);
		mev(firstv->coord, coord, l);
		vertex* prevup = firstup;
		he = he->next;
		vertex* nextv = he->startv;
		while (!(nextv->equal_Coor(firstv->coord))) {
			for (int j = 0; j < 3; j++) {
				coord[j] = nextv->coord[j] + vector[j];
			}
			vertex* up = new vertex(coord);
			mev(nextv->coord,coord,l);
			l = mef(prevup->coord,up->coord,l);
			prevup = up;
			he = he->next;
			nextv = he->startv;
		}
		mef(prevup->coord,firstup->coord,l);
	}
	return f->fsolid;
}