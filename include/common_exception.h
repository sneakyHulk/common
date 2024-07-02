#include <stdexcept>

#include "common_output.h"

namespace common {
	class Exception : public std::exception {
		std::string _exception;

	   public:
#ifdef __cpp_concepts
		explicit Exception(printable auto &&...args){
#else
		template <typename... Args>
		explicit Exception(Args &&...args) {
#endif
		    std::ostringstream ss;
		(ss << ... << args) << std::endl;

		_exception = ss.str();
	} [[nodiscard]] char const *what() const noexcept final {
		return _exception.c_str();
	}
};  // namespace common
}  // namespace common