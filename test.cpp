#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <vector>

void encodeBuffered();
void decodeBuffered();
void encodeStream();
void decodeStream();

constexpr std::array<unsigned int, 5> POW85{ 85*85*85*85, 85*85*85, 85*85, 85, 1 };

TEST(Ascii85Test, EncodeDecodeBuffered) 
{
    std::string inputStr = "Man";
    std::istringstream input(inputStr);
    std::ostringstream encodedOutput;
    
    auto cinbuf = std::cin.rdbuf();
    auto coutbuf = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(encodedOutput.rdbuf());
    
    // Кодирование
    encodeBuffered();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::istringstream encodedInput(encodedOutput.str());
    std::ostringstream decodedOutput;
    std::cin.rdbuf(encodedInput.rdbuf());
    std::cout.rdbuf(decodedOutput.rdbuf());
    
    decodeBuffered();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    EXPECT_EQ(decodedOutput.str(), inputStr);
}

TEST(Ascii85Test, EncodeDecodeStream) {
    std::string inputStr = "TestData";
    std::istringstream input(inputStr);
    std::ostringstream encodedOutput;
    
    auto cinbuf = std::cin.rdbuf();
    auto coutbuf = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(encodedOutput.rdbuf());
    
    encodeStream();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::istringstream encodedInput(encodedOutput.str());
    std::ostringstream decodedOutput;
    std::cin.rdbuf(encodedInput.rdbuf());
    std::cout.rdbuf(decodedOutput.rdbuf());
    
    decodeStream();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    EXPECT_EQ(decodedOutput.str(), inputStr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}