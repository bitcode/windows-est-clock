@echo off
echo Committing fixes to release scripts...

git add build.bat
git add create_release.bat
git commit -m "Fix build and release scripts to handle errors better"
git push origin master

echo Done! Now try running create_release.bat again.
