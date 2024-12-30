
#include <vector>
#include <functional>

namespace idk
{
    class Packager;
}


class idk::Packager
{
private:
    std::vector<std::function<void()>> m_callbacks;

public:
    void onBuild( const std::function<void()>& );
    int  build();
};
