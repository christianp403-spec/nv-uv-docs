# NV-UV Documentation

Public documentation site for **NV-UV**, a GPU undervolting companion tool for NVIDIA RTX 50-series (Blackwell), RTX 40-series and RTX 30-series.

👉 **https://christianp403-spec.github.io/nv-uv-docs/**

## About NV-UV

NV-UV is a free, closed-source companion app for MSI Afterburner that simplifies GPU undervolting on the NVIDIA RTX 50-series (Blackwell) and RTX 40-series (Ada Lovelace, experimental). It is currently in **Open Alpha**, hosted at the [PCGH NV-UV subforum](https://extreme.pcgameshardware.de/forums/nv-uv.3601/) with an active Discord community.

NV-UV does **not** replace MSI Afterburner — it integrates with it as a write backend and adds preset switching, scanner-based optimization, crash recovery, and a streamlined UI for the undervolting workflow.

## Updating the Guides

The HTML files in this repo are the rendered tester guides. To update:

1. Edit the `.html` files directly (or push from your local copy)
2. GitHub Pages picks up the change within ~30 seconds
3. Hard-reload in your browser (Ctrl+F5) to bypass cache

The `.nojekyll` file disables GitHub's Jekyll build — files are served as plain HTML.
