#include "Renderer.h"
#include "../../Core/Public/EngineInfo.h"
#include <sstream>

using ivis::core::EngineName;
using ivis::core::EngineProfile;

namespace ivis::render {
std::string RenderFrame() {
    std::ostringstream stream;
    stream << "Renderer booted for " << EngineName() << " (" << EngineProfile() << ")";
    return stream.str();
}
}
