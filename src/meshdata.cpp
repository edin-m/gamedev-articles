#include "meshdata.h"

namespace k {

MeshData::MeshData(const std::string& shaderName) {
  shader_.loadShader(shaderName);
}

}
