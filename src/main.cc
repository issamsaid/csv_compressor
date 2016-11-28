///
/// @copyright Copyright (c) 2016-, Issam SAID <said.issam@gmail.com>
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the copyright holder nor the names of its
///    contributors may be used to endorse or promote products derived from
///    this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
/// HOLDER OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file csv_compressor/main.cc
/// @author Issam SAID
/// @brief The main routine that launches the csv compression on a file
///        passed by command line.
/// @details Usage: 
/// compress [option] inputFile outputFile:
/// * compress,   name of the executable.
/// * inputFile,  path to the input file.
/// * outputFile, path to the output file.
/// * option is:
///     -c or --compress,   compress the inputFile.
///     -d or --decompress, decompress the outputFile.
///     -h or --help,       show a help message.  
///
#include <iostream>
#include <uparser/uparser.h>

int main(int argc, char** argv) {

    bool compress, decompress;

    ///< Initialize uparser.
    uparser_init(argc, argv);

    ///< Populate the parser with options and arguments.
    uparser_opt('c', "compress", 
                "true", "compress inputFile into outputFile");
    uparser_opt('d', "decompress", 
                "false", "decompress inputFile into outputFile");
    uparser_arg("inputFile", 
                "the path of the input file to compress or decompress");
    uparser_arg("outputFile", 
                "the name of the output file");
    
    ///< Parse the command line arguments.
    uparser_parse();

    ///< Get values.
    uparser_get_bool(  "compress",   &compress);
    uparser_get_bool("decompress", &decompress);

    ///< Finalize uparser.
    uparser_release();

    return 0;
}