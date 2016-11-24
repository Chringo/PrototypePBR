#include "FileLoader.h"

FileLoader::FileLoader()
{

}

FileLoader::FileLoader(fileLoaderDesc & desc)
{
	this->desc = desc;
}

FileLoader::~FileLoader()
{
	for each (Model* m in models)
	{
		delete m;
	}

	delete vertexData;
	delete indexData;
}

void FileLoader::loadFile(char * filePath) //creates a renderable model from a BBF file
{
	Model * model = new Model(desc.gDevice);
	infile.open(filePath, std::fstream::binary);

	MainHeader mainHeader;
	infile.read((char*)&mainHeader, sizeof(MainHeader));
	indexData = new Index*[mainHeader.meshes];

	for (int i = 0; i < mainHeader.meshes; i++)
	{
		MeshHeader meshHeader;
		infile.read((char*)&meshHeader, sizeof(MeshHeader));
	
		/*float floatMat[16];
		for (int i = 0; i < 16; i++)
		{
			floatMat[i] = (float)meshHeader.transformationMatrix[i];
		}*/

		/*Static mesh.*/
		if (meshHeader.skeleton == false)
		{
			vertexData = new Vertex*[mainHeader.meshes];
			vertexData[i] = new Vertex[meshHeader.vertices];
			infile.read((char*)vertexData[i], sizeof(Vertex)*meshHeader.vertices);
		}
		/*Dynamic mesh*/
		else
		{
			skelVertexData = new SkelVertex*[mainHeader.meshes];
			skelVertexData[i] = new SkelVertex[meshHeader.vertices];
			infile.read((char*)skelVertexData[i], sizeof(SkelVertex) * meshHeader.vertices);
		}

		indexData[i] = new Index[meshHeader.indexLength];
		infile.read((char*)indexData[i], sizeof(Index)*meshHeader.indexLength);

		if (meshHeader.skeleton == true)
		{
			model->jointList.resize(meshHeader.jointCount);
			infile.read((char*)model->jointList.data(), sizeof(JointHeader) * meshHeader.jointCount);
			const int jointCount = model->jointList.size();

			for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
			{
				const int animStateCount = model->jointList[jointIndex].animStateCount;
				model->jointList[jointIndex].animationStates.resize(animStateCount);

				infile.read((char*)model->jointList[jointIndex].animationStates.data(), sizeof(AnimationStateHeader) * animStateCount);

				for (int animIndex = 0; animIndex < animStateCount; animIndex++)
				{
					const int keyFrameCount = model->jointList[jointIndex].animationStates[animIndex].keyFrameCount;
					model->jointList[jointIndex].animationStates[animIndex].keyFrames.resize(keyFrameCount);

					infile.read((char*)model->jointList[jointIndex].animationStates[animIndex].keyFrames.data(), sizeof(KeyframeHeader) * keyFrameCount);
				}
			}
		}

		meshDesc mDesc;
		mDesc.FLDesc = &desc;
		mDesc.vertexData = vertexData[i];
		mDesc.indexData = indexData[i];
		//mDesc.transMat = DirectX::XMFLOAT4X4(floatMat);
		mDesc.vertexCount = meshHeader.vertices;
		mDesc.indexCount = meshHeader.indexLength;

		model->hasSkeleton = meshHeader.skeleton;

		model->appendMesh(mDesc);
	}

	//CREATE ZE MODEL PLS
	models.push_back(model);


	infile.close();
}

std::vector<Model*>* FileLoader::getModelVec()
{
	return &models;
}

ID3D11Buffer * FileLoader::getVertexBuffer() const
{
	return models.at(0)->getMVertexBuffer();
}

ID3D11Buffer * FileLoader::getIndexBuffer() const
{
	return models.at(0)->getMIndexBuffer();
}
