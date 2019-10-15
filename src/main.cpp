#include <iostream>
#include <cstdlib>

#include <Flags.hh>

int main( const int argc, char ** argv ) {

		std::string serverAddress = "localhost" ;
		uint64_t serverPort = 1234 ;

		{
			bool displayHelp = false ;

			Flags cliParser ;
			cliParser.Var( serverAddress, char(), "ip-interface", std::string( "localhost" ) , "The ip address from which listen" ) ;
			cliParser.Var( serverPort, char(), "port", uint64_t(1234), "The network port" ) ;
			cliParser.Bool( displayHelp, 'h', "help", "Display the quick help" ) ;

			if( ! cliParser.Parse( argc, argv ) || displayHelp ) {
					cliParser.PrintHelp( argv[0] ) ;
					std::exit( EXIT_SUCCESS ) ;
				}
		}
	}
