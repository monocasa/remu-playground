#include <getopt.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

const char *binary_type_str = nullptr;
const char *machine_type_str = nullptr;

const char *binary_name = nullptr;

enum MachineType
{
	TYPE_PPC,
	TYPE_ARM,
};

enum PpcFlags
{
};

enum ArmFlags
{
};

struct MachineOption
{
	const char *short_name;
	const char *long_name;
	int machine;
	uint64_t flags;
};

const MachineOption predefined_options[] = {
	"ppc", "PowerPC", TYPE_PPC, 0,
	"arm", "ARM",     TYPE_ARM, 0,
};

static void parse_args( int argc, char **argv )
{
	int option_index = 0;

	static struct option long_options[] = {
		{ "binary-type",  required_argument, 0, 'b' },
		{ "machine-type", required_argument, 0, 'm' },
	};

	while( true ) {
		int c = getopt_long(argc, argv, "b:m:", long_options, &option_index);

		if( c == -1 ) {
			break;
		}

		switch( c ) {
		case 'b': binary_type_str = optarg;  break;
		case 'm': machine_type_str = optarg; break;

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

const MachineOption* find_machine_option_for_str( const char *str )
{
	const int num_machine_types = sizeof(predefined_options) / sizeof(MachineOption);
	for( int ii = 0; ii < num_machine_types; ii++ ) {
		if( strcmp(str, predefined_options[ii].short_name) == 0 ) {
			return &predefined_options[ii];
		}
	}

	return nullptr;
}

void print_possible_machines()
{
	const int num_machine_types = sizeof(predefined_options) / sizeof(MachineOption);
	for( int ii = 0; ii < num_machine_types; ii++ ) {
		printf( "%10s - %s\n", predefined_options[ii].short_name, predefined_options[ii].long_name );
	}
}

int main( int argc, char **argv )
{
	parse_args( argc, argv );

	if( machine_type_str == nullptr ) {
		fprintf( stderr, "Error:  No machine type specified\n" );
		exit( 1 );
	}

	if( strcmp(machine_type_str, "?") == 0 ) {
		print_possible_machines();
		exit( 0 );
	}

	auto machine = find_machine_option_for_str( machine_type_str );

	if( nullptr == machine ) {
		fprintf( stderr, "Error:  Unknown machine type \"%s\"\n", machine_type_str );
		exit( 1 );
	}

	if( nullptr == binary_name ) {
		fprintf( stderr, "Error:  No binary specified\n" );
		exit( 1 );
	}

	printf( "Binary:  %s\n", binary_name );

	return 0;
}

