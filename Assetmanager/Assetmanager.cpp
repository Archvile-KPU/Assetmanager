#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>

void ProcessNode(aiNode* node, const aiScene* scene, std::ofstream& outFile) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        // 메시 데이터 처리 및 파일에 쓰기
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D vertex = mesh->mVertices[j];
            outFile.write(reinterpret_cast<char*>(&vertex), sizeof(aiVector3D));
            // 추가적으로 노멀, 텍스처 좌표 등도 쓸 수 있습니다.
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, outFile);
    }
}

void ExportModelToBin(const aiScene* scene, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "ERROR:: Could not write to file: " << filename << std::endl;
        return;
    }

    if (scene->mRootNode) {
        ProcessNode(scene->mRootNode, scene, outFile);
    }

    outFile.close();
}

int main() {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("your_model.fbx",
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return -1;
    }

    std::cout << "Successfully loaded model" << std::endl;
    // Process the scene or model data

    if (scene) {
        ExportModelToBin(scene, "exported_model.bin");
        std::cout << "Successfully exported model" << std::endl;
    }

    return 0;
}