#pragma once

#include "SourceHandler.h"

#include "atl/include/ofstream.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

namespace ACC {

class LinkerBuilder final {

public:
  LinkerBuilder(const atl::shared_ptr<SourceHandler> &assembly,
                const atl::string &outFilename)
      : m_assembly(assembly), m_outFilename(outFilename) {}

  atl::string linkAndBuild() {
    const atl::string temp_s_filename = "temp.s";
    createTempAssemblyFile(temp_s_filename);

    const atl::string nasm_cmd = "nasm -g -f macho64  " + temp_s_filename;
    const int nasm_status = system(nasm_cmd.c_str());
    if (nasm_status != 0) {
      printf("Nasm Failed\n");
      throw;
    }
#ifdef __APPLE__
    const atl::string obj_filename = "temp.o";
    const atl::string ld_cmd = "ld -no_pie -macosx_version_min 10.15 "
                               "-L/Library/Developer/CommandLineTools/SDKs/"
                               "MacOSX.sdk/usr/lib -lSystem -o " +
                               m_outFilename + " " + obj_filename;
    const int ld_status = system(ld_cmd.c_str());
    if (ld_status != 0) {
      printf("ld Failed: `%s`\n", ld_cmd.c_str());
      throw;
    }
    
    { // Remove temp.s
      const atl::string delete_cmd = "rm ./" + temp_s_filename;
      const int delete_status = system(delete_cmd.c_str());
      if (delete_status != 0) {
        printf("Delete temp assembly file failed: `%s`\n", delete_cmd.c_str());
        throw;
      }
    }
    { // Remove temp.o
      const atl::string delete_cmd = "rm ./" + obj_filename;
      const int delete_status = system(delete_cmd.c_str());
      if (delete_status != 0) {
        printf("Delete temp object file failed: `%s`\n", delete_cmd.c_str());
        throw;
      }
    }
#else
    m_outFilename = "echo \"Cannot link and build on this platform\"";
#endif

    return m_outFilename;
  }

private:
  atl::shared_ptr<SourceHandler> m_assembly;
  atl::string m_outFilename;

  void createTempAssemblyFile(const atl::string &temp_s_filename) {
    atl::ofstream temp_s(temp_s_filename);
    if (!temp_s.good()) {
      const atl::string error =
          "Unable to create `" + temp_s_filename + "` file.";
      printf("%s\n", error.c_str());
      throw;
    }
    temp_s.write(m_assembly->read().c_str());
  }
};

} // namespace ACC
