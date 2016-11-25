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

	//delete vertexData;
	delete skelVertexData;
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
			model->tempAnimations.resize(meshHeader.jointCount);

			infile.read((char*)model->jointList.data(), sizeof(JointHeader) * meshHeader.jointCount);

			for (int jointIndex = 0; jointIndex < meshHeader.jointCount; jointIndex++)
			{
				const int animationCount = model->jointList[jointIndex].animStateCount;

				model->tempAnimations[jointIndex].animationCount.resize(animationCount);
				model->tempAnimations[jointIndex].animationData.resize(animationCount);

				infile.read((char*)model->tempAnimations[jointIndex].animationCount.data(), sizeof(AnimationStateHeader) * animationCount);

				for (int animIndex = 0; animIndex < animationCount; animIndex++)
				{
					const int keyframeCount = model->tempAnimations[jointIndex].animationCount[animIndex].keyFrameCount;

					model->tempAnimations[jointIndex].animationData[animIndex].keyframes.resize(keyframeCount);

					infile.read((char*)model->tempAnimations[jointIndex].animationData[animIndex].keyframes.data(), sizeof(KeyframeHeader) * keyframeCount);
				}
			}
		}

		if (meshHeader.skeleton == true)
		{
			meshDesc mDesc; dynamicMesh dynMDesc;
			dynMDesc.FLDesc = &desc;
			dynMDesc.skelVertexData = skelVertexData[i];
			dynMDesc.indexData = indexData[i];
			//mDesc.transMat = DirectX::XMFLOAT4X4(floatMat);
			dynMDesc.vertexCount = meshHeader.vertices;
			dynMDesc.indexCount = meshHeader.indexLength;

			model->appendMesh(mDesc, dynMDesc);
		}

		else
		{
			meshDesc mDesc; dynamicMesh dynMDesc;
			mDesc.FLDesc = &desc;
			mDesc.vertexData = vertexData[i];
			mDesc.indexData = indexData[i];
			//mDesc.transMat = DirectX::XMFLOAT4X4(floatMat);
			mDesc.vertexCount = meshHeader.vertices;
			mDesc.indexCount = meshHeader.indexLength;

			model->appendMesh(mDesc, dynMDesc);
		}

		model->hasSkeleton = meshHeader.skeleton;
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
