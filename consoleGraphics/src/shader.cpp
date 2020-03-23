#include"shader.h"

long unsigned int shader::no = 0;
CHAR_INFO shader::none = { ' ',BACKGROUND_BLUE + 15 };

shader::shader() {
	no++;
}

shader::~shader() {
	no--;
}

CHAR_INFO shader::shade(renderData rd) {
	return none;
}

shader* shader::getSelfPointer() { return this; }

renderDataMask shader::getRenderMask() { return renderDataM; }

unsigned long shader::getNoOfShaders() { return no; }


//solidCharShader
solidCharShader::solidCharShader(CHAR_INFO Data) {
	data = Data;
}
CHAR_INFO solidCharShader::shade(renderData rd) { return data; }


//rd light shader
rdLightShader::rdLightShader(vec3f SurfaceColor, std::vector<light>* Lights) {
	surfaceColor = SurfaceColor;
	renderDataM.realPos = true;
	lights = Lights;
}

CHAR_INFO rdLightShader::shade(renderData rd) {
	vec3f color(0, 0, 0);
	for (int i = 0; i < (*lights).size(); ++i) {
		int d2 = vec3d::subtract((*lights)[i].pt, rd.realPos).mag2();
		if (d2 != 0)
			color = vec3f::add(color, vec3f::multiply(vec3f(surfaceColor.x * (*lights)[i].color.x, surfaceColor.y * (*lights)[i].color.y, surfaceColor.z * (*lights)[i].color.z), (*lights)[i].intensity / d2));
	}
	return color::getColorWithIntensity(color.x, color.y, color.z);
}