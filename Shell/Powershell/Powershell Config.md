`nvim $PROFILE`

- Nerd Fonts

```powershell
# using utf-8
$OutputEncoding = [System.Text.Encoding]::UTF8
# zoxide:
Invoke-Expression (& { (zoxide init powershell | Out-String) })
# github env
$env:GITHUB_TOKEN = "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"
$env:GH_LOG_LEVEL = "info"
# github completion
Invoke-Expression -Command $(gh completion -s powershell | Out-String)
# prompt
function prompt {
    $lastExitCode = $?
    $errorIcon = [char]::ConvertFromUtf32(0xf071)

    $currentTime = Get-Date -Format "MM-dd HH:mm"
    $timeIcon = [char]::ConvertFromUtf32(0xf0109)

    $branch = & git rev-parse --abbrev-ref HEAD 2>$null
    $repoInfo = $null
    if ($branch -and $branch -ne 'HEAD') {
        $remoteName = & git config --get branch.$branch.remote 2>$null
        if ($remoteName) {
            $remoteUrl = & git remote get-url $remoteName 2>$null
            if ($remoteUrl) {
                $repoName = $remoteUrl -replace '^.+github\.com[:\/](.+?)(\.git)?$', '$1'
                $repoInfo = "$branch -> $repoName"
            }
        }
    }
    $gitIcon = [char]::ConvertFromUtf32(0xf418)
    $gitInfo = if ($repoInfo) { " $gitIcon $repoInfo`n" } elseif ($branch) { " $gitIcon $branch`n" } else { "`n" }
    # output:
    Write-Host $gitInfo -ForegroundColor DarkBlue -NoNewline
    Write-Host " $timeIcon $currentTime " -NoNewline
    if (-not $lastExitCode) {
        Write-Host " $errorIcon " -ForegroundColor Red -NoNewline
    } else {
        $checkIcon = [char]::ConvertFromUtf32(0xe63f)
        Write-Host " $checkIcon " -ForegroundColor Green -NoNewline
    }
    " " + $(Get-Location) + " >> "
}
```