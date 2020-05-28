#include "Assert.hpp"
#include <exception>
#include <iostream>

namespace oeng::detail
{
#if OE_BUILD_MODE == 0 || OE_BUILD_MODE == 1

	class AssertionFailed final : public std::exception
	{
	public:
		explicit AssertionFailed(const char* msg) noexcept :msg_{msg} {}
		[[nodiscard]] const char* what() const noexcept override { return msg_; }

	private:
		const char* msg_;
	};

	void OnAssertionFailed(const char* msg)
	{
		throw AssertionFailed{msg};
	}
	
#endif

	void OnEnsureFailed(const char* msg)
	{
		std::cerr << "[WARNING] " << msg << std::endl;
	}
}
