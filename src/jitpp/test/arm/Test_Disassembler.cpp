#include "jitpp/arm/Disassembler.h"

#include <gtest/gtest.h>

using remu::jitpp::arm::Disassembler;

namespace {

const int BUFFER_SIZE = 512;

const uint64_t DEFAULT_ADDR = 0;

} /*anonymous namespace*/

TEST(ArmDisassembler, nop)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0x00000000, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "nop", buffer );
}

TEST(ArmDisassembler, addimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xe2844001, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "add      r4, r4, #0x1", buffer );

	dis.disassemble(0xb295400F, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "addslt   r4, r5, #0xf", buffer );
}

TEST(ArmDisassembler, b)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xEA0006F8, 0x0001001C, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "b        loc_11c04", buffer );

	dis.disassemble(0xEAFFFFFE, 0x00010100, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "b        loc_10100", buffer );
}

TEST(ArmDisassembler, bicimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB3C01A07, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "biclt    r1, r0, #0x7000", buffer );
}

TEST(ArmDisassembler, bl)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xEB0002E0, 0x0001000C, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "bl       loc_10b94", buffer );

	dis.disassemble(0xEBFFF75D, 0x0001322C, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "bl       loc_10fa8", buffer );
}

TEST(ArmDisassembler, bx)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB12FFF15, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "bxlt     r5", buffer );
}

TEST(ArmDisassembler, cmpimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB3540096, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "cmplt    r4, #0x96", buffer );
}

TEST(ArmDisassembler, ldrimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xe5900000, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "ldr      r0, [r0]", buffer );

	dis.disassemble(0xe59f00fc, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "ldr      r0, [pc, #252]", buffer );
}

TEST(ArmDisassembler, mla)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0214392, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mlalt    r1, r2, r3, r4", buffer );
}

TEST(ArmDisassembler, mlas)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0314392, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mlaslt   r1, r2, r3, r4", buffer );
}

TEST(ArmDisassembler, movimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xE3A0D902, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mov      sp, #0x8000", buffer );
}

TEST(ArmDisassembler, movreg)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xE1A0D002, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mov      sp, r2", buffer );

	dis.disassemble(0xE1B0D002, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "movs     sp, r2", buffer );
}

TEST(ArmDisassembler, mul)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0010392, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mullt    r1, r2, r3", buffer );
}

TEST(ArmDisassembler, muls)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0110392, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "mulslt   r1, r2, r3", buffer );
}

TEST(ArmDisassembler, orrimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xC3811A02, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "orrgt    r1, r1, #0x2000", buffer );
}

TEST(ArmDisassembler, pld)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xf5d1f100, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "pld      [r1, #0x100]", buffer );
}

TEST(ArmDisassembler, stmfd)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0x192e4010, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "stmfdne  lr!, {r4, lr}", buffer );

	dis.disassemble(0x192d4010, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "pushne   {r4, lr}", buffer );

	dis.disassemble(0x190d4010, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "stmfdne  sp, {r4, lr}", buffer );
}

TEST(ArmDisassembler, strimm)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xe5801000, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "str      r1, [r0]", buffer );
}

TEST(ArmDisassembler, svc)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xAF123456, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "svcge    #0x123456", buffer );
}

TEST(ArmDisassembler, umull)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0821493, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "umulllt  r1, r2, r3, r4", buffer );
}

TEST(ArmDisassembler, umulls)
{
	char buffer[ BUFFER_SIZE ];
	Disassembler dis;

	dis.disassemble(0xB0921493, DEFAULT_ADDR, buffer, BUFFER_SIZE);

	EXPECT_STREQ( "umullslt r1, r2, r3, r4", buffer );
}

