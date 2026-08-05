// Blackwell Hotspot Monitor for MSI Afterburner - Win32/x86, no CRT, read-only.
// SPDX-License-Identifier: MIT
//
// The plug-in is intentionally model-agnostic. It publishes sources only for
// Afterburner GPU indices whose four die channels and hardware MAX register
// form a complete, valid thermal sample.

using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using S32 = int;
using F32 = float;
using Ptr = void*;
using CStr = const char*;

extern "C" int _fltused = 0;

static_assert(sizeof(U32) == 4, "U32 must be 32-bit");
static_assert(sizeof(Ptr) == 4, "This plugin must be built as Win32/x86");

constexpr U32 kMaxPath = 260u;
constexpr U32 kDescriptorVersion10 = 0x00010000u;
constexpr U32 kPluginGpuSourceId = 0x000000F0u;
constexpr F32 kInvalidFloat = 3.402823466e+38F;
constexpr U32 kFirstChannelRegister = 0x00AD0A90u;
constexpr U32 kChannelCount = 4u;
constexpr U32 kRegisterStride = 4u;
constexpr U32 kHardwareHotspotRegister = 0x00AD0AA0u;
constexpr U32 kHardwareAverageRegister = 0x00AD0AA4u;
constexpr U32 kThermalValidBit = 0x40000000u;
constexpr U32 kMinRaw = 1u * 256u;
constexpr U32 kMaxRaw = 150u * 256u;
constexpr U32 kMaxGpuCount = 32u;

struct MonitoringSourceDesc
{
    U32 dwVersion;
    char szName[kMaxPath];
    char szUnits[kMaxPath];
    char szFormat[kMaxPath];
    char szGroup[kMaxPath];
    U32 dwID;
    U32 dwInstance;
    F32 fltMaxLimit;
    F32 fltMinLimit;
    char szNameTemplate[kMaxPath];
    char szGroupTemplate[kMaxPath];
    char szEnum[kMaxPath];
    char szAlarm[kMaxPath];
};
static_assert(sizeof(MonitoringSourceDesc) == 2100, "MSI Afterburner descriptor ABI mismatch");

using GetGpuNumProc = U32 (__cdecl*)();
using GetTimestampProc = U32 (__cdecl*)();
using ReadRegisterUlongProc = U32 (__cdecl*)(U32 gpu, U32 address);

struct ThermalSample
{
    U32 minimumRaw;
    U32 hotspotRaw;
    U32 averageRaw;
    U32 averageValid;
};

struct CachedThermalSample
{
    U32 timestamp;
    U32 present;
    U32 valid;
    ThermalSample sample;
};

static GetGpuNumProc g_getGpuNum = nullptr;
static GetTimestampProc g_getTimestamp = nullptr;
static ReadRegisterUlongProc g_readRegisterUlong = nullptr;
static U32 g_initialized = 0u;
static U32 g_gpuCount = 0u;
static U32 g_validGpuCount = 0u;
static U32 g_validGpuIndices[kMaxGpuCount] = {};
static CachedThermalSample g_sampleCache[kMaxGpuCount] = {};

static U32 StrLen(CStr s)
{
    U32 n = 0u;
    if (!s) return 0u;
    while (s[n]) ++n;
    return n;
}

static bool StrEq(CStr a, CStr b)
{
    if (!a || !b) return false;
    U32 i = 0u;
    while (a[i] && b[i])
    {
        if (a[i] != b[i]) return false;
        ++i;
    }
    return a[i] == b[i];
}

static void Zero(void* p, U32 n)
{
    U8* out = static_cast<U8*>(p);
    for (U32 i = 0u; i < n; ++i) out[i] = 0u;
}

static void Copy(char* dst, U32 cap, CStr src)
{
    if (!dst || cap == 0u) return;
    U32 i = 0u;
    if (src)
    {
        while (i + 1u < cap && src[i])
        {
            dst[i] = src[i];
            ++i;
        }
    }
    dst[i] = 0;
}

static void Append(char* dst, U32 cap, CStr src)
{
    U32 len = StrLen(dst);
    if (len < cap) Copy(dst + len, cap - len, src);
}

static void AppendUnsigned(char* dst, U32 cap, U32 value)
{
    char tmp[11] = {};
    U32 count = 0u;
    do
    {
        tmp[count++] = static_cast<char>('0' + (value % 10u));
        value /= 10u;
    } while (value && count < 10u);

    U32 len = StrLen(dst);
    while (count && len + 1u < cap) dst[len++] = tmp[--count];
    dst[len] = 0;
}

static void BuildGpuName(char* dst, U32 cap, U32 gpuIndex, CStr suffix)
{
    Copy(dst, cap, "GPU");
    AppendUnsigned(dst, cap, gpuIndex + 1u);
    Append(dst, cap, suffix);
}

static void* HostImageBase()
{
    void* peb = nullptr;
#if defined(_MSC_VER) && !defined(__clang__)
    __asm
    {
        mov eax, fs:[0x30]
        mov peb, eax
    }
#elif defined(__i386__)
    __asm__ __volatile__("movl %%fs:0x30, %0" : "=r"(peb));
#endif
    if (!peb) return nullptr;
    return *reinterpret_cast<void**>(reinterpret_cast<U8*>(peb) + 0x08u);
}

static void* ResolveHostExport(CStr wanted)
{
    U8* base = static_cast<U8*>(HostImageBase());
    if (!base || !wanted) return nullptr;
    if (*reinterpret_cast<U16*>(base) != 0x5A4Du) return nullptr;

    const U32 peOffset = *reinterpret_cast<U32*>(base + 0x3Cu);
    U8* nt = base + peOffset;
    if (*reinterpret_cast<U32*>(nt) != 0x00004550u) return nullptr;

    // PE32 optional header starts at nt+24; export data-directory RVA is +96.
    const U32 exportRva = *reinterpret_cast<U32*>(nt + 24u + 96u);
    if (!exportRva) return nullptr;

    U8* exp = base + exportRva;
    const U32 numberOfNames = *reinterpret_cast<U32*>(exp + 0x18u);
    const U32 functionsRva = *reinterpret_cast<U32*>(exp + 0x1Cu);
    const U32 namesRva = *reinterpret_cast<U32*>(exp + 0x20u);
    const U32 ordinalsRva = *reinterpret_cast<U32*>(exp + 0x24u);
    if (!numberOfNames || !functionsRva || !namesRva || !ordinalsRva) return nullptr;

    U32* functions = reinterpret_cast<U32*>(base + functionsRva);
    U32* names = reinterpret_cast<U32*>(base + namesRva);
    U16* ordinals = reinterpret_cast<U16*>(base + ordinalsRva);

    for (U32 i = 0u; i < numberOfNames; ++i)
    {
        CStr name = reinterpret_cast<CStr>(base + names[i]);
        if (StrEq(name, wanted))
        {
            const U32 functionRva = functions[ordinals[i]];
            return functionRva ? base + functionRva : nullptr;
        }
    }
    return nullptr;
}

static bool DecodeValidThermalRaw(U32 value, U32& raw)
{
    raw = value & 0xFFFFu;
    return (value & kThermalValidBit) != 0u && raw >= kMinRaw && raw < kMaxRaw;
}

static bool CaptureThermalSampleRaw(U32 gpuIndex, ThermalSample& sample)
{
    if (!g_readRegisterUlong || gpuIndex >= g_gpuCount) return false;

    sample.minimumRaw = 0xFFFFFFFFu;
    sample.hotspotRaw = 0u;
    sample.averageRaw = 0u;
    sample.averageValid = 0u;

    for (U32 channel = 0u; channel < kChannelCount; ++channel)
    {
        const U32 value = g_readRegisterUlong(
            gpuIndex,
            kFirstChannelRegister + channel * kRegisterStride);
        U32 raw = 0u;
        if (!DecodeValidThermalRaw(value, raw)) return false;
        if (raw < sample.minimumRaw) sample.minimumRaw = raw;
    }

    const U32 hotspotValue = g_readRegisterUlong(gpuIndex, kHardwareHotspotRegister);
    if (!DecodeValidThermalRaw(hotspotValue, sample.hotspotRaw)) return false;
    if (sample.minimumRaw == 0xFFFFFFFFu || sample.hotspotRaw < sample.minimumRaw) return false;

    const U32 averageValue = g_readRegisterUlong(gpuIndex, kHardwareAverageRegister);
    sample.averageValid = DecodeValidThermalRaw(averageValue, sample.averageRaw) ? 1u : 0u;
    return true;
}

static bool Initialize()
{
    if (g_initialized) return g_getGpuNum && g_readRegisterUlong;
    g_initialized = 1u;

    g_getGpuNum = reinterpret_cast<GetGpuNumProc>(ResolveHostExport("GetGpuNum"));
    g_getTimestamp = reinterpret_cast<GetTimestampProc>(ResolveHostExport("GetTimestamp"));
    g_readRegisterUlong = reinterpret_cast<ReadRegisterUlongProc>(ResolveHostExport("ReadRegisterUlong"));
    if (!g_getGpuNum || !g_readRegisterUlong)
    {
        g_gpuCount = 0u;
        g_validGpuCount = 0u;
        return false;
    }

    g_gpuCount = g_getGpuNum();
    if (g_gpuCount > kMaxGpuCount) g_gpuCount = kMaxGpuCount;

    g_validGpuCount = 0u;
    for (U32 gpuIndex = 0u; gpuIndex < g_gpuCount; ++gpuIndex)
    {
        ThermalSample sample{};
        if (CaptureThermalSampleRaw(gpuIndex, sample))
            g_validGpuIndices[g_validGpuCount++] = gpuIndex;
        g_sampleCache[gpuIndex].present = 0u;
    }

    return true;
}

static bool ReadThermalSnapshot(U32 gpuIndex, ThermalSample& sample)
{
    if (!Initialize() || gpuIndex >= g_gpuCount) return false;

    const U32 timestamp = g_getTimestamp ? g_getTimestamp() : 0u;
    CachedThermalSample& cached = g_sampleCache[gpuIndex];
    if (timestamp != 0u && cached.present && cached.timestamp == timestamp)
    {
        sample = cached.sample;
        return cached.valid != 0u;
    }

    ThermalSample captured{};
    const bool valid = CaptureThermalSampleRaw(gpuIndex, captured);
    if (timestamp != 0u)
    {
        cached.timestamp = timestamp;
        cached.present = 1u;
        cached.valid = valid ? 1u : 0u;
        cached.sample = captured;
    }

    sample = captured;
    return valid;
}

extern "C" __declspec(dllexport) U32 __cdecl GetSourcesNum()
{
    return Initialize() ? g_validGpuCount * 2u : 0u;
}

extern "C" __declspec(dllexport) S32 __cdecl GetSourceDesc(U32 sourceIndex, MonitoringSourceDesc* desc)
{
    if (!desc || !Initialize() || sourceIndex >= g_validGpuCount * 2u) return 0;
    const U32 version = desc->dwVersion;
    if (version < kDescriptorVersion10) return 0;

    Zero(desc, static_cast<U32>(sizeof(MonitoringSourceDesc)));
    desc->dwVersion = version;

    const U32 gpuIndex = g_validGpuIndices[sourceIndex / 2u];
    const bool delta = (sourceIndex & 1u) != 0u;
    BuildGpuName(desc->szName, kMaxPath, gpuIndex, delta ? " hotspot delta" : " hotspot temperature");
    BuildGpuName(desc->szGroup, kMaxPath, gpuIndex, "");
    Copy(desc->szNameTemplate, kMaxPath, delta ? "GPU%d hotspot delta" : "GPU%d hotspot temperature");
    Copy(desc->szGroupTemplate, kMaxPath, "GPU%d");
    desc->szUnits[0] = static_cast<char>(0xB0u);
    desc->szUnits[1] = 'C';
    desc->szUnits[2] = 0;
    Copy(desc->szFormat, kMaxPath, "%.1f");
    desc->dwID = kPluginGpuSourceId;
    desc->dwInstance = gpuIndex;
    desc->fltMaxLimit = delta ? 50.0f : 120.0f;
    desc->fltMinLimit = 0.0f;
    return 1;
}

extern "C" __declspec(dllexport) F32 __cdecl GetSourceData(U32 sourceIndex)
{
    if (!Initialize() || sourceIndex >= g_validGpuCount * 2u) return kInvalidFloat;
    const U32 gpuIndex = g_validGpuIndices[sourceIndex / 2u];
    const bool delta = (sourceIndex & 1u) != 0u;
    ThermalSample sample{};
    if (!ReadThermalSnapshot(gpuIndex, sample)) return kInvalidFloat;
    const U32 raw = delta
        ? (sample.hotspotRaw - sample.minimumRaw)
        : sample.hotspotRaw;
    return static_cast<F32>(raw) / 256.0f;
}
