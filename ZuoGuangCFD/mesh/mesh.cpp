#include "ZuoGuangCFD/mesh/mesh.h"

namespace zgCFD
{
    Mesh::Mesh() {}

    bool Mesh::isSuccessfulRead()
    {
        return success;
    }
}  // namespace zgCFD
