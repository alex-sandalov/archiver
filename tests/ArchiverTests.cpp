#include <ArgParser.h>
#include <HammingCode.h>
#include <Archive.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace ArgumentParser;

TEST(ArgParserTest, EmptyTest) {
    ArgParser parser("My Empty Parser");

	const char* argv[] = {"app"};
	int argc = 1;

    ASSERT_TRUE(parser.Parse(argv, argc));
}

TEST(ArgParserTest, StringTest) {
	ArgParser parser("parser");

	parser.AddStringArgument('f', "file");

	const char* argv[] = {"app", "--file=input.txt"};
	int argc = 2;
	
	ASSERT_TRUE(parser.Parse(argv, argc));

	EXPECT_EQ(parser.GetStringValue("file"), "input.txt");
}

TEST(ArgParserTest, ShortNameTest) {
	ArgParser parser("parser");

	parser.AddStringArgument('f', "file");

	const char* argv[] = {"app", "-f", "input.txt"};
	int argc = 2;

	ASSERT_TRUE(parser.Parse(argv, argc));

	EXPECT_EQ(parser.GetStringValue("file"), "input.txt");
}

TEST(ArgParserTest, BoolArgument) {
	ArgParser parser("parser");

	parser.AddFlag('f', "file");

	const char* argv[] = {"app", "-f", "input.txt"};
	int argc = 2;

	ASSERT_TRUE(parser.Parse(argv, argc));

	ASSERT_TRUE(parser.GetFlag("file"));
}

TEST(ArgParserTest, PositionalArguments) {
	ArgParser parser("parser");

	std::vector<std::string> arguments;

	parser.AddStringArgument('a', "add").MultiValue().StoreValues(arguments).Positional();
	parser.AddStringArgument('c', "comp").MultiValue().Positional();

	const char* argv[] = {"app", "-a", "input1.txt", "input2.txt"};
	int argc = 4;

	ASSERT_TRUE(parser.Parse(argv, argc));

	EXPECT_EQ(arguments[0], "input1.txt");
	EXPECT_EQ(arguments[1], "input2.txt");

}

TEST(HammingCodeTest, HammingCodeExtraBits) {
	std::vector<int> result = {2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5};
	for (int i = 0; i < result.size(); ++i) {
		EXPECT_EQ(result[i], HammingCodeExtraBits(i + 1));
	}
}

TEST(HammingCodeTest, EncodeHammingCode) {
	std::vector<uint8_t> value = {0b10110001, 0b10001011, 0b11111111, 0b11111111, 0b10101001, 0b01101001, 0b00000000, 0b00000000};
	std::vector<uint8_t> result = {0b10100111, 0b00011001, 0b01011111, 0b11111110, 0b11111110, 0b10100101, 0b10100100, 0b00000000, 0b00000000};
	std::vector<uint8_t> result_func = EncodeHamming(value, value.size() * 8);

	EXPECT_EQ(result.size(), result_func.size());

	for (int i = 0; i < result.size(); ++i) {
		EXPECT_EQ(result_func[i], result[i]);
	}
}

TEST(HammingCodeTest, DecodeHammingCode) {
	std::vector<uint8_t> value = {0b10100111, 0b00011001, 0b01011111, 0b11111110, 0b11111110, 0b10100101, 0b10100100, 0b00000000, 0b00000000};
	std::vector<uint8_t> result = {0b10110001, 0b10001011, 0b11111111, 0b11111111, 0b10101001, 0b01101001, 0b00000000, 0b00000000};
	std::vector<uint8_t> result_func = DecodeHamming(value, 8 * 8);
	EXPECT_EQ(result.size(), result_func.size());

	for (int i = 0; i < result.size(); ++i) {
		EXPECT_EQ(result_func[i], result[i]);
	}
}

TEST(HammingCodeTest, IsCorrectTrue) {
	std::vector<uint8_t> value = {0b10100111, 0b00011001, 0b01011111, 0b11111110, 0b11111110, 0b10100101, 0b10100100, 0b00000000, 0b0000000};
	ASSERT_TRUE(IsCorrect(value));
}

TEST(HammingCodeTest, IsCorrectFalse) {
	std::vector<uint8_t> value = {0b00100111, 0b00011001, 0b01011111, 0b11111110, 0b11111110, 0b10100101, 0b10100100, 0b00000000, 0b0000000};
	ASSERT_FALSE(IsCorrect(value));
}
