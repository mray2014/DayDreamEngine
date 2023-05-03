#pragma once

#include "DreamMesh.h"
#include "DreamShader.h"
#include <unordered_map>
#include <string_view>


class DreamAssetManager {

public:
	static DreamAssetManager* GetInstance();
	static void DestroyInstance();
	void LoadAssets();

	DreamMesh* GetMesh(std::string_view name);
	DreamShader* GetShader(std::string_view name);
	DreamShaderLinker* GetShaderLinker(std::string_view name);
	DreamTexture* GetTexture(std::string_view name);

	void StoreMesh(std::string_view name, DreamMesh* mesh);
	void StoreShader(std::string_view name, DreamShader* shader);
	void StoreShaderLinker(std::string_view name, DreamShaderLinker* linker);
	void StoreTexture(std::string_view name, DreamTexture* texture);

private:
	static DreamAssetManager* assetManager;
	DreamAssetManager();
	~DreamAssetManager();

	void LoadMeshes();

	void LoadHardCodedMeshes();
	void LoadAssimpMeshes();

	void LoadShaders();
	void LoadShaderLinkers();
	void LoadTextures();

	std::unordered_map<std::string_view, DreamMesh*> meshStorage;
	std::unordered_map<std::string_view, DreamShader*> shaderStorage;
	std::unordered_map<std::string_view, DreamShaderLinker*> shaderLinkerStorage;
	std::unordered_map<std::string_view, DreamTexture*> textureStorage;
};
