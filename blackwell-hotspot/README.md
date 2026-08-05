# Blackwell Hotspot Monitor for MSI Afterburner

Experimental beta, version **0.4.0-beta1**.

This is a small, read-only Win32/x86 monitoring plug-in for MSI Afterburner. It
adds two monitoring sources for compatible NVIDIA Blackwell GPUs:

- `GPUx hotspot temperature`
- `GPUx hotspot delta`

The current build has been tested on an NVIDIA GeForce RTX 5090 (`DEV_2B85`)
with MSI Afterburner 4.6.7.17352. The plug-in itself has no GPU model or device
ID whitelist: it scans Afterburner's GPU indices and exposes sources only when
the required registers return a complete, valid thermal sample. It is therefore
designed to work with other Blackwell GPUs that expose the same register layout
and have the correct RTCore architecture mapping. Those combinations are
experimental until independently hardware-tested. RTX 5090 D (`DEV_2B87`) uses
the same known RTCore mapping as RTX 5090 but has not been tested by the author.

[Download BlackwellHotspot-0.4.0-beta1.zip](downloads/BlackwellHotspot-0.4.0-beta1.zip)

After the branch is merged, the same file is available through GitHub Pages:

`https://christianp403-spec.github.io/nv-uv-docs/blackwell-hotspot/downloads/BlackwellHotspot-0.4.0-beta1.zip`

## What it does

The plug-in reads four GPU die thermal channels (`0xAD0A90` through
`0xAD0A9C`) and NVIDIA's hardware MAX register (`0xAD0AA0`). The published
hotspot is the hardware MAX value. The delta is calculated as:

`hardware MAX - coldest of the four die channels`

For robustness, the plug-in:

- requires the hardware-valid flag (bit 30) on every required register;
- rejects invalid RTCore values before temperature decoding;
- requires all four die channels and hardware MAX to be valid;
- rejects a hardware MAX value below the coldest die channel;
- caches one register snapshot per MSI Afterburner timestamp, so hotspot and
  delta cannot come from different polling cycles;
- treats the optional hardware AVG register (`0xAD0AA4`) independently, so an
  unavailable AVG value does not disable hotspot or delta.

It does **not** write to GPU registers and does not change voltage, clocks,
power limits or fan settings.

It also does **not** expose per-chip or aggregate GDDR7 memory temperatures.

## Important NV-UV note

If the hotspot feature is already managed by NV-UV, do not install this ZIP on
top of it manually. Use NV-UV's hotspot button so its installer can preserve
configuration backups, upgrades and rollback state. This standalone package is
intended for users who want to configure MSI Afterburner themselves.

## Manual installation

This is an expert/experimental setup. Make backups before changing anything.

1. Exit MSI Afterburner completely.
2. Back up these files from your MSI Afterburner installation:
   - `RTCore.cfg`
   - `MSIAfterburner.cfg`
   - the target GPU file under `Profiles`
3. Copy `BlackwellHotspot.dll` to `Plugins\Monitoring` inside the MSI
   Afterburner installation directory.
4. Open `RTCore.cfg`. Under `[GPU_10DE]`, add the mapping for RTX 5090/5090 D:

   ```ini
   G1B2 = 2B85h,2B87h
   ```

   If a `G1B2` entry already exists, merge the missing device ID into that
   entry. Do not create a duplicate key and do not remove existing IDs.
   This particular mapping covers RTX 5090/5090 D only. Another Blackwell GPU
   needs its own confirmed RTCore architecture mapping; the plug-in binary does
   not otherwise need a model-specific build.
5. In the target NVIDIA GPU profile under `Profiles`, set:

   ```ini
   [Settings]
   LowLevelMonitoring=0
   ```

   Select the profile containing the target GPU's NVIDIA device ID. For RTX
   5090/5090 D that is `DEV_2B85`/`DEV_2B87`. Do not change an unrelated
   adapter profile.
6. Start MSI Afterburner. In **Settings > Monitoring**, enable the
   `BlackwellHotspot.dll` monitoring plug-in and enable both new monitoring
   sources.
7. Restart MSI Afterburner once more and verify that both values are plausible.

If the sources do not appear or return no value, stop and restore the backups.
Do not try random RTCore mappings on unconfirmed GPUs.

## Uninstallation

Exit MSI Afterburner, remove `BlackwellHotspot.dll`, and restore the settings
you changed from your backups. If you made other Afterburner changes after the
backup, reverse only the hotspot-related entries instead of overwriting the
newer configuration wholesale.

## Integrity

The published hashes are in [SHA256SUMS.txt](SHA256SUMS.txt). The beta DLL is a
6,144-byte PE32/i386 binary with no imported functions. Its SHA-256 is:

`3840c4a6364c6bd7682d76351da118ed8a89b3a55c7637fdfb1567e30e01093d`

## Source and license

The production plug-in source is available in [source](source/). It builds as
a small no-CRT x86 DLL and exports only the three MSI Afterburner monitoring
entry points.

The source and binary are released under the [MIT License](LICENSE.txt).

## Credits

Thanks to the Overclock.net community members who researched and independently
verified the Blackwell temperature registers. The relevant discussion starts
around [this RTX 5090 Owners Club post](https://www.overclock.net/threads/official-nvidia-rtx-5090-owners-club.1814246/page-1904?post_id=29600045#post-29600045).

The plug-in implementation published here is original NV-UV project code. No
third-party plug-in source is included.

## Disclaimer

Experimental diagnostic software, provided without warranty. Sensor readings
are not official NVIDIA telemetry and should not be treated as sole evidence
for warranty or RMA decisions. Use at your own risk.
