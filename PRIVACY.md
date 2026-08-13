# NV-UV Datenschutz / Privacy

_Stand / Last updated: 13. August 2026 / 13 August 2026_

## Deutsch

### Verantwortlicher und Kontakt

Verantwortlich für die hier beschriebene Verarbeitung ist Christian Papaioannou, Betreiber des NV-UV-Projekts ([christianp403-spec](https://github.com/christianp403-spec)). Datenschutz-, Widerrufs- und Löschanfragen können per E-Mail an [christian.papaioannou@protonmail.com](mailto:christian.papaioannou@protonmail.com) oder per privater Nachricht über das [PCGH NV-UV-Subforum](https://extreme.pcgameshardware.de/forums/nv-uv.3601/) beziehungsweise Discord gestellt werden. Für die Zuordnung eines privaten Berichts dessen Bericht-ID nur in der privaten Nachricht angeben und nicht öffentlich posten. Eine Kontaktangabe im Bericht ist freiwillig.

### Grundsatz

NV-UV hat keine Benutzerkonten, keine Werbung, verkauft keine Daten und führt keine Hintergrundtelemetrie durch. Die App lädt Fehlerberichte oder Spielwünsche niemals automatisch hoch.

Bei einem Fehler kann NV-UV automatisch ein Diagnosepaket **nur lokal** unter `%LocalAppData%\NV-UV\error-reports\` sichern. Dieses Paket verlässt den Computer nicht, bis der Nutzer in der App den Inhalt prüft, ausdrücklich einwilligt und auf **Senden** klickt. Das lokale Paket bleibt nach dem Senden erhalten, bis der Nutzer es selbst löscht.

NV-UV besitzt daneben freiwillige Community-Funktionen. Jede Übermittlung wird durch eine sichtbare Nutzeraktion ausgelöst. Reine Downloads öffentlicher Community-Daten benötigen keine Übermittlung an den Community- oder Berichts-Worker.

### Freiwillige private Fehlerberichte

Vor dem Senden zeigt NV-UV den Berichtstyp, App-Version, Zusammenfassung, optionalen Kontakt, den vollständigen Pfad und Namen des ZIP-Pakets, Dateigröße, entpackte Größe, SHA-256-Prüfsumme und jede enthaltene Datei an. Ändert sich der Entwurf oder das Paket, wird die Einwilligung zurückgesetzt.

Ein privater Fehlerbericht kann enthalten:

- die vom Nutzer eingegebene Fehlerbeschreibung und einen optionalen Kontakt;
- App-Version, Einwilligungszeitpunkt, Paketname, Größen und SHA-256-Prüfsumme;
- NV-UV-Aktivitäts- und Diagnoselogs, Systeminformationen und eine lokale Fehlerbeschreibung (`failure.json`);
- dadurch möglicherweise Hardware-/GPU- und Treiberangaben, Windows- und Anzeigeeinstellungen, aktive Spiel-/Prozessnamen, Dateipfade, Konfigurationszustände und technische Fehlermeldungen.

Bekannte Werte für Windows-Benutzerprofil, Benutzername und Computername werden beim Erstellen des Pakets lokal ersetzt. Logs können dennoch weitere persönliche oder vertrauliche Inhalte enthalten. Deshalb muss der Nutzer die genaue Dateiliste und Vorschau vor jeder Übermittlung prüfen. Das öffentliche NV-UV-Repository benötigt keine privaten Logs oder CFG-Dateien; öffentlich darf höchstens eine bereinigte technische Zusammenfassung zusammen mit der Bericht-ID erscheinen.

Zweck ist ausschließlich, den gemeldeten Fehler zu untersuchen, Rückfragen zu ermöglichen, wenn freiwillig ein Kontakt angegeben wurde, Missbrauch zu verhindern und den Bericht anschließend zu löschen. Rechtsgrundlage für die Übermittlung und Analyse ist die ausdrückliche Einwilligung des Nutzers. Es besteht keine Pflicht, einen Fehlerbericht zu senden oder eine Identität anzugeben.

### Freiwillige Spielwünsche

„Spiel für die Game Library vorschlagen“ ist ein eigener Berichtstyp und wird nicht mit Fehlerberichten vermischt. Vor dem Senden zeigt NV-UV alle Felder und eine optional ausgewählte Datei samt Pfad, Typ, Größe und SHA-256-Prüfsumme an und verlangt eine eigene ausdrückliche Einwilligung.

Ein Spielwunsch enthält den Spielnamen und die Plattform/den Store. Er kann außerdem die nach sichtbarer Prüfung übernommene oder bearbeitete EXE-Bezeichnung, einen optionalen Kommentar, einen optionalen Kontakt sowie eine bewusst ausgewählte JSON-, TXT- oder ZIP-Profil-/Exportdatei enthalten. NV-UV hängt dabei keine Diagnoselogs automatisch an; Diagnosepakete werden für diesen Berichtstyp abgelehnt.

Zweck ist ausschließlich die Prüfung einer möglichen Aufnahme oder Korrektur in der NV-UV Game Library und eine optionale Rückfrage. Auch diese Übermittlung ist freiwillig und beruht auf ausdrücklicher Einwilligung.

### Private Cloudflare-Infrastruktur und Verbindungsdaten

Private Fehlerberichte und Spielwünsche werden über einen Cloudflare Worker entgegengenommen. Cloudflare verarbeitet die Daten dabei als Infrastruktur-/Auftragsverarbeiter im Rahmen der für das verwendete Konto geltenden Vereinbarungen. Berichtsinhalte werden in einem privaten R2-Bucket mit EU-Jurisdiction gespeichert; die D1-Datenbank mit dem Berichtsindex ist ebenfalls auf die EU-Jurisdiction beschränkt. Diese Auswahl beschränkt Speicherort und Betrieb von R2/D1, nicht jedoch zwingend die Verarbeitung jeder Worker-Anfrage auf die EU: Cloudflare Workers werden im Cloudflare-Netz ausgeführt und können auf eine EU-beschränkte D1-Datenbank auch von außerhalb der EU zugreifen. Der Bucket besitzt keinen öffentlichen Zugriff. Die öffentliche Route kann ausschließlich einen neuen Bericht anlegen; Liste, Lesen, Ändern, Herunterladen und Löschen sind dort nicht möglich. Verwaltungszugriffe laufen über einen getrennten, durch Cloudflare Access geschützten Dienst.

Wie bei jeder Internetverbindung erhält Cloudflare technische Verbindungs- und Sicherheitsmetadaten, insbesondere die IP-Adresse, Zeitpunkt, Protokoll-/HTTP-Daten und gegebenenfalls Sicherheitsereignisse. Der NV-UV-Berichtsdienst bildet aus IP-Adresse, Berichtstyp, Datum und einem nur serverseitigen Geheimnis einen täglich wechselnden HMAC-Schlüssel für das Rate-Limit. Weder Klartext-IP noch dieser Rate-Limit-Schlüssel werden in R2 oder D1 als Teil des Berichts gespeichert. Für Cloudflares eigene notwendige Verarbeitung und Aufbewahrung von Verbindungs-/Sicherheitsmetadaten gelten die Kontoeinstellungen und Cloudflare-Vertrags- und Datenschutzbedingungen; sie kann von der unten genannten Berichtsfrist abweichen.

Die EU-Speicherwahl verhindert daher nicht zwingend jeden technischen Zugriff oder jede Verarbeitung aus einem Drittland. Soweit Cloudflare personenbezogene Daten für NV-UV als Auftragsverarbeiter verarbeitet, gelten die für das Konto vereinbarten Bedingungen einschließlich des anwendbaren [Cloudflare Data Processing Addendum (DPA)](https://www.cloudflare.com/cloudflare-customer-dpa/). Das DPA enthält Regelungen zu internationalen Übermittlungen einschließlich EU-Standardvertragsklauseln. Ergänzend gilt die [Cloudflare-Datenschutzerklärung](https://www.cloudflare.com/privacypolicy/) für Cloudflares eigene Verarbeitung.

### Speicherdauer, Löschung und Widerruf

Private Berichte werden nach der Analyse so früh wie möglich manuell gelöscht. Zusätzlich setzt der Berichtsdienst eine Löschfrist zwei Stunden vor Ablauf von sieben Tagen; ein stündlicher, geschützter Löschlauf entfernt dann sowohl eine vorhandene R2-Datei als auch den zugehörigen D1-Eintrag. Eine separate R2-Lifecycle-Regel nach sieben Tagen ist nur ein zusätzliches Sicherheitsnetz für Dateien, da Cloudflare die Lifecycle-Ausführung technisch verzögert durchführen kann. Ziel und konfigurierte Anwendungsfrist sind höchstens sieben Tage. Technische Löschfehler werden nach Erkennung so schnell wie möglich behoben. Die Bericht-ID wird dem Nutzer nach erfolgreichem Senden angezeigt und ist der Bezug für Lösch- oder Widerrufsanfragen.

Der Nutzer kann seine Einwilligung für einen noch vorhandenen Bericht über die bekannten NV-UV-Kontaktkanäle unter Angabe der Bericht-ID widerrufen und die Löschung verlangen. Ein optional angegebener Kontakt hilft bei der Zuordnung, ist aber keine Voraussetzung. Ein Widerruf berührt nicht die Rechtmäßigkeit der vorherigen Verarbeitung. Bereits automatisch abgelaufene oder gelöschte Berichte können nicht wiederhergestellt werden.

### Rechte der betroffenen Person

Soweit die gesetzlichen Voraussetzungen erfüllt sind, bestehen Rechte auf Auskunft, Berichtigung, Löschung, Einschränkung der Verarbeitung und Datenübertragbarkeit. Eine erteilte Einwilligung kann jederzeit für die Zukunft widerrufen werden. Außerdem besteht das Recht, sich bei einer zuständigen Datenschutzaufsichtsbehörde zu beschweren. NV-UV trifft anhand privater Berichte keine automatisierten Entscheidungen und erstellt daraus keine Nutzerprofile. Da NV-UV keine Benutzerkonten führt und Berichte nach der Löschung nicht wiederherstellen kann, ist für Anfragen zu einem noch vorhandenen Bericht dessen Bericht-ID erforderlich.

### Community-Bewertungen (UV⚡Try)

Ein Klick auf einen Stern speichert die Bewertung lokal und sendet freiwillig drei Werte an den NV-UV-Community-Worker: Preset-ID, Sternwert (1 bis 5) und eine zufällige Client-ID. Die lokal erzeugte GUID enthält keine Hardwareinformation, Seriennummer oder absichtliche persönliche Referenz. Sie verhindert Mehrfachstimmen derselben Installation und liegt unter `%LocalAppData%\NV-UV\nv-uv_client_id.txt`. Ein erneuter Klick auf denselben Stern (0 Sterne) löscht die Stimme serverseitig; die lokale Datei kann jederzeit gelöscht werden.

Nur Durchschnitt und Stimmenzahl je Preset werden als `community/ratings.json` veröffentlicht.

### Community-Preset-Einreichungen

Eine bewusst ausgelöste Preset-Einreichung übermittelt Frequenz, Spannung, Power-Limit/VRAM-Offset, erkanntes GPU-Modell, optional Board-Variante und kurzen Freitext sowie dieselbe zufällige Client-ID. Nach Moderation werden das Preset, die optionale Board-Variante und der Hinweis öffentlich. Der Nutzer darf dort keine persönlichen Daten eintragen. Die Client-ID wird nicht veröffentlicht. Ein veröffentlichtes Preset kann über die bekannten NV-UV-Kontaktkanäle zur Löschung gemeldet werden.

NV-UV nutzt die genannten Daten nicht für Werbung, Profilbildung oder Verkauf an Dritte.

---

## English

### Controller and contact

The controller for the processing described here is Christian Papaioannou, operator of the NV-UV project ([christianp403-spec](https://github.com/christianp403-spec)). Privacy, withdrawal, and deletion requests can be sent by email to [christian.papaioannou@protonmail.com](mailto:christian.papaioannou@protonmail.com) or in a private message through the [PCGH NV-UV subforum](https://extreme.pcgameshardware.de/forums/nv-uv.3601/) or Discord. Include the report ID only in the private message when referring to a private report and do not post it publicly. Providing contact information in a report is voluntary.

### Principle

NV-UV has no user accounts, carries no advertising, does not sell data, and performs no background telemetry. The app never uploads an error report or game request automatically.

When a failure occurs, NV-UV may automatically save a diagnostics package **locally only** under `%LocalAppData%\NV-UV\error-reports\`. It does not leave the computer unless the user reviews the contents in the app, explicitly consents, and clicks **Send**. The local package remains after sending until the user deletes it.

NV-UV also provides voluntary community features. Every submission is caused by a visible user action. Merely downloading public community data does not send data to the community or reporting Worker.

### Voluntary private error reports

Before sending, NV-UV displays the report type, app version, summary, optional contact, full ZIP path and name, file size, expanded size, SHA-256 hash, and every included file. Changing the draft or package resets consent.

A private error report can contain:

- the user's error summary and an optional contact;
- app version, consent time, package name, sizes, and SHA-256 hash;
- NV-UV activity and diagnostics logs, system information, and a local failure description (`failure.json`);
- consequently, possible hardware/GPU and driver details, Windows and display settings, active game/process names, file paths, configuration states, and technical error messages.

Known Windows user-profile, user-name, and computer-name values are replaced locally while the package is created. Logs can still contain other personal or confidential material, so the user must review the exact file list and preview before every submission. The public NV-UV repository does not need private logs or CFG files; at most, a sanitized technical summary and the report ID may be made public.

The sole purposes are to investigate the reported problem, permit follow-up when a contact was voluntarily provided, prevent abuse, and delete the report afterwards. The legal basis for submission and analysis is the user's explicit consent. Sending a report or providing an identity is never required.

### Voluntary game requests

“Suggest a game for the Game Library” is a distinct report type and is not mixed with error reports. Before sending, NV-UV shows every field and any optionally selected file with its path, type, size, and SHA-256 hash, and requires separate explicit consent.

A game request contains the game name and platform/store. It may also contain an executable name accepted or edited after visible review, an optional comment, an optional contact, and a consciously selected JSON, TXT, or ZIP profile/export file. NV-UV does not attach diagnostics logs automatically to this report type, and diagnostics packages are rejected.

Its sole purpose is to consider an addition or correction to the NV-UV Game Library and optionally ask a follow-up question. Submission is voluntary and based on explicit consent.

### Private Cloudflare infrastructure and connection data

Private error reports and game requests are received by a Cloudflare Worker. Cloudflare processes the data as an infrastructure/data processor under the agreements applicable to the account in use. Report contents are stored in a private R2 bucket with EU Jurisdiction, and the D1 database containing the report index is also restricted to the EU Jurisdiction. This choice restricts where R2/D1 store and operate on the data, but it does not necessarily restrict every Worker request to processing in the EU: Cloudflare Workers run on Cloudflare's network and may access an EU-restricted D1 database from outside the EU. The bucket has no public access. The public route can only create a new report; it cannot list, read, change, download, or delete reports. Administrative access uses a separate service protected by Cloudflare Access.

As with any Internet connection, Cloudflare receives technical connection and security metadata, particularly the IP address, time, protocol/HTTP data, and possibly security events. The NV-UV reporting service derives a daily-changing HMAC key from the IP address, report type, date, and a server-only secret for rate limiting. Neither the plaintext IP nor this rate-limit key is stored in R2 or D1 as report data. Cloudflare's necessary processing and retention of its own connection/security metadata is governed by the account settings and Cloudflare contract and privacy terms and can differ from the report-retention period below.

Choosing EU storage therefore does not necessarily prevent every technical access or processing from a third country. Where Cloudflare processes personal data for NV-UV as a processor, the terms agreed for the account apply, including the applicable [Cloudflare Data Processing Addendum (DPA)](https://www.cloudflare.com/cloudflare-customer-dpa/). The DPA includes provisions for international transfers, including the EU Standard Contractual Clauses. The [Cloudflare Privacy Policy](https://www.cloudflare.com/privacypolicy/) additionally applies to Cloudflare's own processing.

### Retention, deletion, and withdrawal

Private reports are manually deleted as soon as practical after analysis. In addition, the reporting service sets a deletion deadline two hours before seven full days; an hourly protected deletion job then removes both any R2 file and its related D1 row. A separate seven-day R2 lifecycle rule is only an additional safety net for files because Cloudflare may technically execute lifecycle deletion later. The intended and configured application retention is no more than seven days. Technical deletion failures are corrected as soon as they are detected. After a successful submission, the user receives a report ID that serves as the reference for deletion or withdrawal requests.

The user may withdraw consent for a report that still exists and request deletion through the established NV-UV contact channels by providing the report ID. An optional contact can help matching but is not required. Withdrawal does not affect the lawfulness of processing that occurred beforehand. Reports that have already expired or been deleted cannot be recovered.

### Data-subject rights

Where the legal requirements are met, the user has rights of access, rectification, erasure, restriction of processing, and data portability. Consent can be withdrawn at any time for the future. The user also has the right to lodge a complaint with a competent data-protection supervisory authority. NV-UV does not make automated decisions or create user profiles from private reports. Because NV-UV has no user accounts and cannot restore deleted reports, the report ID is required for a request concerning a report that still exists.

### Community ratings (UV⚡Try)

Clicking a star stores the rating locally and voluntarily sends three values to the NV-UV community Worker: preset ID, star value (1 to 5), and a random client ID. The locally generated GUID contains no hardware information, serial number, or intentional personal reference. It prevents duplicate votes from one installation and is stored at `%LocalAppData%\NV-UV\nv-uv_client_id.txt`. Clicking the same star again (0 stars) deletes the server-side vote; the local file can be deleted at any time.

Only the average and vote count for each preset are published as `community/ratings.json`.

### Community preset submissions

A deliberately triggered preset submission sends frequency, voltage, power limit/VRAM offset, detected GPU model, an optional board variant and short free text, plus the same random client ID. After moderation, the preset, optional board variant, and note become public. The user must not enter personal information there. The client ID is never published. A published preset can be reported for deletion through the established NV-UV contact channels.

NV-UV does not use any of the data described above for advertising, profiling, or sale to third parties.
