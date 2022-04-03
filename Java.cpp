
#include "stdafx.h"
#include "Utils.h"
#include "Reports.h"
#include "Scanner.h"
#include "Java.h"


bool IsWARVunerable(std::vector<std::string>& dependencies) {
  bool  isSpringCoreFound = false;
  bool  isSpringWebfluxFound = false;
  bool  isSpringWebmvcFound = false;
  char* p = NULL;

  for (size_t i = 0; i < dependencies.size(); i++) {
    p = strstr((char*)dependencies[i].c_str(), "WEB-INF/lib/spring-core");
    if (NULL != p) {
      isSpringCoreFound = true;
    }
    p = strstr((char*)dependencies[i].c_str(), "WEB-INF/lib/spring-webflux");
    if (NULL != p) {
      isSpringWebfluxFound = true;
    }
    p = strstr((char*)dependencies[i].c_str(), "WEB-INF/lib/spring-webmvc");
    if (NULL != p) {
      isSpringWebmvcFound = true;
    }
  }

  return isSpringCoreFound && (isSpringWebfluxFound || isSpringWebmvcFound);
}

bool IsCVE202222963Mitigated(CJavaManifest& javaManifest) {
  int major = 0, minor = 0, build = 0;
  if (ParseVersion(javaManifest.version, major, minor, build)) {
    if ((major == 3) && (minor == 1) && (build >= 7)) return true;
    if ((major == 3) && (minor == 2) && (build >= 3)) return true;
    if ((major == 3) && (minor >= 3)) return true;
  }
  return false;
}


bool IsCVE202222965Mitigated(CJavaManifest& javaManifest, std::vector<std::string>& dependencies) {
  std::string version;
  char* p = NULL;
  int major = 0, minor = 0, build = 0;

  for (size_t i = 0; i < dependencies.size(); i++) {
    p = strstr((char*)dependencies[i].c_str(), "WEB-INF/lib/spring-core-");
    if (NULL != p) {
      version = dependencies[i].substr(strlen("WEB-INF/lib/spring-core-"));
    }
  }

  if (ParseVersion(version, major, minor, build)) {
    if ((major == 5) && (minor == 2) && (build >= 20)) return true;
    if ((major == 5) && (minor == 3) && (build >= 18)) return true;
    if ((major == 5) && (minor >= 4)) return true;
  }

  return false;
}


int32_t ParseJavaManifest(std::string manifest, CJavaManifest& javaManifest) {
  int32_t rv = ERROR_SUCCESS;
  bool    found = false;

  SanitizeContents(manifest);

  found = GetDictionaryValue(manifest, "Implementation-Title:", "Unknown", javaManifest.title);
  if (!found) {
    GetDictionaryValue(manifest, "Bundle-Title:", "Unknown", javaManifest.title);
  }

  found = GetDictionaryValue(manifest, "Implementation-Vendor-Id:", "Unknown", javaManifest.vendor);
  if (!found) {
    found = GetDictionaryValue(manifest, "Implementation-Vendor:", "Unknown", javaManifest.vendor);
    if (!found) {
      GetDictionaryValue(manifest, "Bundle-Vendor:", "Unknown", javaManifest.vendor);
    }
  }

  found = GetDictionaryValue(manifest, "Implementation-Version:", "Unknown", javaManifest.version);
  if (!found) {
    GetDictionaryValue(manifest, "Bundle-Version:", "Unknown", javaManifest.version);
  }

  GetDictionaryValue(manifest, "Built-By:", "", javaManifest.builtBy);
  GetDictionaryValue(manifest, "Created-By:", "", javaManifest.createdBy);
  GetDictionaryValue(manifest, "Build-Jdk:", "", javaManifest.buildJDK);

  GetDictionaryValue(manifest, "Main-Class:", "", javaManifest.mainClass);
  GetDictionaryValue(manifest, "Start-Class:", "", javaManifest.startClass);

  GetDictionaryValue(manifest, "Spring-Boot-Version:", "", javaManifest.springBootVersion);
  GetDictionaryValue(manifest, "Spring-Boot-Classes:", "", javaManifest.springBootClasses);
  GetDictionaryValue(manifest, "Spring-Boot-Lib:", "", javaManifest.springBootLibs);

  StripWhitespace(javaManifest.title);
  StripWhitespace(javaManifest.vendor);
  StripWhitespace(javaManifest.version);
  StripWhitespace(javaManifest.builtBy);
  StripWhitespace(javaManifest.createdBy);
  StripWhitespace(javaManifest.buildJDK);
  StripWhitespace(javaManifest.mainClass);
  StripWhitespace(javaManifest.startClass);
  StripWhitespace(javaManifest.springBootVersion);
  StripWhitespace(javaManifest.springBootClasses);
  StripWhitespace(javaManifest.springBootLibs);

  return rv;
}

int32_t ProcessJavaFileWAR(CScannerOptions& options, std::wstring file, std::wstring file_physical) {
  int32_t                   rv = ERROR_SUCCESS;
  unzFile                   zf = NULL;
  unz_file_info64           file_info;
  char*                     p = NULL;
  char                      filename[_MAX_PATH + 1];
  std::wstring              wFilename;
  std::wstring              tmpFilename;
  bool                      foundManifest = false;
  std::string               manifest;
  CJavaManifest             javaManifest;
  std::vector<std::string>  dependencies;
  std::vector<std::wstring> wdependencies;
  bool springFrameworkDetected = false;
  bool cve202222965Mitigated = false;

  zlib_filefunc64_def zfm = { 0 };
  fill_win32_filefunc64W(&zfm);

  if (!file_physical.empty()) {
    zf = unzOpen2_64(file_physical.c_str(), &zfm);
  } else {
    zf = unzOpen2_64(file.c_str(), &zfm);
  }
  if (NULL != zf) {
    rv = unzGoToFirstFile(zf);
    if (UNZ_OK == rv) {
      do {
        rv = unzGetCurrentFileInfo64(zf, &file_info, filename, _countof(filename), NULL, 0, NULL, 0);

        if (UNZ_OK == rv) {
          if (0 == stricmp(filename, "META-INF/MANIFEST.MF")) {
            foundManifest = true;
            UncompressZIPContentsToString(zf, manifest);
          }
          p = strstr(filename, "WEB-INF/lib/spring-beans");
          if (NULL != p) {
            dependencies.push_back(filename);
          }
          p = strstr(filename, "WEB-INF/lib/spring-boot");
          if (NULL != p) {
            dependencies.push_back(filename);
          }
          p = strstr(filename, "WEB-INF/lib/spring-core");
          if (NULL != p) {
            dependencies.push_back(filename);
          }
          p = strstr(filename, "WEB-INF/lib/spring-webflux");
          if (NULL != p) {
            dependencies.push_back(filename);
          }
          p = strstr(filename, "WEB-INF/lib/spring-webmvc");
          if (NULL != p) {
            dependencies.push_back(filename);
          }

          wFilename = A2W(filename);
          if (IsKnownFileExtension(options.knownZipExtensions, wFilename)) {
            tmpFilename = GetTempporaryFilename();

            if (UncompressZIPContentsToFile(zf, tmpFilename)) {
              std::wstring masked_filename = file + L"!" + wFilename;
              std::wstring alternate_filename = tmpFilename;

              ScanFileZIPArchive(options, masked_filename, alternate_filename);
            }

            DeleteFile(tmpFilename.c_str());
          }

        }
        rv = unzGoToNextFile(zf);
      } while (UNZ_END_OF_LIST_OF_FILE != rv);
    }
    unzClose(zf);
  }
  rv = ERROR_SUCCESS;

  if (foundManifest) {
    std::string cveStatus = "Unknown";

    ParseJavaManifest(manifest, javaManifest);

    if (IsWARVunerable(dependencies)) {
      springFrameworkDetected = true;

      if (IsCVE202222965Mitigated(javaManifest, dependencies)) {
        cve202222965Mitigated = true;
        cveStatus = "Mitigated";
      } else {
        repSummary.foundVunerabilities++;
        cveStatus = "Potentially Vulnerable ( CVE-2022-22965: Found )";
      }

      for (size_t i = 0; i < dependencies.size(); i++) {
        wdependencies.push_back(A2W(dependencies[i].c_str()));
      }

      repVulns.push_back(CReportVulnerabilities(
        file, A2W(javaManifest.title), A2W(javaManifest.vendor), A2W(javaManifest.version), springFrameworkDetected, false,
        wdependencies, A2W(cveStatus), false, cve202222965Mitigated
      ));

      if (options.console) {
        wprintf(L"Spring-Framework Found: '%s' ( Manifest Title: %S, Manifest Vendor: %S, Manifest Version: %S, CVE Status: %S )\n",
                file.c_str(), javaManifest.title.c_str(), javaManifest.vendor.c_str(), javaManifest.version.c_str(), cveStatus.c_str());
      }
    }
  }

  return rv;
}


int32_t ProcessJavaFileEAR(CScannerOptions& options, std::wstring file, std::wstring file_physical) {
  int32_t                   rv = ERROR_SUCCESS;
  return rv;
}

int32_t ProcessJavaFileJAR(CScannerOptions& options, std::wstring file, std::wstring file_physical) {
  int32_t                   rv = ERROR_SUCCESS;
  unzFile                   zf = NULL;
  unz_file_info64           file_info;
  char*                     p = NULL;
  char                      filename[_MAX_PATH + 1];
  std::wstring              wFilename;
  std::wstring              tmpFilename;
  bool                      foundManifest = false;
  bool                      foundWAR = false;
  std::string               manifest;
  CJavaManifest             javaManifest;
  std::vector<std::wstring> wdependencies;
  bool springCloudDetected = false;
  bool cve202222963Mitigated = false;

  zlib_filefunc64_def zfm = { 0 };
  fill_win32_filefunc64W(&zfm);

  if (!file_physical.empty()) {
    zf = unzOpen2_64(file_physical.c_str(), &zfm);
  } else {
    zf = unzOpen2_64(file.c_str(), &zfm);
  }
  if (NULL != zf) {

    rv = unzGoToFirstFile(zf);
    if (UNZ_OK == rv) {
      do {
        rv = unzGetCurrentFileInfo64(zf, &file_info, filename, _countof(filename), NULL, 0, NULL, 0);

        if (UNZ_OK == rv) {
          if (0 == stricmp(filename, "META-INF/MANIFEST.MF")) {
            foundManifest = true;
            UncompressZIPContentsToString(zf, manifest);
          }

          wFilename = A2W(filename);
          if (IsKnownFileExtension(options.knownZipExtensions, wFilename)) {
            tmpFilename = GetTempporaryFilename();

            if (UncompressZIPContentsToFile(zf, tmpFilename)) {
              std::wstring masked_filename = file + L"!" + wFilename;
              std::wstring alternate_filename = tmpFilename;

              ScanFileZIPArchive(options, masked_filename, alternate_filename);
            }

            DeleteFile(tmpFilename.c_str());
          }

        }
        rv = unzGoToNextFile(zf);
      } while (UNZ_END_OF_LIST_OF_FILE != rv);
    }
    unzClose(zf);
  }
  rv = ERROR_SUCCESS;

  if (foundManifest) {
    std::string cveStatus = "Unknown";

    ParseJavaManifest(manifest, javaManifest);

    if (javaManifest.title == "Spring Cloud Function Core") {
      springCloudDetected = true;

      if (IsCVE202222963Mitigated(javaManifest)) {
        cve202222963Mitigated = true;
        cveStatus = "Mitigated";
      } else {
        repSummary.foundVunerabilities++;
        cveStatus = "Potentially Vulnerable ( CVE-2022-22963: Found )";
      }

      repVulns.push_back(CReportVulnerabilities(
        file, A2W(javaManifest.title), A2W(javaManifest.vendor), A2W(javaManifest.version), false, springCloudDetected,
        wdependencies, A2W(cveStatus), cve202222963Mitigated, false
      ));

      if (options.console) {
        wprintf(L"Spring Cloud Found: '%s' ( Manifest Title: %S, Manifest Vendor: %S, Manifest Version: %S, CVE Status: %S )\n",
                file.c_str(), javaManifest.title.c_str(), javaManifest.vendor.c_str(), javaManifest.version.c_str(), cveStatus.c_str());
      }
    }
  }

  return rv;
}
