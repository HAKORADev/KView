# Changelog

All notable changes to KView will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.1.0] - 2026-03-20

### Added

- **Initial Release** of KView - KDE Fast File Previewer
- **Dolphin Integration** - Right-click "KView Preview" in KDE file manager
- **Multi-Format Preview Support:**
  - Images (PNG, JPG, GIF, BMP, WEBP, SVG, etc.)
  - Videos (MP4, MKV, WEBM, AVI, etc.)
  - Audio (MP3, WAV, OGG, FLAC, etc.)
  - Text files (TXT, MD, JSON, code files, etc.)
  - PDF documents
  - Archives (ZIP, TAR, 7Z) - view contents
- **Frameless Window** - Clean, borderless preview that stays on top
- **Directory Navigation:**
  - Left/Right arrows to browse files in same folder
  - Smart wrapping with end/start warnings
  - File counter display (e.g., "3/15")
- **Media Controls:**
  - Space: Play/Pause for video and audio
  - Q: Seek backward 10%
  - E: Seek forward 10%
- **Auto-Setup on First Run:**
  - Creates .desktop file for application menu
  - Creates KDE service menu for Dolphin right-click
  - Refreshes KDE system database automatically
- **File Size Display** - Shows formatted file size
- **Open with Default App** - Press Enter to open in default application

### Technical Details

- Built with Qt 6.6+ and KDE Frameworks 6
- Uses Kirigami for UI components
- Uses KArchive for archive preview
- Uses Qt Multimedia for video/audio playback
- Uses Qt Pdf for PDF rendering

---

[0.1.0]: https://github.com/HAKORADev/KView/releases/tag/v0.1.0
