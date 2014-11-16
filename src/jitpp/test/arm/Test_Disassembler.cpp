#include "jitpp/arm/Disassembler.h"

#include <gtest/gtest.h>

using remu::jitpp::arm::Disassembler;

namespace {

const int BUFFER_SIZE = 512;

const uint64_t DEFAULT_ADDR = 0;

} /*anonymous namespace*/

TEST(ArmDisassembler, Nop)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0x00000000, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "nop", buffer );
}

TEST(ArmDisassembler, Bx)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB12FFF15, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "bxlt     r5", buffer );
}

TEST(ArmDisassembler, Pld)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xf5d1f100, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "pld      [r1, #0x100]", buffer );
}

