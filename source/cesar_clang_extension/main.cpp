// Declares clang::SyntaxOnlyAction.
//#include "clang/Frontend/FrontendActions.h"
//#include "clang/Tooling/CommonOptionsParser.h"
//#include "clang/Tooling/Tooling.h"

#include <clang-c/Index.h>
#include <cstdlib>
#include <iostream>

#include <boost/interprocess/managed_shared_memory.hpp>

using namespace boost::interprocess;

#if 0
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

int main(int argc, const char **argv) {



  // CommonOptionsParser constructor will parse arguments and create a
  // CompilationDatabase.  In case of error it will terminate the program.
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

  // Use OptionsParser.getCompilations() and OptionsParser.getSourcePathList()
  // to retrieve CompilationDatabase and the list of input file paths.

  // A clang tool can run over a number of sources in the same process...
  std::vector<std::string> Sources = OptionsParser.getSourcePathList();
//  Sources.push_back("a.cpp");

// We hand the CompilationDatabase we created and the sources to run over into
// the tool constructor.
  ClangTool Tool(OptionsParser.getCompilations(), Sources);

// The ClangTool needs a new FrontendAction for each translation unit we run
// on.  Thus, it takes a FrontendActionFactory as parameter.  To create a
// FrontendActionFactory from a given FrontendAction type, we call
// newFrontendActionFactory<clang::SyntaxOnlyAction>().
  int result = Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());

  return result;
}
#else
int main(int argc, const char **argv) {

  // hello interprocess
  managed_shared_memory segment(open_or_create, "MySharedMemory", 65536);

  if (argc < 4) {
    std::cout << argv[0] << " file.cc line colum [clang args...]"
    << std::endl;
    return 1;
  }

  CXIndex idx = clang_createIndex(1, 0);
  if (!idx) {
    std::cerr << "createIndex failed" << std::endl;
    return 2;
  }

  CXTranslationUnit u = clang_parseTranslationUnit(idx, argv[1], argv + 4,
                                                   argc - 4, 0, 0,
                                                   CXTranslationUnit_PrecompiledPreamble);

  if (!u) {
    std::cerr << "parseTranslationUnit failed" << std::endl;
    return 2;
  }

  clang_reparseTranslationUnit(u, 0, 0, 0);

  int line = strtol(argv[2], 0, 10);
  int column = strtol(argv[3], 0, 10);
  CXCodeCompleteResults* res = clang_codeCompleteAt(u, argv[1],
                                                    line, column,
                                                    0, 0, 0);
  if (!res) {
    std::cerr << "Could not complete" << std::endl;
    return 2;
  }

  for (unsigned i = 0; i < clang_codeCompleteGetNumDiagnostics(res); i++) {
    const CXDiagnostic& diag = clang_codeCompleteGetDiagnostic(res, i);
    const CXString& s = clang_getDiagnosticSpelling(diag);
    std::cout << clang_getCString(s) << std::endl;
  }

  for (unsigned i = 0; i < res->NumResults; i++) {
    const CXCompletionString& str = res->Results[i].CompletionString;

    for (unsigned j = 0; j < clang_getNumCompletionChunks(str); j++) {
      if (clang_getCompletionChunkKind(str, j) != CXCompletionChunk_TypedText)
        continue;

      const CXString& out = clang_getCompletionChunkText(str, j);
      std::cout << clang_getCString(out);
    }

    std::cout << std::endl;
  }
  clang_disposeCodeCompleteResults(res);
}

#endif
