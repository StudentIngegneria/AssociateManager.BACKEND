#include <iostream>
#include <cstdlib>

#include <Flags.hh>

#include "network.hpp"
#include "database/sqlite3/sqlite3-driver.hpp"

using namespace AssociateManager ;

int main( const int argc, char ** argv ) {

		std::string serverAddress = "localhost" ;
		uint64_t serverPort = 1234 ;
		std::string dbPath ;

		{
			bool displayHelp = false ;

			Flags cliParser ;
			cliParser.Var( dbPath, char(), "database", std::string("ams-database"), "The database file" ) ;
			cliParser.Var( serverAddress, char(), "ip-interface", std::string( "localhost" ) , "The ip address from which listen" ) ;
			cliParser.Var( serverPort, char(), "port", uint64_t(1234), "The network port" ) ;
			cliParser.Bool( displayHelp, 'h', "help", "Display the quick help" ) ;

			if( ! cliParser.Parse( argc, argv ) || displayHelp ) {
					cliParser.PrintHelp( argv[0] ) ;
					std::exit( EXIT_SUCCESS ) ;
				}
		}

		std::cerr << "Starting SI Associate Manager Server ( SI-AMS )\n" ;

		// Connect to the database
		std::cerr << "[INFO] Connecting to the database\n" ;
		//auto db_ptr = std::unique_ptr < DbInterface > ( new Sqlite3Driver( "database.sqlite", json() ) ) ;

		Sqlite3Driver db( dbPath, json() ) ;

		std::cerr << "[INFO] Setting up the HTTP server\n" ;
		Server server ;
		setupServer( server, db ) ;

		while( true ) {
			try {
					std::cerr << "[INFO] Serving network interface on "
						"http://" << serverAddress << ":" << serverPort << "\n" ;

					server.listen( serverAddress.c_str(), serverPort ) ;
				}
			catch( const std::exception & ) {
					std::cerr << "[EXCEPTIONAL_ERROR] A fatal error occurred. Restarting the server\n" ;
				}
			}
	}
