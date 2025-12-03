#pragma once
namespace TDEngine {
    namespace Inner {
        class IPlayerAction {
            public:
            virtual ~IPlayerAction() = default;
            virtual void MakeAction() = 0;
        };
    } // Inner
} // TDEngine