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

// Тестируем буферный режим кодирования и обратное декодирование
TEST(Ascii85Test, EncodeDecodeBuffered) 
{
    // Исходные данные
    std::string inputStr = "Man";
    std::istringstream input(inputStr);
    std::ostringstream encodedOutput;
    
    // Перенаправляем потоки
    auto cinbuf = std::cin.rdbuf();
    auto coutbuf = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(encodedOutput.rdbuf());
    
    // Кодирование
    encodeBuffered();
    
    // Восстанавливаем потоки
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    // Декодирование
    std::istringstream encodedInput(encodedOutput.str());
    std::ostringstream decodedOutput;
    std::cin.rdbuf(encodedInput.rdbuf());
    std::cout.rdbuf(decodedOutput.rdbuf());
    
    decodeBuffered();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    // Ожидаем, что декодированный результат совпадает с исходными данными
    EXPECT_EQ(decodedOutput.str(), inputStr);
}

// Тестируем потоковый режим кодирования и обратное декодирование
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

//
// Точка входа для тестов Google Test
//
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}