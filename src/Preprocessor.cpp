#include "Preprocessor.h"

using namespace ACC;

/* ---- Public ---- */

Preprocessor::Preprocessor(const SourceHandler &src) : Scanner(src), src(src) {}

SourceHandler Preprocessor::getSource() { return src; }