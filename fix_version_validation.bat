@echo off
echo Committing fixes to version validation...

git add create_release.bat
git commit -m "Fix version validation in create_release.bat"
git push origin master

echo Done! Now try running create_release.bat again.
