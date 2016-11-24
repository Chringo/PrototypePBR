#pragma once

#include <fstream>
#include <iostream>
#include "Model.h"

class FileLoader
{
public:
	FileLoader(fileLoaderDesc&);

	~FileLoader();
	void loadFile(char* filePath);
	std::vector<Model*>* getModelVec();

	ID3D11Buffer * getVertexBuffer() const;
	ID3D11Buffer * getIndexBuffer() const;

private:

	FileLoader();
	fileLoaderDesc desc;
	std::ifstream infile;
	std::vector<Model*> models;
	Vertex ** vertexData;
	SkelVertex ** skelVertexData;
	Index ** indexData;
};




