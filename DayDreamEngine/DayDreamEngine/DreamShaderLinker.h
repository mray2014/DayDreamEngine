#pragma once
#include "DreamShader.h"
#include "DreamTexture.h"

class DreamShaderLinker {
private:
	void UpdateBuffer(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0);
protected:
	DreamShaderLinker();
	std::vector<DreamShader*> linkedShaders;
	ResourceBindingPoints bindingPoints;
	DreamPointer* vertexInputLayout;
public:
	virtual ~DreamShaderLinker() {}
	virtual void AttachShader(DreamShader* shader) = 0;
	virtual void Finalize() = 0;
	virtual void BindShaderLink(UniformIndexStore& indexStore, std::unordered_map<std::string, DreamTexture*> texMap) = 0;
	virtual void UnBindShaderLink() = 0;
	virtual void AddNewObjectInfo(UniformIndexStore& store){}


	void GenerateUniformIndexs(UniformIndexStore& indexStore) {
		unsigned int shaderSize = linkedShaders.size();

		for (auto& shaderInfo : linkedShaders) {
			for (auto& uniformInfo : shaderInfo->shaderResources.uniforms) {
				if (uniformInfo.first != "ConstantData" && uniformInfo.first != "LightData") {
					indexStore[uniformInfo.first] = uniformInfo.second.AddUniformBuffer();
				}	
			}

		}
	}

	template <typename T>
	inline bool UpdateUniform(const std::string& uniformName, T& data, unsigned index) {
		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderResources.uniforms.count(uniformName)) {
				DreamBuffer* buffer = linkedShaders[i]->shaderResources.uniforms[uniformName].GetUniformBuffer(index);
				UpdateBuffer(buffer, &data, sizeof(T));
				return true;
			}
		}
		return false;
	}

	template <typename T>
	inline bool UpdateUniformMemberData(const std::string& uniformName, const std::string& memberName, T& data, unsigned index) {

		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderResources.uniforms.count(uniformName)) {
				UniformMembers memberInfo = linkedShaders[i]->shaderResources.uniforms[uniformName].uniformMembers;

				if (memberInfo.count(uniformName)) {
					//DreamGraphics::GetInstance()->UpdateBufferData(buffer, &data, sizeof(T));
					return true;
				}
			}
		}

		return false;
	}

	bool isMaterialRdy = false;

	
};