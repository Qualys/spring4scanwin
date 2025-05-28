THIS SCRIPT IS PROVIDED TO YOU "AS IS." TO THE EXTENT PERMITTED BY LAW, QUALYS HEREBY DISCLAIMS ALL WARRANTIES AND LIABILITY FOR THE PROVISION OR USE OF THIS SCRIPT. IN NO EVENT SHALL THESE SCRIPTS BE DEEMED TO BE CLOUD SERVICES AS PROVIDED BY QUALYS

# Project Deprecated
Functionality has been rolled into our product offerings, see https://www.qualys.com/apps/software-composition-analysis/ for details.

# Spring4Scanner
## Description
The Spring4Scan.exe utility helps to detect CVE-2022-22963, and CVE-2022-22965 vulnerabilities.
The utility will scan the entire hard drive(s) including archives (and nested JARs) for the Java libraries that indicates the Java application contains a vulnerable spring framework or spring cloud library. The utility will output its results to a console.

**NOTE:** Preconditions related to JDK and Tomcat are environmental factors and are not covered by this utility.

QID descriptions can be found here:
https://blog.qualys.com/vulnerabilities-threat-research/2022/03/31/spring-framework-zero-day-remote-code-execution-spring4shell-vulnerability

Qualys customers should use the following to run the tool on any asset they want to scan, from an elevated command prompt:
> Spring4Scan.exe /scan /report_sig

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
> Spring4Scan.exe /scan

Sample Usage (from an elevated command prompt) - The following command helps you scan local drives for vulnerable files and writes a signature report to C:\ProgramData\Qualys
> Spring4Scan.exe /scan /report_sig

## Output - The following output shows the detection
```
D:\Temp>Spring4Scan.exe /scan_directory d:\spring4shell
Qualys Spring-Boot Vulnerability Scanner 1.0.0.0
https://www.qualys.com/
Dependencies: minizip/1.1 zlib/1.2.11, bzip2/1.0.8, rapidjson/1.1.0
Supported CVE(s): CVE-2022-22963, CVE-2022-22965

Known TAR Extensions            : .tar
Known GZIP TAR Extensions       : .tgz, .tar.gz
Known BZIP TAR Extensions       : .tbz, .tbz2, .tar.bz, .tar.bz2
Known ZIP Extensions            : .zip, .jar, .war, .ear, .par, .kar, .sar, .rar, .jpi, .hpi, .apk

Scanning 'd:\spring4shell\'...
Spring-Framework Found: 'd:\spring4shell\demo-0.0.1-SNAPSHOT.war' ( Manifest Title: demo, Manifest Vendor: Unknown, Manifest Version: 0.0.1-SNAPSHOT, CVE Status: Mitigated )
Spring Cloud Found: 'd:\spring4shell\demo-cloud-0.0.1-SNAPSHOT.war!WEB-INF/lib/spring-cloud-function-core-3.2.2.jar' ( Manifest Title: Spring Cloud Function Core, Manifest Vendor: Pivotal Software, Inc., Manifest Version: 3.2.2, CVE Status: Potentially Vulnerable ( CVE-2022-22963: Found ) )
Spring-Framework Found: 'd:\spring4shell\demo-cloud-0.0.1-SNAPSHOT.war' ( Manifest Title: demo-cloud, Manifest Vendor: Unknown, Manifest Version: 0.0.1-SNAPSHOT, CVE Status: Mitigated )
Spring Cloud Found: 'd:\spring4shell\spring-cloud-function-core-3.2.2.jar' ( Manifest Title: Spring Cloud Function Core, Manifest Vendor: Pivotal Software, Inc., Manifest Version: 3.2.2, CVE Status: Potentially Vulnerable ( CVE-2022-22963: Found ) )

Scan Summary:
        Scan Date:                       2022-04-03T15:02:58-0700
        Scan Duration:                   28 Seconds
        Scan Error Count:                0
        Scan Status:                     Success
        Files Scanned:                   5
        Directories Scanned:             0
        Compressed File(s) Scanned:      348
        JAR(s) Scanned:                  341
        WAR(s) Scanned:                  3
        EAR(s) Scanned:                  0
        TAR(s) Scanned:                  0
        Vulnerabilities Found:           3
```
