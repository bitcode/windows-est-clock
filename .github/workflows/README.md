# GitHub Actions Workflows

This directory contains GitHub Actions workflow files that automate various tasks for the EST Clock project.

## Available Workflows

### `release.yml`

This workflow automatically builds and publishes releases when a new tag is pushed to the repository.

#### Trigger

The workflow is triggered by:
- Pushing a tag that starts with "v" (e.g., v1.0.0)
- Manual trigger from the Actions tab in GitHub

#### What it does

1. Sets up a Windows environment with MinGW
2. Builds the EST Clock application using build.bat
3. Creates a GitHub Release with the tag name
4. Attaches the windows-est-clock-portable.exe file to the release

#### How to use it

The easiest way to trigger this workflow is to use the `create_release.bat` script in the root of the repository. This script will:

1. Build the application locally to ensure it compiles
2. Ask for a version number
3. Create and push a git tag with that version
4. The GitHub Actions workflow will then automatically create the release

Alternatively, you can manually create and push a tag:

```bash
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0
```

Or you can manually trigger the workflow from the Actions tab in GitHub.
