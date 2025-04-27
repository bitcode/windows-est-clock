@echo off
echo Fixing GitHub Actions permissions...

git add .github/workflows/release.yml
git commit -m "Add write permissions to GitHub token for releases"
git push origin master

echo Done! Now try creating a release again.
