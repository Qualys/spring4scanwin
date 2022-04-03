#pragma once

class CScannerOptions;

class CJavaManifest {
public:
  std::string               title;
  std::string               vendor;
  std::string               version;
  std::string               mainClass;
  std::string               startClass;
  std::string               createdBy;
  std::string               builtBy;
  std::string               buildJDK;
  std::string               springBootVersion;
  std::string               springBootClasses;
  std::string               springBootLibs;

  CJavaManifest() {
    title.clear();
    vendor.clear();
    version.clear();
    mainClass.clear();
    startClass.clear();
    createdBy.clear();
    builtBy.clear();
    buildJDK.clear();
    springBootVersion.clear();
    springBootClasses.clear();
    springBootLibs.clear();
  }
};


int32_t ParseJavaManifest(std::string manifest, CJavaManifest& javaManifest);

int32_t ProcessJavaFileJAR(CScannerOptions& options, std::wstring file, std::wstring file_physical);
int32_t ProcessJavaFileWAR(CScannerOptions& options, std::wstring file, std::wstring file_physical);
int32_t ProcessJavaFileEAR(CScannerOptions& options, std::wstring file, std::wstring file_physical);
