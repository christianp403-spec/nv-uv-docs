# NV-UV Privacy Notes

NV-UV has **no accounts, no telemetry and no automatic "phoning home"**. The app never transmits anything on its own. Data is only sent when you actively use one of the two community features introduced in v0.99. This page describes exactly what is transmitted, stored and published.

## Star ratings (UV⚡Try)

Clicking a star stores your rating locally and additionally sends an anonymous community vote to the NV-UV community backend (a Cloudflare Worker). The request contains exactly three values:

- the preset ID you rated
- your star value (1 to 5)
- a random client ID

The client ID is a random GUID generated locally on first use. It contains no hardware information, no serial numbers and no personal reference of any kind. It exists only so that one installation counts as one vote per preset (re-rating replaces your previous vote). It is stored in `%LocalAppData%\NV-UV\nv-uv_client_id.txt` and can be deleted at any time; a new one is created on your next vote. Clicking the same star again (0 stars) deletes your vote on the server as well.

Aggregated ratings (average and vote count per preset, nothing else) are published several times a day to this repository as `community/ratings.json`.

## Preset submissions (right-click on a profile slot)

Submitting a preset transmits:

- the UV values from the slot: frequency, voltage, and power limit / VRAM offset if set
- the detected GPU model name
- optionally: your board variant and a short note (free text, max 200 characters)
- the same random client ID

Submissions go into a moderation queue. **After approval, the preset including board variant and note becomes publicly visible** in `community/community_uv_db.json` in this repository and inside the app. Do not put personal information into the note. The client ID is never published.

## IP addresses

As with any internet service, the backend sees the IP address of incoming requests. It is used exclusively for abuse limiting and only as a hash that is re-salted every day. **Plaintext IP addresses are never stored.** The infrastructure provider is Cloudflare; their processing of connection data is described in Cloudflare's own privacy documentation.

## What never leaves your machine

- No usernames, no e-mail addresses, no account data (none exist)
- No hardware serial numbers or system fingerprints
- No usage statistics, crash reports or telemetry of any kind
- Nothing at all, unless you click a star or submit a preset

Displaying community ratings and refreshing the community database inside the app are pure downloads from this repository. No request to the community backend is made for that.

## Data deletion

- **Your vote:** click the same star again (0 stars) — deleted locally and on the server
- **Your client ID:** delete `nv-uv_client_id.txt` in `%LocalAppData%\NV-UV\`
- **A published preset:** open an issue in the NV-UV repository or contact the maintainer, it will be removed from the community database

Questions are welcome on the PCGH subforum, on Discord or via GitHub issues.
