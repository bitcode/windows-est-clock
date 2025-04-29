# EST-Clock Test Runner with Time Resynchronization
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "EST-Clock Test Runner with Time Resynchronization" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Setting up test environment..." -ForegroundColor Yellow

# Store the current directory
$currentDir = Get-Location

# Navigate to the build directory if it exists, or create it
if (Test-Path -Path "build") {
    Set-Location -Path "build"
} else {
    New-Item -Path "build" -ItemType Directory | Out-Null
    Set-Location -Path "build"
}

Write-Host ""
Write-Host "Configuring tests with CMake..." -ForegroundColor Yellow
cmake -S .. -B .

Write-Host ""
Write-Host "Building tests..." -ForegroundColor Yellow
cmake --build .

Write-Host ""
Write-Host "Running tests..." -ForegroundColor Yellow
Write-Host "---------------------------------------------------" -ForegroundColor DarkGray
$testResult = $null
try {
    ctest -C Debug -V
    $testResult = $LASTEXITCODE
} catch {
    Write-Host "Error during test execution: $_" -ForegroundColor Red
    $testResult = 1
}

Write-Host ""
Write-Host "---------------------------------------------------" -ForegroundColor DarkGray
Write-Host "Ensuring time synchronization after tests..." -ForegroundColor Yellow
Write-Host "---------------------------------------------------" -ForegroundColor DarkGray

# Run time resynchronization regardless of test outcome
Write-Host "Resynchronizing system time..." -ForegroundColor Yellow
try {
    $resyncResult = Start-Process -FilePath "w32tm" -ArgumentList "/resync" -Wait -PassThru -NoNewWindow
    
    if ($resyncResult.ExitCode -ne 0) {
        Write-Host "WARNING: Failed to resynchronize time. Error code: $($resyncResult.ExitCode)" -ForegroundColor Red
        Write-Host "Attempting alternative resynchronization method..." -ForegroundColor Yellow
        
        # Try an alternative method to resync time
        $altResyncResult = Start-Process -FilePath "net" -ArgumentList "time", "/set" -Wait -PassThru -NoNewWindow
        
        if ($altResyncResult.ExitCode -ne 0) {
            Write-Host "WARNING: Alternative time resynchronization also failed." -ForegroundColor Red
            Write-Host "Please manually run 'w32tm /resync' in an administrator PowerShell." -ForegroundColor Red
        } else {
            Write-Host "Alternative time resynchronization completed successfully." -ForegroundColor Green
        }
    } else {
        Write-Host "Time resynchronization completed successfully." -ForegroundColor Green
    }
} catch {
    Write-Host "Error during time resynchronization: $_" -ForegroundColor Red
    Write-Host "Please manually run 'w32tm /resync' in an administrator PowerShell." -ForegroundColor Red
}

# Return to the original directory
Set-Location -Path $currentDir

Write-Host ""
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "Test execution completed with exit code: $testResult" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

# Return the test result as the script's exit code
exit $testResult