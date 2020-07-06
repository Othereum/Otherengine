#include "Platform.hpp"
#include <cstring>

namespace oeng::plf
{
	const CpuInfo& CpuInfo::Get()
	{
		static const CpuInfo info;
		return info;
	}

	CpuInfo::CpuInfo()
	{
		int cpu_id[4];
		
		CpuId(cpu_id, 0);
		const auto num_ids = cpu_id[0] + 1;
		
		CpuIdEx(cpu_id, 0, 0);
		*reinterpret_cast<int*>(vendor_) = cpu_id[1];
		*reinterpret_cast<int*>(vendor_ + 4) = cpu_id[3];
		*reinterpret_cast<int*>(vendor_ + 8) = cpu_id[2];
		
		if (std::strcmp(vendor_, "GenuineIntel") == 0) vendor_enum_ = Vendor::kIntel;
		else if (std::strcmp(vendor_, "AuthenticAMD") == 0) vendor_enum_ = Vendor::kAmd;

		if (num_ids > 1)
		{
			CpuIdEx(cpu_id, 1, 0);
			f_1_ecx_ = cpu_id[2];
			f_1_edx_ = cpu_id[3];
		}

		if (num_ids > 7)
		{
			CpuIdEx(cpu_id, 7, 0);
			f_7_ebx_ = cpu_id[1];
			f_7_ecx_ = cpu_id[2];
		}

		constexpr auto ex_fn_id = static_cast<int>(0x80000000);
		CpuId(cpu_id, ex_fn_id);
		const auto num_ex_ids = (cpu_id[0] & ~ex_fn_id) + 1;

		if (num_ex_ids > 1)
		{
			CpuIdEx(cpu_id, 1 | ex_fn_id, 0);
			f_81_ecx_ = cpu_id[2];
			f_81_edx_ = cpu_id[3];
		}

		if (num_ex_ids > 4)
		{
			CpuIdEx(cpu_id, 2 | ex_fn_id, 0);
			std::memcpy(brand_, cpu_id, 16);
			
			CpuIdEx(cpu_id, 3 | ex_fn_id, 0);
			std::memcpy(brand_ + 16, cpu_id, 16);
			
			CpuIdEx(cpu_id, 4 | ex_fn_id, 0);
			std::memcpy(brand_ + 32, cpu_id, 16);
		}
	}
}
