/* 
 * npaextract: .npa file extraction utility
 * Copyright (C) 2013-2014 Mislav Blažević <krofnica996@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include "npafile.hpp"

#include <iostream>
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace boost::locale;
using namespace boost::locale::conv;

void WriteFile(char* Data, uint32_t Size, std::string FileName)
{
    // Create directories
    std::string Path = FileName;
    char* delim = strchr((char*)FileName.c_str(), '/');
    do
    {
        *delim = 0;
        if (!exists(path(Path)))
            create_directory(path(Path));
        *delim = '/';
    } while ((delim = strchr(delim + 1, '/')));

    // Create file
    std::ofstream Out(FileName, std::ios::binary | std::ios::out);
    Out.write(Data, Size);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <file.npa>" << std::endl;
        return 1;
    }

    std::locale loc = generator().generate("ja_JP.SHIFT-JIS");

    NpaFile Achieve(argv[1], NPA_READ);
    for (NpaIterator File = Achieve.Begin(); File != Achieve.End(); ++File)
    {
        std::string FileName = to_utf<char>(File.GetFileNameRaw(), File.GetFileNameRaw() + File.GetFileNameSize(), loc);
        std::cout << "Writing file: " << FileName << "..." << std::endl;
        char* Data = File.GetFileData();
        WriteFile(Data, File.GetFileSize(), FileName);
        delete Data;
    }
}
