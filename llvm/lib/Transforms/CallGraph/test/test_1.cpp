#include <gtest/gtest.h>
#include <gtest/gmock.h>
#include <fstream>
#include <algorithm>

bool areFilesEqual(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary); //open file at the end
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary); //open file at the end
    const std::ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg())
        return false; //different file size

    file1.seekg(0); //rewind
    file2.seekg(0); //rewind

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    return std::equal(begin1,std::istreambuf_iterator<char>(),begin2); //Second argument is end-of-range iterator
}

TEST(graphBuilding, 1) {
    system("/home/voffk4/llvmHuawei/llvm-practice/build/bin/clang++ 1.cpp ../Logger/Logger.cpp -o 1");
    
    ASSERT_EQ(areFilesEqual("Graph.txt", "GRAPTH_1.txt"), true);
    ASSERT_EQ(areFilesEqual("maps.txt", "MAPS_STANDART_1.txt"), true);

    system("home/voffk4/llvmHuawei/llvm-practice/elf-parser/tmp/CallGraph /home/voffk4/llvmHuawei/llvm-practice/llvm/lib/Transforms/CallGraph/test/1 /home/voffk4/llvmHuawei/llvm-practice/llvm/lib/Transforms/CallGraph/test/Graph.txt /home/voffk4/llvmHuawei/llvm-practice/llvm/lib/Transforms/CallGraph/test/maps.txt > syms.txt");
    ASSERT_EQ(areFilesEqual("dump.dot", "DOT_STANDART_1.dot"), true);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest (&argc, argv);
    testing::InitGoogleMock (&argc, argv);

    return RUN_ALL_TESTS();
}
