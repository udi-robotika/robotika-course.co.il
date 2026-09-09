@echo off
cd /d "%~dp0"
echo Connecting to the existing robotika-course project...
call npx vercel@latest link --project robotika-course --scope udi-personal --yes
if errorlevel 1 pause & exit /b 1
echo Checking permanent storage for the live code board...
call npx vercel@latest env ls production --json > "%TEMP%\robotika-storage-check.json"
findstr /C:"BLOB_READ_WRITE_TOKEN" "%TEMP%\robotika-storage-check.json" >nul
set "ROBOTIKA_STORAGE_FOUND=%ERRORLEVEL%"
del "%TEMP%\robotika-storage-check.json" >nul 2>&1
if not "%ROBOTIKA_STORAGE_FOUND%"=="0" (
  echo Creating and connecting permanent Blob storage...
  call npx vercel@latest blob create-store robotika-course-codes --access public --yes --environment production --environment preview --environment development
  if errorlevel 1 pause & exit /b 1
)
echo Deploying to production...
call npx vercel@latest --prod --yes
pause
