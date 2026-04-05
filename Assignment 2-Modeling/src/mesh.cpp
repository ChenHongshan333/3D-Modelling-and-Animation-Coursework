#include "mesh.h"

#ifdef _WIN32
#include <Windows.h>
#include "GL\glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#include "math.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "mesh.h"
#include <map>
#include <queue>
#include <iomanip>
using namespace std;



void myObjType::draw(bool smooth) {

	glEnable(GL_LIGHTING);

	if (smooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glPushMatrix();
	double longestSide = 0.0;
	for (int i = 0; i < 3; i++)
		if ((lmax[i] - lmin[i]) > longestSide)
			longestSide = (lmax[i] - lmin[i]);
	glScalef(4.0 / longestSide, 4.0 / longestSide, 4.0 / longestSide);
	glTranslated(-(lmin[0] + lmax[0]) / 2.0, -(lmin[1] + lmax[1]) / 2.0, -(lmin[2] + lmax[2]) / 2.0);
	for (int i = 1; i <= tcount; i++)
	{
		glBegin(GL_POLYGON);
		if (smooth) {
			// Per-vertex normals for Gouraud shading
			for (int j = 0; j < 3; j++) {
				glNormal3dv(vnlist[tlist[i][j]]);
				glVertex3dv(vlist[tlist[i][j]]);
			}
		} else {
			// Per-face normal for flat shading
			glNormal3dv(nlist[i]);
			for (int j = 0; j < 3; j++)
				glVertex3dv(vlist[tlist[i][j]]);
		}
		glEnd();
	}
	glDisable(GL_LIGHTING);

	// Draw boundary edges (shared by only one triangle) in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	for (int i = 1; i <= tcount; i++)
		for (int k = 0; k < 3; k++)
			if (fnlist[i][k] == -1) {
				glVertex3dv(vlist[tlist[i][k]]);
				glVertex3dv(vlist[tlist[i][(k+1)%3]]);
			}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f); // reset to white

	glPopMatrix();
}

void myObjType::writeFile(char* filename)
{
	ofstream outFile;
	outFile.open(filename);
	if (!outFile.is_open()) {
		cout << "Cannot open file " << filename << " for writing." << endl;
		return;
	}

	outFile << fixed << setprecision(6);
	for (int i = 1; i <= vcount; i++)
		outFile << "v " << vlist[i][0] << " " << vlist[i][1] << " " << vlist[i][2] << "\n";

	for (int i = 1; i <= tcount; i++)
		outFile << "f " << tlist[i][0] << " " << tlist[i][1] << " " << tlist[i][2] << "\n";

	outFile.close();
	cout << "Written to " << filename << endl;
}

void myObjType::readFile(char* filename)
{
	cout << "Opening " << filename << endl;
	ifstream inFile;
	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "We cannot find your file " << filename << endl;
		exit(1);
	}

	string line;
	int i, j;
	bool firstVertex = 1;
	double currCood;

	while (getline(inFile, line))
	{
		if ((line[0] == 'v' || line[0] == 'f') && line[1] == ' ')
		{
			if (line[0] == 'v')
			{
				vcount++;
				i = 1;
				const char* linec = line.data();
				for (int k = 0; k < 3; k++) { // k is 0,1,2 for x,y,z
					while (linec[i] == ' ') i++;
					j = i;
					while (linec[j] != ' ') j++;
					currCood = vlist[vcount][k] = atof(line.substr(i, j - i).c_str());
					if (firstVertex) 
						lmin[k] = lmax[k] = currCood;
					else {
						if (lmin[k] > currCood)
							lmin[k] = currCood;
						if (lmax[k] < currCood)
							lmax[k] = currCood;
					}
					i = j;
				}

				firstVertex = 0;
			}
			if (line[0] == 'f')
			{
				tcount++;
				i = 1;
				const char* linec = line.data();
				for (int k = 0; k < 3; k++) {
					while (linec[i] == ' ') i++;
					j = i;
					while (linec[j] != ' ' && linec[j] != '\\') j++;
					tlist[tcount][k] = atof(line.substr(i, j - i).c_str());
					i = j;
					fnlist[tcount][k] = 0;
					while (linec[j] != ' ') j++;

				}

			}


		}
	}

	// Build fnlist: for each directed edge (A->B) in triangle t at local index k,
	// find the adjacent triangle that has the same edge in opposite direction (B->A),
	// then store its sym so fnlist[t][k] has org=A, dest=B on the other triangle.
	{
		map<pair<int,int>, OrTri> edgeMap;
		for (int t = 1; t <= tcount; t++)
			for (int k = 0; k < 3; k++) {
				int A = tlist[t][k], B = tlist[t][(k+1)%3];
				edgeMap[{A, B}] = makeOrTri(t, k);
			}

		for (int t = 1; t <= tcount; t++)
			for (int k = 0; k < 3; k++) {
				int A = tlist[t][k], B = tlist[t][(k+1)%3];
				auto it = edgeMap.find({B, A});
				fnlist[t][k] = (it != edgeMap.end()) ? sym(it->second) : -1;
			}
	}

	// Compute triangle normals
	for (int i = 1; i <= tcount; i++)
	{
		double* v0 = vlist[tlist[i][0]];
		double* v1 = vlist[tlist[i][1]];
		double* v2 = vlist[tlist[i][2]];

		double e1[3] = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
		double e2[3] = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };

		double nx = e1[1] * e2[2] - e1[2] * e2[1];
		double ny = e1[2] * e2[0] - e1[0] * e2[2];
		double nz = e1[0] * e2[1] - e1[1] * e2[0];

		double len = sqrt(nx * nx + ny * ny + nz * nz);
		if (len > 0.0) {
			nlist[i][0] = nx / len;
			nlist[i][1] = ny / len;
			nlist[i][2] = nz / len;
		}
		else {
			nlist[i][0] = nlist[i][1] = nlist[i][2] = 0.0;
		}
	}

	// Compute vertex normals: accumulate surrounding triangle normals then normalize
	for (int i = 1; i <= vcount; i++)
		vnlist[i][0] = vnlist[i][1] = vnlist[i][2] = 0.0;
	for (int i = 1; i <= tcount; i++)
		for (int j = 0; j < 3; j++) {
			int v = tlist[i][j];
			vnlist[v][0] += nlist[i][0];
			vnlist[v][1] += nlist[i][1];
			vnlist[v][2] += nlist[i][2];
		}
	for (int i = 1; i <= vcount; i++) {
		double len = sqrt(vnlist[i][0]*vnlist[i][0] + vnlist[i][1]*vnlist[i][1] + vnlist[i][2]*vnlist[i][2]);
		if (len > 0.0) { vnlist[i][0] /= len; vnlist[i][1] /= len; vnlist[i][2] /= len; }
	}

    cout << "No. of vertices: " << vcount << endl;
    cout << "No. of triangles: " << tcount << endl;
    computeStat();
}



OrTri myObjType::fnext(OrTri ot) const
{
	int v = ver(ot);
	if (v < 3)
		return fnlist[idx(ot)][v];
	else
		return sym(fnlist[idx(ot)][v - 3]);
}

void myObjType::computeStat()
{
	int i;
    double minAngle = 180.0;
    double maxAngle = 0.0;

	// Zero out the buckets
	for (i = 0; i < 18; i++)
		statMinAngle[i] = statMaxAngle[i] = 0;

	for (i = 1; i <= tcount; i++)
	{
		double* v0 = vlist[tlist[i][0]];
		double* v1 = vlist[tlist[i][1]];
		double* v2 = vlist[tlist[i][2]];

		// Edge vectors from each vertex
		double a[3] = { v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2] }; // v0->v1
		double b[3] = { v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2] }; // v0->v2
		double c[3] = { v0[0]-v1[0], v0[1]-v1[1], v0[2]-v1[2] }; // v1->v0
		double d[3] = { v2[0]-v1[0], v2[1]-v1[1], v2[2]-v1[2] }; // v1->v2

		double lenA = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
		double lenB = sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
		double lenC = sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
		double lenD = sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);

		// Angle at v0: between edges a and b
		double cos0 = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
		cos0 /= (lenA * lenB);
		if (cos0 < -1.0) cos0 = -1.0; if (cos0 > 1.0) cos0 = 1.0;
		double ang0 = acos(cos0) * 180.0 / M_PI;

		// Angle at v1: between edges c (v1->v0) and d (v1->v2)
		double cos1 = c[0]*d[0] + c[1]*d[1] + c[2]*d[2];
		cos1 /= (lenC * lenD);
		if (cos1 < -1.0) cos1 = -1.0; if (cos1 > 1.0) cos1 = 1.0;
		double ang1 = acos(cos1) * 180.0 / M_PI;

		// Angle at v2: remainder
		double ang2 = 180.0 - ang0 - ang1;

		double triMin = ang0 < ang1 ? (ang0 < ang2 ? ang0 : ang2) : (ang1 < ang2 ? ang1 : ang2);
		double triMax = ang0 > ang1 ? (ang0 > ang2 ? ang0 : ang2) : (ang1 > ang2 ? ang1 : ang2);

		if (triMin < minAngle) minAngle = triMin;
		if (triMax > maxAngle) maxAngle = triMax;

		int bucketMin = (int)(triMin / 10.0);
		int bucketMax = (int)(triMax / 10.0);
		if (bucketMin > 17) bucketMin = 17;
		if (bucketMax > 17) bucketMax = 17;
		statMinAngle[bucketMin]++;
		statMaxAngle[bucketMax]++;
	}

    cout << "Min. angle = " << minAngle << endl;
    cout << "Max. angle = " << maxAngle << endl;

	cout << "Statistics for Maximum Angles" << endl;
	for (i = 0; i < 18; i++)
		cout << statMaxAngle[i] << " ";
	cout << endl;
	cout << "Statistics for Minimum Angles" << endl;
	for (i = 0; i < 18; i++)
		cout << statMinAngle[i] << " ";
	cout << endl;

	// Count connected components via BFS on fnlist
	vector<bool> visited(tcount + 1, false);
	int components = 0;
	queue<int> bfsQueue;
	for (i = 1; i <= tcount; i++) {
		if (!visited[i]) {
			components++;
			visited[i] = true;
			bfsQueue.push(i);
			while (!bfsQueue.empty()) {
				int cur = bfsQueue.front();
				bfsQueue.pop();
				for (int k = 0; k < 3; k++) {
					int nb = fnlist[cur][k];
					if (nb != -1) {
						int nbTri = idx(nb);
						if (!visited[nbTri]) {
							visited[nbTri] = true;
							bfsQueue.push(nbTri);
						}
					}
				}
			}
		}
	}
	cout << "No. of components: " << components << endl;
}
