#pragma once

#include "atl/include/fstream.h"
#include "atl/include/string.h"

namespace ACC {

class SourceHandler {
public:
  virtual ~SourceHandler() {}

  virtual atl::string getFilepath() const = 0;
  virtual atl::string read() const = 0;
  virtual void write(const atl::string &line) = 0;
};

class SourceFileHandler : public SourceHandler {
private:
  atl::string m_filename;
  atl::fstream m_file_handle;

public:
  SourceFileHandler(const atl::string &filename, const atl::fstream::open_mode &mode = atl::fstream::open_mode::read)
    : m_filename(filename), m_file_handle(m_filename, mode) {
    if (!m_file_handle.good()) {
      printf("Unable to open file: %s\n", m_filename.c_str());
      throw;
    }
  }
  SourceFileHandler(const SourceFileHandler &rhs) = delete;

  atl::string getFilepath() const override {
    return m_filename;
  }
  atl::string read() const override {
    return m_file_handle.read();
  }
  void write(const atl::string &line) override {
    m_file_handle.write(line);
  }

};

class SourceMemHandler : public SourceHandler {
private:
  atl::string m_value;

public:
  SourceMemHandler() = default;
  SourceMemHandler(const atl::string &value) : m_value(value) {}
  
  atl::string getFilepath() const override {
    return "RAW";
  }
  atl::string read() const override { return m_value; }
  void write(const atl::string &value) override { m_value += value; }

};

} // namespace ACC
