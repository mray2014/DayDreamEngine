#pragma once
#include "DreamShader.h"

class DreamShaderLinker {
private:
	void UpdateBuffer(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0);
protected:
	DreamShaderLinker();
	std::vector<DreamShader*> linkedShaders;
	UniformBindingPoints bindingPoints;
public:
	virtual ~DreamShaderLinker() {}
	virtual void AttachShader(DreamShader* shader) = 0;
	virtual void Finalize() = 0;
	virtual void BindShaderLink(UniformIndexStore& indexStore) = 0;
	virtual void UnBindShaderLink() = 0;

	void GenerateUniformIndexs(UniformIndexStore& indexStore) {
		unsigned int shaderSize = linkedShaders.size();

		for (auto& shaderInfo : linkedShaders) {
			for (auto& uniformInfo : shaderInfo->shaderUniforms) {
				if (uniformInfo.first != "ConstantData") {
					indexStore[uniformInfo.first] = uniformInfo.second.AddUniformBuffer();
				}	
			}

		}
	}

	template <typename T>
	inline bool UpdateUniform(const std::string& uniformName, T& data, unsigned index) {
		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderUniforms.count(uniformName)) {
				DreamBuffer* buffer = linkedShaders[i]->shaderUniforms[uniformName].GetUniformBuffer(index);
				UpdateBuffer(buffer, &data, sizeof(T));
				return true;
			}
		}
		return false;
	}

	template <typename T>
	inline bool UpdateUniformMemberData(const std::string& uniformName, const std::string& memberName, T& data, unsigned index) {

		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderUniforms.count(uniformName)) {
				UniformMembers memberInfo = linkedShaders[i]->shaderUniforms[uniformName].uniformMembers;

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