@echo off
echo Committing fix to release notes input...

git add create_release.bat
git commit -m "Simplify release notes to fix script hanging"
git push origin master

echo Done! Now try running create_release.bat again.
