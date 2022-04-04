
#include "stdafx.h"
#include "Utils.h"
#include "Reports.h"
#include "Version.info"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;
using WriterW = rapidjson::Writer<rapidjson::StringBuffer, rapidjson::UTF16<>, rapidjson::UTF8<>>;
using PrettyWriterW = rapidjson::PrettyWriter<rapidjson::StringBuffer, rapidjson::UTF16<>, rapidjson::UTF8<>>;


CReportSummary repSummary;
std::vector<CReportVulnerabilities> repVulns;


int32_t ReportProcessJARFile() {
  repSummary.scannedJARs++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessWARFile() {
  repSummary.scannedWARs++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessEARFile() {
  repSummary.scannedEARs++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessTARFile() {
  repSummary.scannedTARs++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessCompressedFile() {
  repSummary.scannedCompressed++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessDirectory() {
  repSummary.scannedDirectories++;
  return ERROR_SUCCESS;
}

int32_t ReportProcessFile() {
  repSummary.scannedFiles++;
  return ERROR_SUCCESS;
}

int32_t GenerateReportSummary(DocumentW& doc) {
  int32_t rv = ERROR_SUCCESS;

  ValueW vScanEngine(rapidjson::kStringType);
  ValueW vScanHostname(rapidjson::kStringType);
  ValueW vScanDate(rapidjson::kStringType);
  ValueW vScanDuration(rapidjson::kNumberType);
  ValueW vScanErrorCount(rapidjson::kNumberType);
  ValueW vScanStatus(rapidjson::kStringType);
  ValueW vScannedFiles(rapidjson::kNumberType);
  ValueW vScannedDirectories(rapidjson::kNumberType);
  ValueW vScannedJARs(rapidjson::kNumberType);
  ValueW vScannedWARs(rapidjson::kNumberType);
  ValueW vScannedEARs(rapidjson::kNumberType);
  ValueW vScannedPARs(rapidjson::kNumberType);
  ValueW vScannedTARs(rapidjson::kNumberType);
  ValueW vScannedCompressed(rapidjson::kNumberType);
  ValueW vVulnerabilitiesFound(rapidjson::kNumberType);
  ValueW vExcludedDrives(rapidjson::kArrayType);
  ValueW vExcludedDirectories(rapidjson::kArrayType);
  ValueW vExcludedFiles(rapidjson::kArrayType);
  ValueW vKnownTarExtensions(rapidjson::kArrayType);
  ValueW vKnownGZipTarExtensions(rapidjson::kArrayType);
  ValueW vKnownBZipTarExtensions(rapidjson::kArrayType);
  ValueW vKnownZipExtensions(rapidjson::kArrayType);
  ValueW oSummary(rapidjson::kObjectType);

  vScanEngine.SetString(A2W(SCANNER_VERSION_STRING).c_str(), doc.GetAllocator());
  vScanHostname.SetString(GetHostName().c_str(), doc.GetAllocator());
  vScanDate.SetString(FormatLocalTime(repSummary.scanStart).c_str(), doc.GetAllocator());
  vScanDuration.SetInt64(repSummary.scanEnd - repSummary.scanStart);
  vScanErrorCount.SetInt64(repSummary.scanErrorCount);
  vScanStatus.SetString(repSummary.scanStatus.c_str(), doc.GetAllocator());
  vScannedFiles.SetInt64(repSummary.scannedFiles);
  vScannedDirectories.SetInt64(repSummary.scannedDirectories);
  vScannedJARs.SetInt64(repSummary.scannedJARs);
  vScannedWARs.SetInt64(repSummary.scannedWARs);
  vScannedEARs.SetInt64(repSummary.scannedEARs);
  vScannedTARs.SetInt64(repSummary.scannedTARs);
  vScannedCompressed.SetInt64(repSummary.scannedCompressed);
  for (size_t i = 0; i < repSummary.excludedDrives.size(); ++i) {
    ValueW vDrive(repSummary.excludedDrives[i].c_str(), doc.GetAllocator());
    vExcludedDrives.PushBack(vDrive, doc.GetAllocator());
  }
  for (size_t i = 0; i < repSummary.excludedDirectories.size(); ++i) {
    ValueW vDirectory(repSummary.excludedDirectories[i].c_str(), doc.GetAllocator());
    vExcludedDirectories.PushBack(vDirectory, doc.GetAllocator());
  }
  for (size_t i = 0; i < repSummary.excludedFiles.size(); ++i) {
    ValueW vFile(repSummary.excludedFiles[i].c_str(), doc.GetAllocator());
    vExcludedFiles.PushBack(vFile, doc.GetAllocator());
  }
  for (const auto &ext : repSummary.knownTarExtensions) {
    ValueW vExtension(ext.c_str(), doc.GetAllocator());
    vKnownTarExtensions.PushBack(vExtension, doc.GetAllocator());
  }
  for (const auto& ext : repSummary.knownGZipTarExtensions) {
    ValueW vExtension(ext.c_str(), doc.GetAllocator());
    vKnownGZipTarExtensions.PushBack(vExtension, doc.GetAllocator());
  }
  for (const auto& ext : repSummary.knownBZipTarExtensions) {
    ValueW vExtension(ext.c_str(), doc.GetAllocator());
    vKnownBZipTarExtensions.PushBack(vExtension, doc.GetAllocator());
  }
  for (const auto& ext : repSummary.knownZipExtensions) {
    ValueW vExtension(ext.c_str(), doc.GetAllocator());
    vKnownZipExtensions.PushBack(vExtension, doc.GetAllocator());
  }
  vVulnerabilitiesFound.SetInt64(repSummary.foundVunerabilities);

  oSummary.AddMember(L"scanEngine", vScanEngine, doc.GetAllocator());
  oSummary.AddMember(L"scanHostname", vScanHostname, doc.GetAllocator());
  oSummary.AddMember(L"scanDate", vScanDate, doc.GetAllocator());
  oSummary.AddMember(L"scanDurationSeconds", vScanDuration, doc.GetAllocator());
  oSummary.AddMember(L"scanErrorCount", vScanErrorCount, doc.GetAllocator());
  oSummary.AddMember(L"scanStatus", vScanStatus, doc.GetAllocator());
  oSummary.AddMember(L"scannedFiles", vScannedFiles, doc.GetAllocator());
  oSummary.AddMember(L"scannedDirectories", vScannedDirectories, doc.GetAllocator());
  oSummary.AddMember(L"scannedJARs", vScannedJARs, doc.GetAllocator());
  oSummary.AddMember(L"scannedWARs", vScannedWARs, doc.GetAllocator());
  oSummary.AddMember(L"scannedEARs", vScannedEARs, doc.GetAllocator());
  oSummary.AddMember(L"scannedTARs", vScannedTARs, doc.GetAllocator());
  oSummary.AddMember(L"scannedCompressed", vScannedCompressed, doc.GetAllocator());
  oSummary.AddMember(L"excludedDrives", vExcludedDrives, doc.GetAllocator());
  oSummary.AddMember(L"excludedDirectories", vExcludedDirectories, doc.GetAllocator());
  oSummary.AddMember(L"excludedFiles", vExcludedFiles, doc.GetAllocator());
  oSummary.AddMember(L"knownTarExtensions", vKnownTarExtensions, doc.GetAllocator());
  oSummary.AddMember(L"knownGZipTarExtensions", vKnownGZipTarExtensions, doc.GetAllocator());
  oSummary.AddMember(L"knownBZipTarExtensions", vKnownBZipTarExtensions, doc.GetAllocator());
  oSummary.AddMember(L"knownZipExtensions", vKnownZipExtensions, doc.GetAllocator());
  oSummary.AddMember(L"vulnerabilitiesFound", vVulnerabilitiesFound, doc.GetAllocator());

  doc.AddMember(L"scanSummary", oSummary, doc.GetAllocator());

  return rv;
}

int32_t GenerateReportDetail(DocumentW& doc) {
  int32_t rv = ERROR_SUCCESS;
  ValueW oDetails(rapidjson::kArrayType);

  for (size_t i = 0; i < repVulns.size(); i++) {
    CReportVulnerabilities vuln = repVulns[i];

    ValueW vFile(rapidjson::kStringType);
    ValueW vManifestTitle(rapidjson::kStringType);
    ValueW vManifestVendor(rapidjson::kStringType);
    ValueW vManifestVersion(rapidjson::kStringType);
    ValueW vDetectedSpringFramework(rapidjson::kTrueType);
    ValueW vDetectedSpringCloud(rapidjson::kTrueType);
    ValueW vDependencies(rapidjson::kArrayType);
    ValueW vCVE202222963Mitigated(rapidjson::kTrueType);
    ValueW vCVE202222965Mitigated(rapidjson::kTrueType);
    ValueW vCVEStatus(rapidjson::kStringType);
    ValueW oDetail(rapidjson::kObjectType);

    vFile.SetString(vuln.file.c_str(), doc.GetAllocator());
    vManifestVendor.SetString(vuln.manifestTitle.c_str(), doc.GetAllocator());
    vManifestVendor.SetString(vuln.manifestVendor.c_str(), doc.GetAllocator());
    vManifestVersion.SetString(vuln.manifestVersion.c_str(), doc.GetAllocator());
    vDetectedSpringFramework.SetBool(vuln.detectedSpringFramework);
    vDetectedSpringCloud.SetBool(vuln.detectedSpringCloud);
    for (const auto& depend : vuln.dependencies) {
      ValueW vDepend(depend.c_str(), doc.GetAllocator());
      vDependencies.PushBack(vDepend, doc.GetAllocator());
    }
    vCVE202222963Mitigated.SetBool(vuln.cve202222963Mitigated);
    vCVE202222965Mitigated.SetBool(vuln.cve202222965Mitigated);
    vCVEStatus.SetString(vuln.cveStatus.c_str(), doc.GetAllocator());

    oDetail.AddMember(L"file", vFile, doc.GetAllocator());
    oDetail.AddMember(L"manifestTitle", vManifestTitle, doc.GetAllocator());
    oDetail.AddMember(L"manifestVendor", vManifestVendor, doc.GetAllocator());
    oDetail.AddMember(L"manifestVersion", vManifestVersion, doc.GetAllocator());
    oDetail.AddMember(L"detectedSpringFramework", vDetectedSpringFramework, doc.GetAllocator());
    oDetail.AddMember(L"detectedSpringCloud", vDetectedSpringCloud, doc.GetAllocator());
    oDetail.AddMember(L"dependencies", vDependencies, doc.GetAllocator());
    oDetail.AddMember(L"cve202222963Mitigated", vCVEStatus, doc.GetAllocator());
    oDetail.AddMember(L"cve202222965Mitigated", vCVEStatus, doc.GetAllocator());
    oDetail.AddMember(L"cveStatus", vCVEStatus, doc.GetAllocator());

    oDetails.PushBack(oDetail, doc.GetAllocator());
  }

  doc.AddMember(L"scanDetails", oDetails, doc.GetAllocator());
  return rv;
}

int32_t GenerateJSONReport(bool pretty) {
  int32_t rv = ERROR_SUCCESS;
  DocumentW doc;
  rapidjson::StringBuffer buffer;

  doc.Parse(L"{}");

  GenerateReportSummary(doc);
  GenerateReportDetail(doc);

  if (pretty) {
    PrettyWriterW writer(buffer);
    doc.Accept(writer);
  } else {
    WriterW writer(buffer);
    doc.Accept(writer);
  }

  wprintf(L"%S", buffer.GetString());
  return rv;
}

int32_t GenerateSignatureReport() {
  int32_t rv = ERROR_SUCCESS;

  FILE* signature_summary = nullptr;
  _wfopen_s(&signature_summary, GetSignatureReportSummaryFilename().c_str(), L"w+, ccs=UTF-8");
  if (signature_summary) {
    fwprintf_s(signature_summary, L"scanEngine: %S\n", SCANNER_VERSION_STRING);
    fwprintf_s(signature_summary, L"scanHostname: %s\n", GetHostName().c_str());
    fwprintf_s(signature_summary, L"scanDate: %s\n", FormatLocalTime(repSummary.scanStart).c_str());
    fwprintf_s(signature_summary, L"scanDurationSeconds: %I64d\n", repSummary.scanEnd - repSummary.scanStart);
    fwprintf_s(signature_summary, L"scanErrorCount: %I64d\n", repSummary.scanErrorCount);
    fwprintf_s(signature_summary, L"scanStatus: %s\n", repSummary.scanStatus.c_str());
    fwprintf_s(signature_summary, L"scanFiles: %I64d\n", repSummary.scannedFiles);
    fwprintf_s(signature_summary, L"scannedDirectories: %I64d\n", repSummary.scannedDirectories);
    fwprintf_s(signature_summary, L"scannedCompressed: %I64d\n", repSummary.scannedCompressed);
    fwprintf_s(signature_summary, L"scannedJARS: %I64d\n", repSummary.scannedJARs);
    fwprintf_s(signature_summary, L"scannedWARS: %I64d\n", repSummary.scannedWARs);
    fwprintf_s(signature_summary, L"scannedEARS: %I64d\n", repSummary.scannedEARs);
    fwprintf_s(signature_summary, L"scannedTARS: %I64d\n", repSummary.scannedTARs);
    for (size_t i = 0; i < repSummary.excludedDrives.size(); ++i) {
      fwprintf_s(signature_summary, L"excludedDrive: %s\n", repSummary.excludedDrives[i].c_str());
    }
    for (size_t i = 0; i < repSummary.excludedDirectories.size(); ++i) {
      fwprintf_s(signature_summary, L"excludedDirectory: %s\n", repSummary.excludedDirectories[i].c_str());
    }
    for (size_t i = 0; i < repSummary.excludedFiles.size(); ++i) {
      fwprintf_s(signature_summary, L"excludedFile: %s\n", repSummary.excludedFiles[i].c_str());
    }
    for (const auto& ext : repSummary.knownTarExtensions) {
      fwprintf_s(signature_summary, L"knownTarExtensions: %s\n", ext.c_str());
    }
    for (const auto& ext : repSummary.knownGZipTarExtensions) {
      fwprintf_s(signature_summary, L"knownGZipTarExtensions: %s\n", ext.c_str());
    }
    for (const auto& ext : repSummary.knownBZipTarExtensions) {
      fwprintf_s(signature_summary, L"knownBZipTarExtensions: %s\n", ext.c_str());
    }
    for (const auto& ext : repSummary.knownZipExtensions) {
      fwprintf_s(signature_summary, L"knownZipExtensions: %s\n", ext.c_str());
    }
    fwprintf_s(signature_summary, L"vulnerabilitiesFound: %I64d\n", repSummary.foundVunerabilities);
    fclose(signature_summary);
  } 

  FILE* signature_file = nullptr;
  _wfopen_s(&signature_file, GetSignatureReportFindingsFilename().c_str(), L"w+, ccs=UTF-8");
  if (signature_file) {
    for (size_t i = 0; i < repVulns.size(); i++) {
      CReportVulnerabilities vuln = repVulns[i];

      if (vuln.dependencies.size()) {
        for (size_t i = 0; i < vuln.dependencies.size(); i++) {
          fwprintf_s(signature_file, L"Path= %s!%s\n", vuln.file.c_str(), vuln.dependencies[i].c_str());
        }
      } else {
          fwprintf_s(signature_file, L"Path= %s, SpringCloudCore Version: %s\n", vuln.file.c_str(), vuln.manifestVersion.c_str());
      }

      fwprintf_s(signature_file, L"------------------------------------------------------------------------\n");
    }

    fclose(signature_file);
  } 

  return rv;
}
