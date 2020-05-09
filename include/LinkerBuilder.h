#pragma once

#include "SourceHandler.h"

#include "atl/include/ofstream.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

namespace ACC {

class LinkerBuilder final {

public:
  LinkerBuilder(const atl::shared_ptr<SourceHandler> &assembly, const atl::string &outFilename)
    : m_assembly(assembly),
      m_outFilename(outFilename) {}

  atl::shared_ptr<SourceFileHandler> linkAndBuild() {
    const atl::string temp_s_filename = "temp.s";
    createTempAssemblyFile(temp_s_filename);

    const atl::string nasm_cmd = "nasm -f macho64 " + temp_s_filename;
    const int nasm_status = system(nasm_cmd.c_str());
    if (nasm_status != 0) {
      printf("Nasm Failed\n");
      throw;
    }
    const atl::string ld_cmd = "ld -no_pie -macosx_version_min 10.15 -lSystem -o " + m_outFilename + " temp.o";
    const int ld_status = system(ld_cmd.c_str());
    if (ld_status != 0) {
      printf("ld Failed: `%s`\n", ld_cmd.c_str());
      throw;
    }

    return atl::shared_ptr<ACC::SourceFileHandler>(new SourceFileHandler(m_outFilename));
  }

private:
  atl::shared_ptr<SourceHandler> m_assembly;
  atl::string m_outFilename;

  void createTempAssemblyFile(const atl::string &temp_s_filename) {
    atl::ofstream temp_s(temp_s_filename);
    if (!temp_s.good()) {
      const atl::string error = "Unable to create `" + temp_s_filename + "` file." ;
      printf("%s\n", error.c_str());
      throw;
    }
    temp_s.write(m_assembly->read().c_str());
  }
};

} // namespace ACC
