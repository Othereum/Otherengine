#pragma once

#define OE_INTERFACE_BODY(Name) \
public: \
	constexpr Name() noexcept = default; \
	virtual ~Name() = 0 {} \
	\
	Name(Name&&) = default; \
	Name(const Name&) = default; \
	Name& operator=(Name&&) = default; \
	Name& operator=(const Name&) = default
