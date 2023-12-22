#Parameter
$Directories = "..\Binaries", "..\DerivedDataCache", "..\Intermediate", "..\Saved"
 
#Delete files in each directory
ForEach ($Dir in $Directories) {
    if (Test-Path $Dir -PathType Container) {
        Remove-Item -Path $Dir -Recurse -Force
        Write-Host ""$Dir" `tdirectory removed successfully."
    } else {
        Write-Host ""$Dir" `tdirectory does not exist."
    }
}
