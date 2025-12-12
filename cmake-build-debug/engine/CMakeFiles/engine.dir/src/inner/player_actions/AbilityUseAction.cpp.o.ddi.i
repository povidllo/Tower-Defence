# 0 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/AbilityUseAction.cpp"
# 1 "/home/dimandr/Other/F/Programming/Tower-Defence/cmake-build-debug//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/AbilityUseAction.cpp"
# 1 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/AbilityUseAction.h" 1
       
# 1 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/IPlayerAction.h" 1
       
namespace TDEngine {
    namespace Inner {
        class IPlayerAction {
            public:
            virtual ~IPlayerAction() = default;
            virtual void MakeAction() = 0;
        };
    }
}
# 3 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/AbilityUseAction.h" 2

namespace TDEngine {
    namespace Inner {
        class AbilityUseAction : public IPlayerAction{
        };
    }
}
# 2 "/home/dimandr/Other/F/Programming/Tower-Defence/engine/src/inner/player_actions/AbilityUseAction.cpp" 2

namespace TDEngine {
    namespace Inner {
    }
}
