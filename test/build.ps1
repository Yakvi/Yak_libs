#Build file for Yak_Lib tests
# $dateFormat = "dddd MM/dd/yyyy HH:mm:fff"
$dateFormat = "HH:mm:fff"

function Output-Logs([String[]]$data, [string]$title = "") {
    foreach ($line in $data) {
        If ($line -match "error ") {
            [string]$m = $matches.Values[0].trim()
            $i = $line.IndexOf($m)
            $line.Substring(0, $i) | Write-Host -ForegroundColor "Red" -NoNewline
            $line.Substring($i) | Write-Host -ForegroundColor "Gray" 
        }
        elseif ($line -match "warning ") {
            [string]$m = $matches.Values[0].trim()
            $i = $line.IndexOf($m)
            $line.Substring(0, $i) | Write-Host -ForegroundColor "DarkYellow" -NoNewline
            $line.Substring($i) | Write-Host -ForegroundColor "Gray" 
        }
        elseif ($line -match "note ") {
            [string]$m = $matches.Values[0].trim()
            $i = $line.IndexOf($m)
            $line.Substring(0, $i) | Write-Host -ForegroundColor "Cyan" -NoNewline
            $line.Substring($i) | Write-Host -ForegroundColor "Gray" 
        }
        else {
            Write-Host $line
        }
    }

    if ($data -match "error") {
        Write-Host "[$(Get-Date -Format $dateFormat)]: " -ForegroundColor "Yellow" -NoNewLine 
        Write-Host "Compilation failed, " -ForegroundColor "Red" -NoNewLine
        Write-Host $title -ForegroundColor "Cyan"
    }
}

$srcDir = "test"
$buildDir = "build"
$win32file = "win32_" + $project + ".cpp"

# NOTE: Compiler flags
$c = '-nologo','-FC'                   #Display full path of source code
# NOTE: Faster compile/runtime
$c += '-fp:fast'                       #Floating point behaviour. Precise behaviour usually unnecessary.
$c += '-fp:except-'                    #Floating point behaviour. Precise behaviour usually unnecessary.
$c += '-Gm-'                           #Enables minimal rebuild. For faster compile.
$c += '-GR-'                           #Disables run-time type information. For faster compile.
$c += '-Oi'                            #Generates intrinsic functions. For faster runtime.
$c += '-DYAK_DEV=1'                    #For debug stuff
# NOTE: other
$c += '-EHa-'                          #Disable exception handling, -EHsc for c++
# NOTE: Debug mode
$debug = '-DDEBUG=1', '-D_DEBUG=1'     #Basic debug defines
$debug += '-Od'                        #Optimizations disabled
$debug += '-MTd'                       #Creates debug multithreaded executable
$debug += '-Zo'                        #Enhance Optimized Debugging
$debug += '-Z7'                        #Generates C 7.0–compatible debugging information.
$debug += '-WX'                        #Treats all warnings as errors (except the ones below).
$debug += '-W4'                        #Displays all warnings (except the ones below).
# NOTE: Ignoring selected warnings:
$debug += '-wd4100'                  #Unreferenced variable
$debug += '-wd4189'                  #Local variable initialized but not used
$debug += '-wd4505'                  #Unreferenced local function
# $debug += '-wd4201'                  #Nonstandard extension used: nameless struct/union
# $debug += '-wd4127'                  #
# $debug += '-wd4302'                  #pointer truncation
# $debug += '-wd4311'                  #truncation
# $debug += '-wd4838'                  #Conversion from type 1 to type 2 requires a narrowing conversion
# $debug += '-wd4456'                  #Declaration hides previous local declaration

# NOTE: linker flags, go after the source file
$linker = '/link', '-incremental:no'   #Passes linker parameters from here; Disables incremental linking of the linker
$linker += '-opt:ref'                  #Eliminates functions and data that are never referenced
# NOTE: Extra libraries for win32
$32linker = 'user32.lib','gdi32.lib'   #Creates and manipulates the standard elements of the Windows GUI. #Graphics Device Interface, used for primitive drawing functions.
# $32linker += 'kernel32.lib'
# $32linker += 'shell32.lib'

#timeout /t 1
# Remove-Item -Path ..\$buildDir -Force -Recurse # NOTE: Clean build
if(!(Test-Path -Path ..\$buildDir)) { mkdir ..\$buildDir }
pushd ..\$buildDir
Clear
Write-Host "[$(Get-Date -Format $dateFormat)]: " -ForegroundColor "Yellow" -NoNewLine 
Write-Host "Compilation started." -ForegroundColor "Cyan"
H:\C\vcvars.ps1
Write-Host ""
$CompileTimer = [System.Diagnostics.Stopwatch]::StartNew()

### BOOKMARK: Actual compiler calls

# NOTE: Yak_String Tests
# $yak_string = & cl -O2 $c ..\$srcDir\yak_string_test.cpp $linker $32linker
# Output-Logs -data $yak_string -title "yak string tests"
# NOTE: Yak_Memory Tests
$yak_memory = & cl -O2 $c ..\$srcDir\yak_memory_test.cpp -Fmyak_memory_test $linker $32linker
Output-Logs -data $yak_memory -title "yak memory tests"
# NOTE: Yak_Win32 Tests
$yak_win32 = & cl -O2 $c $debug ..\$srcDir\yak_win32_test.cpp -Fmyak_win32_test $linker $32linker
Output-Logs -data $yak_win32 -title "yak Win32 tests"

# NOTE: Compiling Diagnostics
$CompileTime = $CompileTimer.Elapsed
Write-Host ""
Write-Host "[$(Get-Date -Format $dateFormat)]: " -ForegroundColor "Yellow" -NoNewLine 
Write-Host "Compilation finished in " -ForegroundColor "Cyan" -NoNewLine
Write-Host $([string]::Format("{0:d1}s {1:d3}ms", $CompileTime.seconds, $CompileTime.milliseconds)) -ForegroundColor "Green"

### BOOKMARK: Running tests
function Run-Command([String]$Command)
{
    $Timer = [System.Diagnostics.Stopwatch]::StartNew()
    & $Command
    $ElapsedTime = $Timer.Elapsed
    Write-Host $([string]::Format("{0:d1}s {1:d3}ms", $ElapsedTime.seconds, $ElapsedTime.milliseconds)) -ForegroundColor "Green"

}
# Run-Command ./yak_string_test.exe
Run-Command ./yak_memory_test.exe
Run-Command ./yak_win32_test.exe

popd
