#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <fstream>
#include <string>
#include <map>
#include <GL/glew.h>
#include <iostream>
using namespace std;

enum TextureState { TextureOn, TextureOff };
enum DisplayColor { None, Green, Red };
enum DisplayMode { Wireframe, Flat, Flatlines };
enum ShadingMode { FlatS, Gouraud, Phong };
enum TransformMode { Rotation, Translation };

class ModelManager {
public:
	ModelManager();
	~ModelManager();

	bool importModel(const std::string& fileName);
	bool loadTextures();
	void renderTheModel();

public:
	TextureState textureState;
	DisplayMode displayMode;
	DisplayColor displayColor;
	ShadingMode shadingMode;
	TransformMode transformMode;
	int subdivisionDepth;          


};

#endif // !MODELMANAGER_H
