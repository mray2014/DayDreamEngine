#include "DreamAssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DreamGraphics.h"

DreamAssetManager* DreamAssetManager::assetManager = nullptr;

DreamAssetManager* DreamAssetManager::GetInstance()
{
	if (!assetManager) {
		assetManager = new DreamAssetManager();
	}
	return assetManager;
}

void DreamAssetManager::DestroyInstance()
{
	if (assetManager) {
		delete assetManager;
		assetManager = nullptr;
	}
}

DreamAssetManager::DreamAssetManager()
{
}

DreamAssetManager::~DreamAssetManager()
{
	//TODO: delete all the assets!!
}

void DreamAssetManager::LoadAssets()
{
	LoadMeshes();
	LoadShaders();
	LoadShaderLinkers();
	LoadTextures();
}

DreamMesh* DreamAssetManager::GetMesh(std::string_view name)
{
	DreamMesh* returnMesh = nullptr;

	if (meshStorage.contains(name)) {
		returnMesh = meshStorage[name];
	}
	else {
		printf("COULDN'T FIND MESH ASSET '%s'", std::string(name).c_str());
	}

	return returnMesh;
}

DreamShader* DreamAssetManager::GetShader(std::string_view name)
{
	DreamShader* returnShader = nullptr;

	if (shaderStorage.contains(name)) {
		returnShader = shaderStorage[name];
	}
	else {
		printf("COULDN'T FIND SHADER ASSET '%s'", std::string(name).c_str());
	}

	return returnShader;
}

DreamShaderLinker* DreamAssetManager::GetShaderLinker(std::string_view name)
{
	DreamShaderLinker* returnShaderLinker = nullptr;

	if (shaderLinkerStorage.contains(name)) {
		returnShaderLinker = shaderLinkerStorage[name];
	}
	else {
		printf("COULDN'T FIND SHADER LINKER ASSET '%s'", std::string(name).c_str());
	}

	return returnShaderLinker;
}

DreamTexture* DreamAssetManager::GetTexture(std::string_view name)
{
	DreamTexture* returnTexture = nullptr;

	if (textureStorage.contains(name)) {
		returnTexture = textureStorage[name];
	}
	else {
		printf("COULDN'T FIND TEXTURE ASSET '%s'", std::string(name).c_str());
	}

	return returnTexture;
}

void DreamAssetManager::StoreMesh(std::string_view name, DreamMesh* mesh)
{
	if (mesh) {
		meshStorage[name] = mesh;
	}
	else {
		printf("NO MESH CREATED TO STORE FOR: '%s'", std::string(name).c_str());
	}
}

void DreamAssetManager::StoreShader(std::string_view name, DreamShader* shader)
{
	if (shader) {
		shaderStorage[name] = shader;
	}
	else {
		printf("NO SHADER CREATED TO STORE FOR: '%s'", std::string(name).c_str());
	}
}

void DreamAssetManager::StoreShaderLinker(std::string_view name, DreamShaderLinker* linker)
{
	if (linker) {
		shaderLinkerStorage[name] = linker;
	}
	else {
		printf("NO SHADER LINKER CREATED TO STORE FOR: '%s'", std::string(name).c_str());
	}
}

void DreamAssetManager::StoreTexture(std::string_view name, DreamTexture* texture)
{
	if (texture) {
		textureStorage[name] = texture;
	}
	else {
		printf("NO TEXTURE CREATED TO STORE FOR: '%s'", std::string(name).c_str());
	}
}

void DreamAssetManager::LoadMeshes()
{
	LoadHardCodedMeshes();
	LoadAssimpMeshes();
}

void DreamAssetManager::LoadHardCodedMeshes()
{
	std::vector<uint32_t> indices = std::vector<uint32_t>();

	std::vector<DreamVertex> vert = std::vector<DreamVertex>();
	vert.push_back(DreamVertex(-0.5, -0.5, 0.0f));
	vert.push_back(DreamVertex(0.0, 0.5, 0.0f));
	vert.push_back(DreamVertex(0.5, -0.5, 0.0f));
	vert.push_back(DreamVertex(1.0, 0.5, 0.0f));

	std::vector<DreamVertex> vert2 = std::vector<DreamVertex>();
	vert2.push_back(DreamVertex(-0.5, -0.5, 0.0f));
	vert2.push_back(DreamVertex(0.0, 2.0f, 0.0f));
	vert2.push_back(DreamVertex(0.5, -0.5, 0.0f));
	vert2.push_back(DreamVertex(1.0, 0.5, 0.0f));

	std::vector<DreamVertex> vert3_Index = std::vector<DreamVertex>();
	vert3_Index.push_back(DreamVertex(-1, -1, 0.0f));
	vert3_Index.push_back(DreamVertex(-1, 1, 0.0f));
	vert3_Index.push_back(DreamVertex(1, -1, 0.0f));
	vert3_Index.push_back(DreamVertex(1, 1, 0.0f));

	vert3_Index[0].uv = DreamVector2(0, 0);
	vert3_Index[1].uv = DreamVector2(0, 1);
	vert3_Index[2].uv = DreamVector2(1, 0);
	vert3_Index[3].uv = DreamVector2(1, 1);

	std::vector<DreamVertex> vert3_Vert = std::vector<DreamVertex>();
	vert3_Vert.push_back(DreamVertex(-0.5, -1, 0.0f));
	vert3_Vert.push_back(DreamVertex(-0.5, 1, 0.0f));
	vert3_Vert.push_back(DreamVertex(0.5, -1, 0.0f));
	vert3_Vert.push_back(DreamVertex(0.5, -1, 0.0f));
	vert3_Vert.push_back(DreamVertex(-0.5, 1, 0.0f));
	vert3_Vert.push_back(DreamVertex(0.5, 1, 0.0f));

	vert3_Vert[0].uv = DreamVector2(0, 0);
	vert3_Vert[1].uv = DreamVector2(0, 1);
	vert3_Vert[2].uv = DreamVector2(1, 0);
	vert3_Vert[3].uv = DreamVector2(1, 0);
	vert3_Vert[4].uv = DreamVector2(0, 1);
	vert3_Vert[5].uv = DreamVector2(1, 1);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	StoreMesh("triangleMesh", new DreamMesh(vert2));
	StoreMesh("parallogramMesh", new DreamMesh(vert, indices));
	StoreMesh("squareMesh", new DreamMesh(vert3_Index, indices));
}

void DreamAssetManager::LoadAssimpMeshes()
{
	AssimpLoadMeshes("cube", "Models/cube.obj");
	AssimpLoadMeshes("quad", "Models/quad.obj");
	AssimpLoadMeshes("sphere", "Models/sphere.obj");
	AssimpLoadMeshes("teapot", "Models/teapot.obj");
	AssimpLoadMeshes("raygun", "Models/raygun.obj");
	AssimpLoadMeshes("rainbow_road", "Models/RainbowRoad.obj");
}

void DreamAssetManager::AssimpLoadMeshes(std::string_view name, std::string_view fileName)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<DreamVertex> verts;           // Verts we're assembling
	std::vector<uint32_t> indices;           // Indices of these verts

	unsigned int indCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertsPerMesh = 0;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[j]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[j]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

			DreamVertex v;
			v.pos = DreamVector3(pPos->x, pPos->y, pPos->z);
			v.uv = DreamVector2(pTexCoord->x, pTexCoord->y);
			v.normal = DreamVector3(pNormal->x, pNormal->y, pNormal->z);

			verts.push_back(v);
			vertCount++;
		}
		for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {
			const aiFace& Face = paiMesh->mFaces[j];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[2] + vertsPerMesh);
			indices.push_back(Face.mIndices[1] + vertsPerMesh);
			indices.push_back(Face.mIndices[0] + vertsPerMesh);

			indCount += 3;
		}
		vertsPerMesh = vertCount;
	}
	StoreMesh(name, new DreamMesh(verts, indices));
	importer.FreeScene();
}

void DreamAssetManager::LoadShaders()
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();

	StoreShader("default_VShader", graphics->LoadShader(L"vertex", ShaderType::VertexShader));
	StoreShader("default_PShader", graphics->LoadShader(L"pixel", ShaderType::PixelShader));
	StoreShader("texture_VShader", graphics->LoadShader(L"Texture_vertex", ShaderType::VertexShader));
	StoreShader("texture_PShader", graphics->LoadShader(L"Texture_pixel", ShaderType::PixelShader));
}

void DreamAssetManager::LoadShaderLinkers()
{
	DreamShaderLinker* defaultLinker = DreamGraphics::GenerateShaderLinker();
	defaultLinker->AttachShader(shaderStorage["default_VShader"]);
	defaultLinker->AttachShader(shaderStorage["default_PShader"]);
	defaultLinker->Finalize();

	DreamShaderLinker* textureLinker = DreamGraphics::GenerateShaderLinker();
	textureLinker->AttachShader(shaderStorage["texture_VShader"]);
	textureLinker->AttachShader(shaderStorage["texture_PShader"]);
	textureLinker->Finalize();

	StoreShaderLinker("defaultShaderLinker", defaultLinker);
	StoreShaderLinker("textureShaderLinker", textureLinker);
}

void DreamAssetManager::LoadTextures()
{
	StoreTexture("HUH", new DreamTexture("Textures/HUH.jpg"));
	StoreTexture("FrenchFries", new DreamTexture("Textures/FrenchFries.jpg"));
	//StoreTexture("Sun_SkyBox", new DreamTexture("Textures/SkyBox/SunnyCubeMap.dds", TextureType::Texture3D));
}
