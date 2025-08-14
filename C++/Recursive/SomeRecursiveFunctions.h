
#include <cstdarg>
#include <cstddef>
#include <vector>
namespace recursive {
int sum(int, ...);
int count(int, ...);
int max(int, ...);
int min(int, ...);

int sum(const std::vector<int> &, std::size_t = 0);
int count(const std::vector<int> &, std::size_t = 0);
int max(const std::vector<int> &, std::size_t = 0);
int min(const std::vector<int> &, std::size_t = 0);

} // namespace recursive
