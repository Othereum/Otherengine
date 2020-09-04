#include "Platform.hpp"

namespace oeng
{
inline namespace core
{
#ifndef OE_BUILD_SHIPPING
fs::path GetUserDataPath()
{
    return u8"../Saved"sv;
}
#endif

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

    char vendor[12];
    CpuIdCnt(cpu_id, 0, 0);
    *reinterpret_cast<int*>(vendor) = cpu_id[1];
    *reinterpret_cast<int*>(vendor + 4) = cpu_id[3];
    *reinterpret_cast<int*>(vendor + 8) = cpu_id[2];

    if (std::equal(vendor, vendor + 12, "GenuineIntel"))
    {
        vendor_enum_ = Vendor::kIntel;
    }
    else if (std::equal(vendor, vendor + 12, "AuthenticAMD"))
    {
        vendor_enum_ = Vendor::kAmd;
    }

    std::copy(vendor, vendor + 12, vendor_);

    if (num_ids > 1)
    {
        CpuIdCnt(cpu_id, 1, 0);
        f_1_ecx_ = cpu_id[2];
        f_1_edx_ = cpu_id[3];
    }

    if (num_ids > 7)
    {
        CpuIdCnt(cpu_id, 7, 0);
        f_7_ebx_ = cpu_id[1];
        f_7_ecx_ = cpu_id[2];
    }

    constexpr auto ex_fn_id = static_cast<int>(0x80000000);
    CpuId(cpu_id, ex_fn_id);
    const auto num_ex_ids = (cpu_id[0] & ~ex_fn_id) + 1;

    if (num_ex_ids > 1)
    {
        CpuIdCnt(cpu_id, 1 | ex_fn_id, 0);
        f_81_ecx_ = cpu_id[2];
        f_81_edx_ = cpu_id[3];
    }

    if (num_ex_ids > 4)
    {
        char brand[48];
        const auto as_int = reinterpret_cast<int*>(brand);

        CpuIdCnt(as_int, 2 | ex_fn_id, 0);
        CpuIdCnt(as_int + 4, 3 | ex_fn_id, 0);
        CpuIdCnt(as_int + 8, 4 | ex_fn_id, 0);

        std::copy(brand, brand + 48, brand_);
    }
}
}
}
