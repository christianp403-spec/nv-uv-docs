# Building the plug-in

The release DLL is built from `AfterburnerMonitoringPlugin.cpp` as a
freestanding Win32/x86 DLL with no CRT and no imported functions.

The reproducible release build used LLVM/LLD 22.1.8. With `clang` and either
`lld-link` or `ld.lld` on `PATH`, run from PowerShell:

```powershell
.\build-clang-lld.ps1
```

The result is written to `bin\BlackwellHotspot.dll`. Inspect the PE headers and
exports before distribution. A valid build exports only:

- `GetSourcesNum`
- `GetSourceDesc`
- `GetSourceData`

The adapter source contains the complete production implementation; it does
not depend on NV-UV application code.
