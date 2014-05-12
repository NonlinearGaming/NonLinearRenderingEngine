/*
-----------------------------------------------------------------------------
This source file is part of NLRE
(NonLinear Rendering Engine)
For the latest info, see https://github.com/AlexandrSachkov/NonLinearRenderingEngine

Copyright (c) 2014 NonLinear Rendering Engine Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "ResourceLoader\NLREAssimpAssetImporter.h"

NLREAssimpAssetImporter::NLREAssimpAssetImporter(NLRERenderingDevice* renderingDevice, NLRETextureLoader* textureLoader)
{
	_renderingDevice = renderingDevice;
	_textureLoader = textureLoader;
	_textureResourcePath = L"";
	useBuiltInTexturePath = true;

	if (!initialize())
	{
		NLRE_Log::err(NLRE_Log::CRITICAL, "Assimp asset importer failed to initialize");
		throw std::exception("Assimp asset importer failed to initialize");
	}
}

NLREAssimpAssetImporter::NLREAssimpAssetImporter(const NLREAssimpAssetImporter& other)
{
}

NLREAssimpAssetImporter::~NLREAssimpAssetImporter()
{
	Assimp::DefaultLogger::kill();
}

bool NLREAssimpAssetImporter::initialize()
{
	if (_renderingDevice == NULL || _textureLoader == NULL)
	{
		NLRE_Log::err(NLRE_Log::CRITICAL, "Un-initialized parameters");
		return false;
	}

#if defined(_DEBUG) || defined(DEBUG)
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
#else
	Assimp::DefaultLogger::create("", Assimp::Logger::NORMAL);
	const unsigned int severity = Assimp::Logger::Err | Assimp::Logger::Warn;
#endif
	
	Assimp::DefaultLogger::get()->attachStream(new NLREAssimpLogStream(), severity);

	return true;
}

bool NLREAssimpAssetImporter::importAssets(std::wstring path, std::vector<NLRE_RenderableAsset*>& assets)
{
	Assimp::Importer importer;
	std::string assetPath(path.begin(), path.end());
	const aiScene* scene = importer.ReadFile(assetPath.c_str(),
		aiProcess_MakeLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_SortByPType |
		aiProcess_RemoveComponent |
		//aiProcess_FlipUVs				|
		aiProcess_SortByPType);

	if (!scene)
	{
		NLRE_Log::err(NLRE_Log::REG, "Failed to import asset: ", assetPath);
		return false;
	}

	assets = loadAsStatic(assetPath, scene);

	return true;
}

std::vector<NLRE_RenderableAsset*> NLREAssimpAssetImporter::loadAsStatic(std::string path, const aiScene* scene)
{
	std::vector<NLRE_RenderableAsset*> assetArr;
	NLRE_Mesh** meshArr = loadMeshes(scene);
	NLRE_Material** materialArr = loadMaterials(path, scene);

	nextNode(scene, scene->mRootNode, scene->mRootNode->mTransformation, meshArr, materialArr, assetArr);
	return assetArr;
}

void NLREAssimpAssetImporter::nextNode(
	const aiScene* scene,
	aiNode* node,
	aiMatrix4x4& accTransform,
	NLRE_Mesh** meshArr,
	NLRE_Material** materialArr,
	std::vector<NLRE_RenderableAsset*>& assetArr)
{
	aiMatrix4x4 transform;

	if (node->mNumMeshes > 0)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			assembleAsset(scene, node->mMeshes[i], transform, meshArr, materialArr, assetArr);
		}
	}
	else
	{
		transform = node->mTransformation * accTransform;
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		nextNode(scene, node->mChildren[i], transform, meshArr, materialArr, assetArr);
	}
}

void NLREAssimpAssetImporter::assembleAsset(
	const aiScene* scene,
	unsigned int meshIndex,
	aiMatrix4x4& transform,
	NLRE_Mesh** meshArr,
	NLRE_Material** materialArr,
	std::vector<NLRE_RenderableAsset*>& assetArr)
{
	NLRE_RenderableAsset* asset = new NLRE_RenderableAsset();
	asset->mesh = meshArr[meshIndex];
	asset->material = materialArr[scene->mMeshes[meshIndex]->mMaterialIndex];
	asset->transform = NLE_FLOAT4X4((const float*)(&transform));

	assetArr.push_back(asset);
}

NLRE_Mesh** NLREAssimpAssetImporter::loadMeshes(const aiScene* scene)
{
	if (!scene->HasMeshes()) return NULL;

	NLRE_Mesh** meshArr = new NLRE_Mesh*[scene->mNumMeshes];
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshArr[i] = new NLRE_Mesh();
		NLRE_GeomStr* geomStreamArr = NULL;
		unsigned int geomStreamLength = 0;
		loadGeometryStream(scene->mMeshes[i], geomStreamArr, geomStreamLength);
		_renderingDevice->createBuffer<NLRE_GeomStr>(NLRE_BIND_VERTEX_BUFFER, NLRE_USAGE_IMMUTABLE, geomStreamArr, geomStreamLength, meshArr[i]->geomBuffer);

		NLRE_Index* indexArr = NULL;
		unsigned int indexArrLength = 0;
		loadIndices(scene->mMeshes[i], indexArr, indexArrLength);
		_renderingDevice->createBuffer<NLRE_Index>(NLRE_BIND_INDEX_BUFFER, NLRE_USAGE_IMMUTABLE, indexArr, indexArrLength, meshArr[i]->indexBuffer);
	}

	return meshArr;
}

void NLREAssimpAssetImporter::loadGeometryStream(aiMesh* mesh, NLRE_GeomStr*& geomStreamArr, unsigned int& streamLength)
{
	if (!mesh->HasPositions()) return;

	streamLength = mesh->mNumVertices;
	geomStreamArr = new NLRE_GeomStr[streamLength];
	for (int i = 0; i < streamLength; i++)
	{
		const aiVector3D* pos = &(mesh->mVertices[i]);
		const aiVector3D* normal = &(mesh->mNormals[i]);
		const aiVector3D* texCoord = &(mesh->mTextureCoords[0][i]);

		geomStreamArr[i].position = NLE_FLOAT3(pos->x, pos->y, pos->z);
		if (mesh->HasNormals())
		{
			geomStreamArr[i].normal = NLE_FLOAT3(normal->x, normal->y, normal->z);
		}
		if (mesh->HasTextureCoords(0))
		{
			geomStreamArr[i].textCoord = NLE_FLOAT2(texCoord->x, texCoord->y);
		}
	}
}

void NLREAssimpAssetImporter::loadIndices(aiMesh* mesh, NLRE_Index*& indexArr, unsigned int& indexArrLength)
{
	if (!mesh->HasFaces()) return;

	indexArrLength = mesh->mNumFaces * 3;
	indexArr = new NLRE_Index[indexArrLength];
	int counter = 0;
	for (int k = 0; k < mesh->mNumFaces; k++)
	{
		const aiFace& tempFace = mesh->mFaces[k];
		for (int m = 0; m < 3; m++)
		{
			indexArr[counter] = tempFace.mIndices[m];
			counter++;
		}
	}
}

NLRE_Material** NLREAssimpAssetImporter::loadMaterials(std::string path, const aiScene* scene)
{
	if (!scene->HasMaterials()) return NULL;

	NLRE_Material** materialArr = new NLRE_Material*[scene->mNumMaterials];
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		materialArr[i] = new NLRE_Material();
		NLRE_MaterialBufferStruct materialBuffStruct;
		loadMaterialBuffer(scene->mMaterials[i], materialBuffStruct);
		_renderingDevice->createBuffer<NLRE_MaterialBufferStruct>(NLRE_BIND_CONSTANT_BUFFER, NLRE_USAGE_IMMUTABLE, &materialBuffStruct, 1, materialArr[i]->materialBuffer);

		loadMaterialParams(scene->mMaterials[i], materialArr[i]);
		loadMaterialTextures(path, scene->mMaterials[i], materialArr[i]);
	}
	return materialArr;
}

void NLREAssimpAssetImporter::loadMaterialParams(aiMaterial* material, NLRE_Material*& nlreMaterial)
{
	material->Get(AI_MATKEY_TWOSIDED, nlreMaterial->twoSided);

	int temp = 0;

	if (material->Get(AI_MATKEY_BLEND_FUNC, temp) == AI_SUCCESS)				nlreMaterial->blendMode = temp;
	if (material->Get(AI_MATKEY_SHADING_MODEL, temp) == AI_SUCCESS)				nlreMaterial->shadingModel = temp;
	if (material->Get(AI_MATKEY_MAPPINGMODE_U_DIFFUSE(0), temp) == AI_SUCCESS)	nlreMaterial->textureMappingMode_u = temp;
	if (material->Get(AI_MATKEY_MAPPINGMODE_V_DIFFUSE(0), temp) == AI_SUCCESS)	nlreMaterial->textureMappingMode_v = temp;
}

void NLREAssimpAssetImporter::loadMaterialBuffer(aiMaterial* material, NLRE_MaterialBufferStruct& materialStruct)
{
	aiColor3D tempColor;
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, tempColor) == AI_SUCCESS)
	{
		materialStruct.diffuseColor.x = tempColor.r;
		materialStruct.diffuseColor.y = tempColor.g;
		materialStruct.diffuseColor.z = tempColor.b;
	}
	if (material->Get(AI_MATKEY_COLOR_AMBIENT, tempColor) == AI_SUCCESS)
	{
		materialStruct.ambientColor.x = tempColor.r;
		materialStruct.ambientColor.y = tempColor.g;
		materialStruct.ambientColor.z = tempColor.b;
	}
	if (material->Get(AI_MATKEY_COLOR_SPECULAR, tempColor) == AI_SUCCESS)
	{
		materialStruct.specularColor.x = tempColor.r;
		materialStruct.specularColor.y = tempColor.g;
		materialStruct.specularColor.z = tempColor.b;
	}
	if (material->Get(AI_MATKEY_COLOR_EMISSIVE, tempColor) == AI_SUCCESS)
	{
		materialStruct.emissiveColor.x = tempColor.r;
		materialStruct.emissiveColor.y = tempColor.g;
		materialStruct.emissiveColor.z = tempColor.b;
	}
	if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, tempColor) == AI_SUCCESS)
	{
		materialStruct.transparentColor.x = tempColor.r;
		materialStruct.transparentColor.y = tempColor.g;
		materialStruct.transparentColor.z = tempColor.b;
	}

	float temp = 0.0f;
	if (material->Get(AI_MATKEY_OPACITY, temp) == AI_SUCCESS)			materialStruct.opacity = temp;
	if (material->Get(AI_MATKEY_SHININESS, temp) == AI_SUCCESS)			materialStruct.shininess = temp;
	if (material->Get(AI_MATKEY_SHININESS_STRENGTH, temp) == AI_SUCCESS) materialStruct.shininess_str = temp;
	if (material->Get(AI_MATKEY_REFRACTI, temp) == AI_SUCCESS)			materialStruct.refracti = temp;
}

void NLREAssimpAssetImporter::loadMaterialTextures(std::string assetPath, aiMaterial* material, NLRE_Material*& nlreMaterial)
{
	aiString path;

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->diffuseText, 
				nlreMaterial->diffuseTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->specularText, 
				nlreMaterial->specularTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
	{
		if (material->GetTexture(aiTextureType_AMBIENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->ambientText, 
				nlreMaterial->ambientTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
	{
		if (material->GetTexture(aiTextureType_EMISSIVE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->emissiveText, 
				nlreMaterial->emissiveTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
	{
		if (material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->heightmapText, 
				nlreMaterial->heightmapTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{
		if (material->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->normalText, 
				nlreMaterial->normalTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		if (material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->shininessText, 
				nlreMaterial->shininessTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_OPACITY) > 0)
	{
		if (material->GetTexture(aiTextureType_OPACITY, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->opacityText, 
				nlreMaterial->opacityTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
	{
		if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->displacementText, 
				nlreMaterial->displacementTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_LIGHTMAP) > 0)
	{
		if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->lightmapText, 
				nlreMaterial->lightmapTextView);
		}
	}

	if (material->GetTextureCount(aiTextureType_REFLECTION) > 0)
	{
		if (material->GetTexture(aiTextureType_REFLECTION, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			_textureLoader->loadTexture(
				generateTextureResourcePath(path, assetPath), 
				NLRE_BIND_SHADER_RESOURCE, 
				NLRE_USAGE_IMMUTABLE, 
				nlreMaterial->reflectionText, 
				nlreMaterial->reflectionTextView);
		}
	}
}

void NLREAssimpAssetImporter::setTextureResourcePath(std::wstring path)
{
	_textureResourcePath = path;
}

void NLREAssimpAssetImporter::enableBuiltInTexturePath(bool status)
{
	useBuiltInTexturePath = status;
}

std::wstring NLREAssimpAssetImporter::generateTextureResourcePath(aiString textureResourcePath, std::string assetResourcePath)
{

	fs::path texturePath(textureResourcePath.data);
	texturePath = texturePath.make_preferred();

	if (!useBuiltInTexturePath)
	{
		fs::path providedPath(_textureResourcePath.c_str());
		providedPath /= texturePath.filename();
		texturePath = providedPath.make_preferred();
	}

	if (texturePath.is_absolute())
	{
		return texturePath.generic_wstring();
	}
	else
	{
		fs::path assetPath(assetResourcePath.c_str());
		assetPath /= texturePath;
		assetPath = assetPath.make_preferred();
		return assetPath.generic_wstring();
	}
}