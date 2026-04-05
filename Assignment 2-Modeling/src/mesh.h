#pragma once

// maximum number of vertices and triangles
#define MAXV 1000000
#define MAXT 1000000

typedef int OrTri;
typedef int tIdx;

inline OrTri makeOrTri(tIdx t, int version) { return (t << 3) | version; };
inline tIdx idx(OrTri ot) { return ot >> 3; };
inline int ver(OrTri ot) { return ot & 0b111; };
// Rotate to the next directed edge within the same oriented triangle.
// Versions 0->1->2->0 (CCW face), 3->4->5->3 (CW face).
inline OrTri enext(OrTri ot) {
	int v = ver(ot);
	int newV = (v < 3) ? (v + 1) % 3 : 3 + (v - 3 + 1) % 3;
	return makeOrTri(idx(ot), newV);
}

// Flip the orientation of the directed edge (swap org and dest),
// staying on the same triangle edge.
// Pairs: (0<->3), (1<->4), (2<->5)
inline OrTri sym(OrTri ot) {
	return makeOrTri(idx(ot), (ver(ot) + 3) % 6);
}


class myObjType {
	int vcount = 0;
	int tcount = 0;

	double vlist[MAXV][3];   // vertices list
	int tlist[MAXT][3];      // triangle list
	int fnlist[MAXT][3];     // fnext list: fnlist[t][k] = adjacent OrTri sharing edge k of triangle t
	double nlist[MAXT][3];   // storing triangle normals
	double vnlist[MAXV][3];  // storing vertex normals (average of surrounding triangles)
	
	double lmax[3];          // the maximum coordinates of x,y,z
	double lmin[3];          // the minimum coordinates of x,y,z

	int statMinAngle[18]; // each bucket is  degrees has a 10 degree range from 0 to 180 degree
	int statMaxAngle[18]; 


public:
	myObjType() { vcount = 0; tcount = 0; };
	void readFile(char* filename);  // assumming file contains a manifold
	void writeFile(char* filename);
	void draw(bool smooth);
    void computeStat();

	// Return the origin vertex index of the directed edge.
	// ver 0,1,2 (CCW): org is tlist[t][v]
	// ver 3,4,5 (CW):  org is tlist[t][(v-3+1)%3]  (i.e. the dest of the sym edge)
	inline int org(OrTri ot) const {
		int v = ver(ot);
		return (v < 3) ? tlist[idx(ot)][v]
		               : tlist[idx(ot)][(v - 3 + 1) % 3];
	}

	// Return the destination vertex index of the directed edge.
	// ver 0,1,2 (CCW): dest is tlist[t][(v+1)%3]
	// ver 3,4,5 (CW):  dest is tlist[t][v-3]        (i.e. the org of the sym edge)
	inline int dest(OrTri ot) const {
		int v = ver(ot);
		return (v < 3) ? tlist[idx(ot)][(v + 1) % 3]
		               : tlist[idx(ot)][v - 3];
	}

	// Return the OrTri of the same directed edge on the adjacent triangle.
	// For a manifold mesh, fnext(fnext(ot)) == ot.
	OrTri fnext(OrTri ot) const;

};


