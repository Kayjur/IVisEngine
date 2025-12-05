#include "Engine.h"
#include "EngineInfo.h"
#include "Renderer.h"
#include <sstream>

using ivis::core::EngineName;
using ivis::core::EngineProfile;

namespace ivis::engine {
std::string Boot() {
    std::ostringstream stream;
    stream << "Booting " << EngineName() << " (" << EngineProfile() << ")" << '\n';
    stream << ivis::render::RenderFrame();
    return stream.str();
}
}
