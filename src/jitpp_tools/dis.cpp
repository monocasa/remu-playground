#include "jitpp/arm/Disassembler.h"
#include "jitpp/ppc/Disassembler.h"

#include "binary/Binary.h"

#include <getopt.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

const char *binary_type_str = nullptr;
const char *machine_type_str = nullptr;

const char *binary_name = nullptr;

std::vector<std::string> symbol_files;

remu::binary::BinaryOptions binary_options = {
	.load_addr   = remu::binary::UNKNOWN_LOAD_ADDR,
	.entry_point = remu::binary::UNKNOWN_ENTRY_POINT,
	.machine     = remu::binary::MachineType::UNKNOWN,
};

std::unique_ptr<remu::jitpp::Disassembler> disassembler;

static void parse_args( int argc, char **argv )
{
	int option_index = 0;

	static struct option long_options[] = {
		{ "binary-type",  required_argument, 0, 'b' },
		{ "machine-type", required_argument, 0, 'm' },
		{ "symbol-file",  required_argument, 0, 's' },
	};

	while( true ) {
		int c = getopt_long(argc, argv, "b:m:", long_options, &option_index);

		if( c == -1 ) {
			break;
		}

		switch( c ) {
		case 'b': binary_type_str = optarg;       break;
		case 'm': machine_type_str = optarg;      break;
		case 's': symbol_files.push_back(optarg); break;

		case '?': exit( 1 );
		default:  exit( 1 );
		}
	}

	while( optind < argc ) {
		if( nullptr == binary_name ) {
			binary_name = argv[optind++];
		}
		else {
			fprintf( stderr, "Error:  Multiple binary files specified\n" );
			exit( 1 );
		}
	}
}

static void print_header( remu::binary::Binary* binary )
{
	printf( "\n" );
	printf( "%s:     file format %s\n", binary_name, binary->formatName().c_str() );
	printf( "\n" );	
}

static size_t count_zeros( const remu::util::memslice& slice )
{
	size_t num_zeros = 0;

	while( (num_zeros < slice.size) && (slice.data[num_zeros] == 0) ) {
		num_zeros++;
	}

	return num_zeros;
}

static void disassemble_file( remu::binary::Binary* binary )
{
	print_header( binary );

	auto segment_list = binary->getSectionSegments();

	auto symbol_db = binary->getSymbolDB();

	char dis[64];

	for( const auto& segment : segment_list ) {
		if( !segment.executable ) {
			continue;
		}

		printf( "\n" );
		printf( "Disassembly of section %s:\n", segment.name.c_str() );

		uint64_t cur_off = segment.base;
		const uint64_t end = segment.base + segment.size;
		std::vector<uint8_t> cur_section = binary->readSegment( segment );
		remu::util::memslice slice = { cur_section.data(), cur_section.size() };

		while( cur_off < end ) {
			const auto num_zeros = count_zeros( slice );

			if( count_zeros(slice) >= 8 ) {
				printf( "\t...\n" );
				const auto bytes_to_consume = num_zeros - (num_zeros % 4);

				cur_off += bytes_to_consume;

				slice.data += bytes_to_consume;
				slice.size -= bytes_to_consume;
			}
			else {
				const auto symbol = symbol_db.getSymbolExact( cur_off );
				if( symbol ) {
					printf( "\n" );
					printf( "%lx <%s>:\n", cur_off, symbol->name.c_str() );
				}
				else if( cur_off == segment.base ) {
					printf( "\n" );
					printf( "%lx <%s>:\n", cur_off, segment.name.c_str() );
				}

				size_t instr_size = disassembler->disassemble( slice, cur_off, dis, 64 );
				printf( "%lx:\t", cur_off );
				size_t ii = 0;
				for( ; ii < instr_size; ii++ ) {
					printf( "%02x ", slice.data[ii] );
				}
				for( ; ii < 4; ii++ ) {
					printf( "   " );
				}
				printf( "\t%s\n", dis );

				cur_off += instr_size;

				slice.data += instr_size;
				slice.size -= instr_size;
			}
		}
	}
}

int main( int argc, char **argv )
{
	parse_args( argc, argv );

	if( nullptr == binary_name ) {
		fprintf( stderr, "Error:  No binary specified\n" );
		exit( 1 );
	}

	remu::oshal::File file( binary_name );
	if( !file.isOpen() ) {
		fprintf( stderr, "Error:  Unable to open file \"%s\"\n", binary_name );
		exit( 1 );
	}

	auto binary = remu::binary::binaryForFile( file, binary_options );

	if( !binary ) {
		fprintf( stderr, "Error:  Unable to divine filetype for file \"%s\"\n", binary_name );
		exit( 1 );
	}

	if( binary->machineType() == remu::binary::MachineType::UNKNOWN ) {
	}

	switch( binary->machineType() ) {
		case remu::binary::MachineType::UNKNOWN: {
			fprintf( stderr, "Error: Unknown machine type in file \"%s\"\n", binary_name );
			exit( 1 );
		}
		break;

		case remu::binary::MachineType::PPC: {
			disassembler = std::unique_ptr<remu::jitpp::Disassembler>( new remu::jitpp::ppc::Disassembler() );
		}
		break;

		case remu::binary::MachineType::ARM: {
			disassembler = std::unique_ptr<remu::jitpp::Disassembler>( new remu::jitpp::arm::Disassembler() );
		}
		break;
	}

	disassemble_file( binary.get() );

	return 0;
}

