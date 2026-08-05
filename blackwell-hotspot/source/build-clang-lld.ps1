$ErrorActionPreference = 'Stop'

$sourceRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$outputRoot = Join-Path $sourceRoot 'bin'
$objectPath = Join-Path $outputRoot 'BlackwellHotspot.obj'
$dllPath = Join-Path $outputRoot 'BlackwellHotspot.dll'
$sourcePath = Join-Path $sourceRoot 'AfterburnerMonitoringPlugin.cpp'
$definitionPath = Join-Path $sourceRoot 'BlackwellHotspot.def'

New-Item -ItemType Directory -Force -Path $outputRoot | Out-Null

$clang = Get-Command clang -ErrorAction Stop
& $clang.Source -target i686-pc-windows-msvc -std=c++20 -O2 -ffreestanding `
    -fno-exceptions -fno-rtti -fno-stack-protector -fno-builtin `
    -c $sourcePath -o $objectPath
if ($LASTEXITCODE -ne 0) { throw 'clang compilation failed.' }

$linkArguments = @(
    '/dll', '/noentry', '/machine:x86', '/nodefaultlib', '/opt:ref', '/opt:icf',
    '/dynamicbase', '/nxcompat', '/timestamp:0', "/out:$dllPath",
    "/def:$definitionPath", $objectPath
)

$lldLink = Get-Command lld-link -ErrorAction SilentlyContinue
if ($lldLink)
{
    & $lldLink.Source @linkArguments
}
else
{
    $ldLld = Get-Command ld.lld -ErrorAction Stop
    & $ldLld.Source -flavor link @linkArguments
}
if ($LASTEXITCODE -ne 0) { throw 'lld-link failed.' }

Get-Item -LiteralPath $dllPath | Select-Object FullName, Length
Get-FileHash -LiteralPath $dllPath -Algorithm SHA256
