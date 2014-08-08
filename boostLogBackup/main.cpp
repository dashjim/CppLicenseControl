
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/locale.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace boost::locale;

int main(int argc, char* argv[])
{
directory_iterator end_iter;
directory_iterator file_itr;

string inputName;
string checkName;
inputName.assign(argv[1]);


if (is_directory(inputName))
{

    for (directory_iterator file_itr(inputName); file_itr != end_iter; ++file_itr)
    {
  
        string folderName(inputName);
        if (is_regular_file(*file_itr)) 
        {
            std::string fileType = file_itr->path().extension().string();
            string aFilePath(file_itr->path().string());
            string oldName(file_itr->path().filename().string());


            generator gen;
            std::locale::global(gen(""));

            date_time now;
            std::cout.imbue(std::locale());   
            std::ostringstream ostr;
            //ostr.imbue(loc);
            ostr << as::datetime << as::ftime("%Y%m%d%H%M%S") << now;
            std::string timeStr = "_"+ostr.str(); 
            cout <<timeStr << endl;

            //std::cout << as::ftime("%Y-%m-%d-%H-%M-%S") << now << std::endl;
            if(find_first(oldName, "_bak"))
            {

                replace_first(aFilePath, "_bak", timeStr);
                //RENAMING function
                rename(file_itr->path(), path(aFilePath.c_str()));
            }
                    
        }
    }
}
return 0;
}

