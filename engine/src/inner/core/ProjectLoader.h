#pragma once
#include <memory>

#include "../Zaglushki/ProjectSample.h"

namespace TDEngine {
    namespace Inner {
        class ProjectLoader {
            public:
            ProjectLoader();
            std::shared_ptr<ProjectSample> load();
        };
    } // Inner
} // TDEngine