Blackwell Hotspot Monitor for MSI Afterburner
Version 0.4.0-beta1
==============================================

Small, experimental, read-only Win32/x86 monitoring plug-in for compatible
NVIDIA Blackwell GPUs.

Exposed monitoring sources:
  - GPUx hotspot temperature
  - GPUx hotspot delta

Tested configuration:
  - NVIDIA GeForce RTX 5090 (DEV_2B85)
  - MSI Afterburner 4.6.7.17352

The plug-in has no GPU model/device ID whitelist. It is designed to work with
other Blackwell GPUs that expose the same thermal register layout and have a
correct RTCore architecture mapping. These combinations remain experimental
until hardware-tested. RTX 5090 D (DEV_2B87) uses the same known mapping as RTX
5090 but has not been tested by the author.

IMPORTANT FOR NV-UV USERS
-------------------------
If NV-UV already manages your hotspot plug-in, do not install this package on
top of it manually. Use NV-UV's hotspot button so backups and rollback state
remain managed correctly.

MANUAL INSTALLATION
-------------------
This is an expert setup. Exit MSI Afterburner and back up RTCore.cfg,
MSIAfterburner.cfg and the target file under Profiles before continuing.

1. Copy BlackwellHotspot.dll into MSI Afterburner\Plugins\Monitoring.

2. In RTCore.cfg, under [GPU_10DE], add:

       G1B2 = 2B85h,2B87h

   If G1B2 already exists, merge the missing ID into the existing entry. Do
   not create a duplicate key or remove existing IDs.

   This mapping covers RTX 5090/5090 D only. Other Blackwell models need their
   own confirmed RTCore architecture mapping; the plug-in DLL itself does not
   need a model-specific build.

3. In the target NVIDIA GPU profile (DEV_2B85/DEV_2B87 for RTX 5090/5090 D),
   set:

       [Settings]
       LowLevelMonitoring=0

4. Start MSI Afterburner. Under Settings > Monitoring, enable the
   BlackwellHotspot.dll plug-in and these two sources:

       GPUx hotspot temperature
       GPUx hotspot delta

5. Restart MSI Afterburner and verify that both values are plausible.

If the sources are missing or return no value, stop and restore the backups.
Do not try random RTCore mappings on unconfirmed GPUs.

WHAT IT DOES
------------
The plug-in reads four GPU die temperature channels and NVIDIA's hardware MAX
register. It requires the register valid flag, fails closed on invalid RTCore
values, and keeps hotspot and delta on the same Afterburner polling snapshot.

It is completely read-only. It does not change registers, clocks, voltage,
power limits or fans.

It does NOT expose GDDR7 memory temperatures.

UNINSTALLATION
--------------
Exit MSI Afterburner, remove BlackwellHotspot.dll and reverse the configuration
changes above using your backups.

INTEGRITY
---------
BlackwellHotspot.dll
Size:    6144 bytes
SHA-256: 3840c4a6364c6bd7682d76351da118ed8a89b3a55c7637fdfb1567e30e01093d

Project documentation and source:
https://github.com/christianp403-spec/nv-uv-docs/tree/main/blackwell-hotspot

Thanks to the Overclock.net community members who researched and verified the
Blackwell temperature registers.

Released under the MIT License. Experimental diagnostic software; use at your
own risk. Sensor readings are not official NVIDIA telemetry and should not be
used as sole evidence for warranty or RMA decisions.
