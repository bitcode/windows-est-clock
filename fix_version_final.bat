@echo off
echo Committing final fix to version validation...

git add create_release.bat
git commit -m "Remove complex version validation to fix release script"
git push origin master

echo Done! Now try running create_release.bat again.
