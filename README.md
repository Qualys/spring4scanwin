THIS SCRIPT IS PROVIDED TO YOU "AS IS." TO THE EXTENT PERMITTED BY LAW, QUALYS HEREBY DISCLAIMS ALL WARRANTIES AND LIABILITY FOR THE PROVISION OR USE OF THIS SCRIPT. IN NO EVENT SHALL THESE SCRIPTS BE DEEMED TO BE CLOUD SERVICES AS PROVIDED BY QUALYS

# Direct Download Links
https://github.com/Qualys/log4jscanwin/releases/download/2.1.2.0/Log4jScanner-2.1.2.0.zip
https://github.com/Qualys/log4jscanwin/releases/download/2.1.2.0/Log4jRemediate-1.2.1.1.zip

# Log4jScanner
## Description
The Log4jScanner.exe utility helps to detect CVE-2021-44228, CVE-2021-44832, CVE-2021-45046, and CVE-2021-45105 vulnerabilities.
The utility will scan the entire hard drive(s) including archives (and nested JARs) for the Java class that indicates the Java application contains a vulnerable log4j library. The utility will output its results to a console.

Qualys has added the following new QIDs that are designed to look for the results of this scan and mark the asset as vulnerable if the vulnerable log4j library was found.
- (376160) CVE-2021-44228
- (376193) CVE-2021-45046
- (376195) CVE-2021-45105
- (376210) CVE-2021-44832
- (45515) Information Gathering that the Log4j Scan Utility was ran on the host

Qualys customers should use the following to run the tool on any asset they want to scan, from an elevated command prompt:
> Log4jScanner.exe /scan /report_sig

## Usage
```
/scan
  Scan local drives for vulnerable files used by various Java applications.
/scan_network
  Scan network drives for vulnerable files used by various Java applications.
/scan_directory "C:\Some\Path"
  Scan a specific directory for vulnerable files used by various Java applications.
/scan_file "C:\Some\Path\Some.jar"
  Scan a specific file for supported CVE(s).
/scaninclmountpoints
  Scan local drives including mount points for vulnerable files used by various Java applications.
/exclude_drive "C:\"
  Exclude a drive from the scan.
/exclude_directory "C:\Some\Path"
  Exclude a directory from a scan.
/exclude_file "C:\Some\Path\Some.jar"
  Exclude a file from a scan.
/knownTarExtension ".tar"
/knownGZipTarExtension ".tgz"
/knownBZipTarExtension ".tbz"
/knownZipExtension ".jar"
  Add additional file type extensions to the scanner.
/report
  Generate a JSON report of possible detections of supported CVE(s).
/report_pretty
  Generate a human readable JSON report of possible detections of supported CVE(s).
/report_sig
  Generate a signature report of possible detections of supported CVE(s).
/lowpriority
  Lowers the execution and I/O priority of the scanner.
/help
  Displays this help page.
```

Sample Usage (from an elevated command prompt) - The following command helps you scan local drives for vulnerable JAR, WAR, EAR, and ZIP.
> Log4jScanner.exe /scan

Sample Usage (from an elevated command prompt) - The following command helps you scan local drives for vulnerable files and writes a signature report to C:\ProgramData\Qualys
> Log4jScanner.exe /scan /report_sig

## Output - The following output shows the detection
```
D:\Temp>Log4jScanner.exe /scan /exclude_directory C:\ /knownZipExtension .ZZZ
Qualys Log4j Vulnerability Scanner 2.1.1.0
https://www.qualys.com/
Dependencies: minizip/1.1 zlib/1.2.11, bzip2/1.0.8
Supported CVE(s): CVE-2021-4104, CVE-2021-44228, CVE-2021-44832, CVE-2021-45046, CVE-2021-45105

Known TAR Extensions            : .tar
Known GZIP TAR Extensions       : .tgz, .tar.gz
Known BZIP TAR Extensions       : .tbz, .tbz2, .tar.bz, .tar.bz2
Known ZIP Extensions            : .zip, .jar, .war, .ear, .par, .kar, .sar, .rar, .jpi, .hpi, .apk, .ZZZ
Excluding Directories:
        C:\


Scanning Local Drives...
Log4j Found: 'D:\Src\Projects\log4jscanner\jar\testdata\arara.jar' ( Manifest Vendor: Unknown, Manifest Version: 6.1.1, JNDI Class: Found, Log4j Vendor: log4j-core, Log4j Version: 2.14.1, CVE Status: Potentially Vulnerable ( CVE-2021-44228: Found CVE-2021-44832: Found CVE-2021-45046: Found CVE-2021-45105: Found ) )
Log4j Found: 'D:\Src\Projects\log4jscanner\jar\testdata\arara.signed.jar' ( Manifest Vendor: Unknown, Manifest Version: 6.1.1, JNDI Class: Found, Log4j Vendor: log4j-core, Log4j Version: 2.14.1, CVE Status: Potentially Vulnerable ( CVE-2021-44228: Found CVE-2021-44832: Found CVE-2021-45046: Found CVE-2021-45105: Found ) )

Scan Summary:
        Scan Date:                       2022-01-10T10:05:18-0800
        Scan Duration:                   9 Seconds
        Scan Error Count:                1
        Scan Status:                     Partially Successful
        Files Scanned:                   184889
        Directories Scanned:             30159
        Compressed File(s) Scanned:      96
        JAR(s) Scanned:                  50
        WAR(s) Scanned:                  0
        EAR(s) Scanned:                  0
        TAR(s) Scanned:                  2
        Vulnerabilities Found:           22
```
