#include "renderer.h"


using namespace std;
namespace render {
	
	void renderPoint(mesh::point pt, vec3d vertex, screen sc, vec3d scNormal, std::vector < std::vector<CHAR_INFO>>* data, std::vector < std::vector<double>>* zBuffer);
	void renderLine(mesh::line ln, vec3d vertex, screen sc, vec3d scNormal, std::vector < std::vector<CHAR_INFO>>* data, std::vector < std::vector<double>>* zBuffer);
	void renderTriangle(mesh::triangle tr, vec3d vertex, screen sc, vec3d scNormal, std::vector < std::vector<CHAR_INFO>>* data, std::vector < std::vector<double>>* zBuffer);

	vector<mesh::point>* points;
	vector<mesh::line>* lines;
	vector<mesh::triangle>* triangles;

	vector< vector< CHAR_INFO>>  render(screen sc, vec3d vertex, CHAR_INFO background) {
		vector < vector<CHAR_INFO>> data;
		vector < vector<double>> zBuffer;
		//fill the zBuffers and data
		{
			vector<CHAR_INFO> t0;
			vector<double> t1;
			for (int i = 0; i < sc.xPixels; ++i) {
				t0.push_back(background);
				t1.push_back(-1);
			}
			for (int i = 0; i < sc.yPixels; ++i) {
				data.push_back(t0);
				zBuffer.push_back(t1);
			}

		}

		//calculating screen normal
		vec3d scNormal = vec3d::cross(sc.down, sc.right);
		if (vec3d::dot(scNormal, vec3d::subtract(sc.TopLeft, vertex)) < 0) {
			scNormal.multiply(-1);
		}
		if (scNormal.normalize()) {
			goto end;
		}

		//draw points
		for (int i = 0; i < (*points).size(); ++i) {
			renderPoint((*points)[i], vertex, sc, scNormal, &data, &zBuffer);
		}

		//draw lines
		for (int i = 0; i < (*lines).size(); ++i) {
			renderLine((*lines)[i], vertex, sc, scNormal, &data, &zBuffer);
		}

		//draw triangles
		for (int i = 0; i < (*triangles).size(); ++i) {
			renderTriangle((*triangles)[i], vertex, sc, scNormal, &data, &zBuffer);
		}

	end:
		return data;

	}

	bool isPtVisible(vec3d camPos, screen* sc, vector < vector<double>>* zBuffer) {
		if ((camPos.x < 0) || (camPos.y < 0) || (camPos.x >= (*sc).xPixels) || (camPos.y >= (*sc).yPixels)) {
			return 0;
		}
		else {
			if ((*zBuffer)[camPos.y][camPos.x] > camPos.z || (*zBuffer)[camPos.y][camPos.x] == -1) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}

	void drawPointRaw(vec3d camPos, CHAR_INFO d, vector < vector<CHAR_INFO>>* data, vector < vector<double>>* zBuffer) {
		(*data)[camPos.y][camPos.x] = d;
		(*zBuffer)[camPos.y][camPos.x] = camPos.z;
	}

	void drawPoint(vec3d camPos, CHAR_INFO d, screen* sc, vector < vector<CHAR_INFO>>* data, vector < vector<double>>* zBuffer) {
		if (isPtVisible(camPos, sc, zBuffer)) {
			drawPointRaw(camPos, d, data, zBuffer);
		}
	}

	void renderPoint(mesh::point pt, vec3d vertex, screen sc, vec3d scNormal, vector < vector<CHAR_INFO>>* data, vector < vector<double>>* zBuffer) {
		//get position of point;
		linearMathD::line l(vertex, vec3d::subtract(pt.pt, vertex));
		if (linearMathD::errCode) return;
		linearMathD::plane p(sc.TopLeft, scNormal);
		vec3d ans = linearMathD::rayCast(l, p);
		if (linearMathD::errCode) return;

		//getting screen position
		vec3d camPos;
		camPos.x = (int)(vec3d::dot(vec3d::subtract(ans, sc.TopLeft), sc.right) * sc.xPixels / sc.right.mag2());
		camPos.y = (int)(vec3d::dot(vec3d::subtract(ans, sc.TopLeft), sc.down) * sc.yPixels / sc.down.mag2());
		camPos.z = vec3d::dot(vec3d::subtract(pt.pt, vertex), scNormal);
		if (!isPtVisible(camPos, &sc, zBuffer))return;
		if (camPos.z <= vec3d::dot(vec3d::subtract(sc.TopLeft, vertex), scNormal)) return;
		renderData rd;
		if ((*(pt.shaderPointer)).getRenderMask().camPos) {
			rd.camPos = camPos;
		}
		if ((*(pt.shaderPointer)).getRenderMask().mesh) {
			rd.mesh = 'p';//p for point
		}
		if ((*(pt.shaderPointer)).getRenderMask().pts) {
			rd.pts[0] = pt.pt;
		}
		if ((*(pt.shaderPointer)).getRenderMask().realPos) {
			rd.realPos = pt.pt;
		}
		if ((*(pt.shaderPointer)).getRenderMask().s) {
			rd.s = sc;
		}
		if ((*(pt.shaderPointer)).getRenderMask().vertex) {
			rd.vertex = vertex;
		}
		CHAR_INFO d = (*(pt.shaderPointer)).shade(rd);
		drawPointRaw(camPos, d, data, zBuffer);
	}

	void renderLine(mesh::line ln, vec3d vertex, screen sc, vec3d scNormal, vector < vector<CHAR_INFO>>* data, vector < vector<double>>* zBuffer) {
		linearMathD::plane pl(sc.TopLeft, scNormal);
		linearMathD::line intersectLns[2];
		int badVertex = 2;
		//
		if (linearMathD::aDistance(ln.pts[0], pl) * linearMathD::aDistance(vertex, pl) >= 0) badVertex = 0;
		//
		intersectLns[0].set(vertex, vec3d::subtract(ln.pts[0], vertex));
		//if (math::errCode)badVertex = 0;
		//
		if (linearMathD::aDistance(ln.pts[1], pl) * linearMathD::aDistance(vertex, pl) >= 0) {
			if (badVertex == 0) {
				return;
			}
			else {
				badVertex = 1;
			}
		}
		intersectLns[1].set(vertex, vec3d::subtract(ln.pts[1], vertex));
		vec3d tempPts[2];
		double ratio = 1;
		if (badVertex == 2) {
			tempPts[0] = linearMathD::rayCast(intersectLns[0], pl);
			tempPts[1] = linearMathD::rayCast(intersectLns[1], pl);
		}
		if (badVertex != 2) {
			linearMathD::line templ;
			templ.set(ln.pts[0], vec3d::subtract(ln.pts[0], ln.pts[1]));
			tempPts[badVertex] = linearMathD::intersection(templ, pl);
			//
			tempPts[!badVertex] = linearMathD::rayCast(intersectLns[!badVertex], pl);
			//
			ratio = vec3d::subtract(ln.pts[!badVertex], tempPts[badVertex]).mag() / vec3d::subtract(ln.pts[0], ln.pts[1]).mag();
		}
		vec3d camPos[2];
		camPos[0].x = (int)(vec3d::dot(vec3d::subtract(tempPts[0], sc.TopLeft), sc.right) * sc.xPixels / sc.right.mag2());
		camPos[0].y = (int)(vec3d::dot(vec3d::subtract(tempPts[0], sc.TopLeft), sc.down) * sc.yPixels / sc.down.mag2());
		camPos[1].x = (int)(vec3d::dot(vec3d::subtract(tempPts[1], sc.TopLeft), sc.right) * sc.xPixels / sc.right.mag2());
		camPos[1].y = (int)(vec3d::dot(vec3d::subtract(tempPts[1], sc.TopLeft), sc.down) * sc.yPixels / sc.down.mag2());


		if (badVertex == 2) {
			camPos[0].z = vec3d::dot(vec3d::subtract(ln.pts[0], vertex), scNormal);
			camPos[1].z = vec3d::dot(vec3d::subtract(ln.pts[1], vertex), scNormal);
		}
		else if (badVertex == 1) {
			camPos[0].z = vec3d::dot(vec3d::subtract(ln.pts[0], vertex), scNormal);
			camPos[1].z = vec3d::dot(vec3d::subtract(sc.TopLeft, vertex), scNormal);
		}
		else if (badVertex == 0) {
			camPos[0].z = vec3d::dot(vec3d::subtract(sc.TopLeft, vertex), scNormal);
			camPos[1].z = vec3d::dot(vec3d::subtract(ln.pts[1], vertex), scNormal);
		}

		int minx = 0;
		int miny = 0;
		int maxx = sc.xPixels - 1;
		int maxy = sc.yPixels - 1;

		if (camPos[0].x > camPos[1].x) {
			if (camPos[0].x < maxx) {
				maxx = camPos[0].x;
			}
			if (camPos[1].x > minx) {
				minx = camPos[1].x;
			}
		}
		else {
			if (camPos[1].x < maxx) {
				maxx = camPos[1].x;
			}
			if (camPos[0].x > minx) {
				minx = camPos[0].x;
			}
		}

		if (camPos[0].y > camPos[1].y) {
			if (camPos[0].y < maxy) {
				maxy = camPos[0].y;
			}
			if (camPos[1].y > miny) {
				miny = camPos[1].y;
			}
		}
		else {
			if (camPos[1].y < maxy) {
				maxy = camPos[1].y;
			}
			if (camPos[0].y > miny) {
				miny = camPos[0].y;
			}
		}
		char coord;
		renderData rd;
		CHAR_INFO ch;
		if ((maxx - minx == 0) && (maxy - miny == 0)) {
			//draw one of the points
			if (camPos[0].z > camPos[1].z) {
				if (!isPtVisible(camPos[1], &sc, zBuffer))return;
				if ((*(ln.shaderPointer)).getRenderMask().camPos) {
					rd.camPos = camPos[1];
				}
				if ((*(ln.shaderPointer)).getRenderMask().mesh) {
					rd.mesh = 'l';
				}
				if ((*(ln.shaderPointer)).getRenderMask().pts) {
					rd.pts[0] = ln.pts[0];
					rd.pts[1] = ln.pts[1];
				}
				if ((*(ln.shaderPointer)).getRenderMask().relativePos || (*(ln.shaderPointer)).getRenderMask().realPos) {
					if (badVertex == 1) {
						rd.relativePos.x = ratio;
					}
					else {
						rd.relativePos.x = 1;
					}
				}
				if ((*(ln.shaderPointer)).getRenderMask().realPos) {
					rd.realPos = vec3d::add(vec3d::multiply(ln.pts[1], rd.relativePos.x), vec3d::multiply(ln.pts[0], 1 - rd.relativePos.x));
				}
				if ((*(ln.shaderPointer)).getRenderMask().s) {
					rd.s = sc;
				}
				if ((*(ln.shaderPointer)).getRenderMask().vertex) {
					rd.vertex = vertex;
				}
				ch = (*(ln.shaderPointer)).shade(rd);
				drawPointRaw(camPos[1], ch, data, zBuffer);
			}
			else
			{
				if (!isPtVisible(camPos[0], &sc, zBuffer))return;
				if ((*(ln.shaderPointer)).getRenderMask().camPos) {
					rd.camPos = camPos[0];
				}
				if ((*(ln.shaderPointer)).getRenderMask().mesh) {
					rd.mesh = 'l';
				}
				if ((*(ln.shaderPointer)).getRenderMask().pts) {
					rd.pts[0] = ln.pts[0];
					rd.pts[1] = ln.pts[1];
				}
				if ((*(ln.shaderPointer)).getRenderMask().relativePos || (*(ln.shaderPointer)).getRenderMask().realPos) {
					if (badVertex != 0) {
						rd.relativePos.x = 0;
					}
					else {
						rd.relativePos.x = 1 - ratio;
					}
				}
				if ((*(ln.shaderPointer)).getRenderMask().realPos) {
					rd.realPos = vec3d::add(vec3d::multiply(ln.pts[1], rd.relativePos.x), vec3d::multiply(ln.pts[0], 1 - rd.relativePos.x));
				}
				if ((*(ln.shaderPointer)).getRenderMask().s) {
					rd.s = sc;
				}
				if ((*(ln.shaderPointer)).getRenderMask().vertex) {
					rd.vertex = vertex;
				}
				ch = (*ln.shaderPointer).shade(rd);
				drawPointRaw(camPos[0], ch, data, zBuffer);
			}
		}
		else {
			if (abs(camPos[0].x - camPos[1].x) > abs(camPos[0].y - camPos[1].y)) {
				coord = 0;
			}
			else {
				coord = 1;
				maxx = maxy;
				minx = miny;
			}
			//loop
			for (int i = minx; i <= maxx; ++i) {//for every pixel
				vec3d cPosPixel;
				linearMathD::line pixelLine;
				pixelLine.set(vec3d::add(camPos[0], vec3d(0.5, 0.5, 0)), vec3d::subtract(camPos[1], camPos[0]));
				cPosPixel = linearMathD::getPt(pixelLine, i + 0.5, (coordinateName)coord);
				if (linearMathD::errCode)return;
				cPosPixel.x = (int)(cPosPixel.x);
				cPosPixel.y = (int)(cPosPixel.y);
				if (!isPtVisible(cPosPixel, &sc, zBuffer))continue;
				if ((*(ln.shaderPointer)).getRenderMask().camPos) {
					rd.camPos = cPosPixel;
				}
				if ((*(ln.shaderPointer)).getRenderMask().mesh) {
					rd.mesh = 'l';
				}
				if ((*(ln.shaderPointer)).getRenderMask().pts) {
					rd.pts[0] = ln.pts[0];
					rd.pts[1] = ln.pts[1];
				}
				if ((*(ln.shaderPointer)).getRenderMask().relativePos || (*(ln.shaderPointer)).getRenderMask().realPos) {
					if (badVertex != 0) {
						rd.relativePos.x = vec3d::subtract(camPos[0], cPosPixel).mag() * ratio / vec3d::subtract(camPos[0], camPos[1]).mag();
					}
					else {
						rd.relativePos.x = 1 - (vec3d::subtract(camPos[1], cPosPixel).mag() * ratio / vec3d::subtract(camPos[1], camPos[0]).mag());
					}
				}
				if ((*(ln.shaderPointer)).getRenderMask().realPos) {
					rd.realPos = vec3d::add(vec3d::multiply(ln.pts[1], rd.relativePos.x), vec3d::multiply(ln.pts[0], 1 - rd.relativePos.x));
				}
				if ((*(ln.shaderPointer)).getRenderMask().s) {
					rd.s = sc;
				}
				if ((*(ln.shaderPointer)).getRenderMask().vertex) {
					rd.vertex = vertex;
				}
				ch = (*ln.shaderPointer).shade(rd);
				drawPointRaw(cPosPixel, ch, data, zBuffer);

			}
		}


	}

	void renderTriangle(mesh::triangle tr, vec3d vertex, screen sc, vec3d scNormal, vector < vector<CHAR_INFO>>* data, vector < vector<double>>* zBuffer) {
		linearMathD::plane scPlane;
		scPlane.set(sc.TopLeft, scNormal);
		//calculate camerapos of each vertex
		vec3d camPoses[3];//of drawable vertices
		vector <char> drawableVertices;
		vector <char> nonDrawableVertices;
		linearMathD::line intersectionLns[3];
		bool halfCheck0[3] = { 0,0,0 };
		bool halfCheck1[3] = { 0,0,0 };
		for (int i = 0; i < 3; ++i) {
			intersectionLns[i].set(vertex, vec3d::subtract(tr.pts[i], vertex));
			//halfCheck0[i] = math::errCode;
			//
			halfCheck0[i] = (linearMathD::aDistance(tr.pts[i], scPlane) * linearMathD::aDistance(vertex, scPlane) >= 0);
			//
			vec3d tempCamPos = linearMathD::rayCast(intersectionLns[i], scPlane);
			halfCheck1[i] = linearMathD::errCode;
			if (halfCheck0[i] || halfCheck1[i]) {
				nonDrawableVertices.push_back(i);
			}
			else {
				drawableVertices.push_back(i);
				//convert to pixel space
				camPoses[i].x = (int)(vec3d::dot(vec3d::subtract(tempCamPos, sc.TopLeft), sc.right) * sc.xPixels / sc.right.mag2());
				camPoses[i].y = (int)(vec3d::dot(vec3d::subtract(tempCamPos, sc.TopLeft), sc.down) * sc.yPixels / sc.down.mag2());
				camPoses[i].z = vec3d::dot(vec3d::subtract(tr.pts[i], vertex), scNormal);
			}
		}

		if (nonDrawableVertices.size() == 3)return;
		vector <linearMathD::line> pixelPlaneLines;
		//add lines connecting drawable points
		for (int i = 0; i < drawableVertices.size() - 1; ++i) {
			for (int j = i + 1; j < drawableVertices.size(); ++j) {
				pixelPlaneLines.push_back(linearMathD::line(camPoses[drawableVertices[i]], vec3d::subtract(camPoses[drawableVertices[j]], camPoses[drawableVertices[i]])));
			}
		}
		//find lines joining drawable and non drawable points
		vector<vec3d> camPosesND;//non drawable points
		for (int i = 0; i < nonDrawableVertices.size(); ++i) {
			for (int j = 0; j < drawableVertices.size(); ++j) {
				linearMathD::line intersectionL;
				intersectionL.set(tr.pts[nonDrawableVertices[i]], vec3d::subtract(tr.pts[drawableVertices[j]], tr.pts[nonDrawableVertices[i]]));
				vec3d intersectionPt = linearMathD::intersection(intersectionL, scPlane);
				vec3d camPosIPt;
				camPosIPt.x = (int)(vec3d::dot(vec3d::subtract(intersectionPt, sc.TopLeft), sc.right) * sc.xPixels / sc.right.mag2());
				camPosIPt.y = (int)(vec3d::dot(vec3d::subtract(intersectionPt, sc.TopLeft), sc.down) * sc.yPixels / sc.down.mag2());
				camPosIPt.z = vec3d::dot(vec3d::subtract(sc.TopLeft, vertex), scNormal);
				camPosesND.push_back(camPosIPt);
				pixelPlaneLines.push_back(linearMathD::line(camPosIPt, vec3d::subtract(camPoses[drawableVertices[j]], camPosIPt)));
			}
		}

		//pushback line joining the points that are not drawable
		if (camPosesND.size() > 0) {
			pixelPlaneLines.push_back(linearMathD::line(camPosesND[0], vec3d::subtract(camPosesND[1], camPosesND[0])));
		}



		//determine the pixel plane
		linearMathD::plane pixelPlane;
		{
			vector<vec3d> pts;
			for (int i = 0; i < drawableVertices.size(); ++i) {
				pts.push_back(camPoses[drawableVertices[i]]);
			}
			for (int i = 0; i < camPosesND.size(); ++i) {
				pts.push_back(camPosesND[i]);
			}
			vec3d pixelPNormal = vec3d::cross(vec3d::subtract(pts[1], pts[0]), vec3d::subtract(pts[2], pts[0]));
			if (pixelPNormal.mag2() == 0)return;
			pixelPlane.set(pts[0], pixelPNormal);
		}

		//lines in the pixel plane have been accuried find start and end x coord for each y
		vector<int>startx;
		vector<int>endx;
		int starty = -1;
		for (int i = 0; i < sc.yPixels; ++i) {//for each y
			int s = sc.xPixels, e = -1;
			for (int j = 0; j < pixelPlaneLines.size(); ++j) {
				vec3d ans;
				char result = linearMathD::getPtIn(pixelPlaneLines[j].getPt(), vec3d::add(pixelPlaneLines[j].getPt(), pixelPlaneLines[j].getDr()), i, (coordinateName)1, &ans);
				if (result == 2) {
					//if (starty != -1)continue;
					continue;
				}
				else if (result == 0) {
					//if (starty == -1) { starty = i; }
					if ((int)(ans.x) > e)e = (int)(ans.x);
					if ((int)(ans.x) < s)s = (int)(ans.x);
				}
				else {
					ans = pixelPlaneLines[j].getPt();
					if (ans.y == i) {
						//if (starty == -1) { starty = i; }
						if ((int)(ans.x) > e)e = (int)(ans.x);
						if ((int)(ans.x) < s)s = (int)(ans.x);
						ans = vec3d::add(pixelPlaneLines[j].getPt(), pixelPlaneLines[j].getDr());
						if ((int)(ans.x) > e)e = (int)(ans.x);
						if ((int)(ans.x) < s)s = (int)(ans.x);
					}
				}
			}

			if (s<sc.xPixels && e > -1) {
				//
				if (starty == -1) { starty = i; }
				//
				if (s < 0)s = 0;
				if (e > (sc.xPixels - 1))e = (sc.xPixels - 1);
				startx.push_back(s);
				endx.push_back(e);
			}
			else {
				if (starty != -1)break;
			}

		}

		//startx and endx calculated

		//draw each pixel
		renderData rd;
		for (int i = 0; i < startx.size(); ++i) {
			for (int j = startx[i]; j <= endx[i]; ++j) {//for every pixel
				//get pixel cam pos
				vec3d ans;
				ans.x = j;
				ans.y = i + starty;
				if (linearMathD::getPt(pixelPlane, &ans, coordinateName::zCoordinate))return;
				if (!isPtVisible(ans, &sc, zBuffer))continue;
				//calculate rd
				if ((*tr.shaderPointer).getRenderMask().camPos) {
					rd.camPos = ans;
				}
				if ((*tr.shaderPointer).getRenderMask().mesh) {
					rd.mesh = 't';
				}
				if ((*tr.shaderPointer).getRenderMask().pts) {
					rd.pts[0] = tr.pts[0];
					rd.pts[1] = tr.pts[1];
					rd.pts[2] = tr.pts[2];
				}
				if ((*tr.shaderPointer).getRenderMask().realPos || (*tr.shaderPointer).getRenderMask().relativePos) {
					linearMathD::line templ;
					vec3d pt = vec3d::add(sc.TopLeft, vec3d::add(vec3d::multiply(sc.right, j / (double)sc.xPixels), vec3d::multiply(sc.down, (i + starty) / (double)sc.yPixels)));
					templ.set(vertex, vec3d::subtract(pt, vertex));
					linearMathD::plane tempPlane;
					tempPlane.set(tr.pts[0], vec3d::cross(vec3d::subtract(tr.pts[1], tr.pts[0]), vec3d::subtract(tr.pts[2], tr.pts[0])));
					if (linearMathD::errCode)return;
					rd.realPos = linearMathD::intersection(templ, tempPlane);
				}
				if ((*tr.shaderPointer).getRenderMask().relativePos) {
					double lambda1, lambda2;
					lambda2 = vec3d::cross(vec3d::subtract(rd.realPos, tr.pts[0]), vec3d::normalize(vec3d::subtract(tr.pts[1], tr.pts[0]))).mag() /
						vec3d::cross(vec3d::subtract(tr.pts[2], tr.pts[0]), vec3d::normalize(vec3d::subtract(tr.pts[1], tr.pts[0]))).mag();
					lambda1 = (vec3d::dot(vec3d::subtract(rd.realPos, tr.pts[0]), vec3d::normalize(vec3d::subtract(tr.pts[1], tr.pts[0]))) - lambda2 * vec3d::dot(vec3d::subtract(tr.pts[2], tr.pts[0]), vec3d::normalize(vec3d::subtract(tr.pts[1], tr.pts[0])))) / vec3d::subtract(tr.pts[1], tr.pts[0]).mag();
					if (lambda1 < 0)lambda1 = 0;
					if (lambda2 < 0)lambda2 = 0;
					rd.relativePos.x = lambda1;
					rd.relativePos.y = lambda2;
				}
				if ((*tr.shaderPointer).getRenderMask().s)rd.s = sc;
				if ((*tr.shaderPointer).getRenderMask().vertex)rd.vertex = vertex;

				CHAR_INFO d;
				d = (*tr.shaderPointer).shade(rd);
				drawPointRaw(ans, d, data, zBuffer);
			}
		}
	}
}


vector<vector <CHAR_INFO>> graphicalWorld::render(screen sc, vec3d vertex, CHAR_INFO background){
	render::points = &points;
	render::lines = &lines;
	render::triangles = &triangles;
	return render::render(sc, vertex, background);
}