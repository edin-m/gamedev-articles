#include "meshdata.h"

namespace k {

MeshData::MeshData(const Shader& shader)
  : shader_(shader)
{

}

MeshData::MeshData(const std::string& shaderName) {
  shader_.loadShader(shaderName);
}

}
