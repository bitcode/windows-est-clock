# Versioning Strategy for EST Clock

This document outlines the versioning strategy for the EST Clock application.

## Semantic Versioning

EST Clock follows [Semantic Versioning 2.0.0](https://semver.org/) (SemVer) for version numbering.

### Format

```
MAJOR.MINOR.PATCH[-PRERELEASE][+BUILD]
```

Where:
- **MAJOR**: Incremented for incompatible changes (e.g., changes that might affect user settings or workflows)
- **MINOR**: Incremented for new features added in a backward-compatible manner
- **PATCH**: Incremented for backward-compatible bug fixes
- **PRERELEASE**: Optional identifier for pre-release versions (e.g., alpha, beta, rc)
- **BUILD**: Optional build metadata

### Examples

- `1.0.0` - Initial stable release
- `1.1.0` - Added new feature (e.g., additional timezone support)
- `1.1.1` - Bug fix release
- `2.0.0` - Major update with potentially breaking changes
- `1.2.0-beta.1` - Beta release of upcoming 1.2.0 version

## Rationale for Semantic Versioning

Semantic Versioning is particularly well-suited for the EST Clock application for several reasons:

1. **Clarity for Users**: SemVer provides clear signals to users about the nature of changes in each release. Users can easily understand if an update contains just bug fixes, new features, or potentially breaking changes.

2. **Desktop Application Context**: As a standalone desktop application, EST Clock benefits from a versioning scheme that clearly communicates the significance of each update. Users can make informed decisions about when to update based on the version number.

3. **Future Extensibility**: As outlined in the project's future plans (additional timezone support, customization options, etc.), the application is likely to evolve with new features. SemVer's MINOR version increments provide a natural way to represent these feature additions.

4. **Simplicity**: SemVer is widely understood in the software development community, making it easy for contributors to understand and follow.

5. **Compatibility Signaling**: If future versions introduce changes to settings storage formats or user workflows, the MAJOR version increment provides a clear signal about potential compatibility issues.

## Version Display

The current version should be:

1. Displayed in the application's system tray tooltip
2. Included in the "About" information (if implemented)
3. Clearly marked in all release notes
4. Tagged in the Git repository

## Release Process

1. Determine the appropriate version number based on the changes since the last release
2. Update version information in the code (resource file)
3. Create a Git tag for the version
4. Build the release executable (`windows-est-clock-portable.exe`)
5. Create a GitHub release with release notes
6. Attach the executable to the GitHub release

## Alternative Approaches Considered

### Calendar Versioning (CalVer)

Calendar Versioning (e.g., `YYYY.MM.DD` or `YY.MM.MICRO`) was considered but not adopted because:

- EST Clock's development pace may not align well with calendar-based releases
- The nature of changes (features vs. bug fixes) is not immediately apparent from CalVer
- SemVer better communicates the significance of changes to users

### Simple Incremental Versioning

Simple incremental versioning (e.g., v1, v2, v3) was considered but not adopted because:
- It doesn't provide enough granularity for different types of changes
- It doesn't clearly communicate the nature of changes to users

## Conclusion

Semantic Versioning provides the right balance of clarity, simplicity, and expressiveness for the EST Clock project. It effectively communicates the nature of changes to users and contributors, while providing a structured approach to version numbering that can accommodate the project's future growth.