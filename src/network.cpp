#include <charconv>
#include <optional>
#include <iostream>

#include "database/interface.hpp"
#include "network.hpp"

namespace AssociateManager {

		#define HTTPLIB_HANDLER_LAMBDA \
			[&]( const httplib::Request & req , httplib::Response & res ) -> void


		void reportMalformedPayload( httplib::Response & res ) {
				res.status = 400 ; // 400 Bad Request
				res.set_content( "Malformed payload", "text/plain" ) ;
			}

		// STUB FUNCTION
		bool checkAuth( const httplib::Request & req, httplib::Response & res, DbInterface & db ) {
				return true ;
			}

		void setupServer( Server & srv, DbInterface & db ) {
				std::vector < std::string > handlers = {
					"/startSession",
					"/db/getUser",
					"/db/getUsers",
					"/db/getSession",
					"/db/getSessions",
					"/db/getDegree",
					"/db/getDegrees",
					"/db/getRegistrationSeason",
					"/db/getRegistrationSeasons",
					"/db/getMembership",
					"/db/getMemberships",
					"/db/createAdmin",
					"/db/createDegree",
					"/db/createMember",
					"/db/createRegistration",
					"/db/deleteMember",
					"/db/closeRegistration"
				};

				for (std::string handler: handlers) {
					srv.Options(handler.c_str(), HTTPLIB_HANDLER_LAMBDA
					{
						res.status = 200;
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;
						res.headers.insert( std::make_pair( "Access-Control-Allow-Methods", "POST" ) ) ;
						res.headers.insert( std::make_pair( "Access-Control-Allow-Headers", "Content-Type") ) ;
						res.set_content("", "text/plain");
					});
				}


				// TODO: Improve invalid request handling
				auto safeParse = [&]( const std::string_view & s ) -> json
					{
						json j ;

						try { j = json::parse( s ) ; }
						catch ( const json::parse_error & ) {
								std::cerr << "[ERROR] Received invalid request from network. "
									<< "Expect a malformed payload response." << std::endl ;
							}

						return j ;
					} ;

				srv.Post( "/startSession", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						json reqPayload = safeParse( req.body ) ;

						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res ) ;
								return ;
							} ;

						if( ! reqPayload["username"].is_string() ) {
								reportMalformedPayload( res ) ;
								return ;
							}

						if( ! reqPayload["password"].is_string() ) {
								reportMalformedPayload( res ) ;
								return ;
							}

						// Retrive credentials
						auto username = reqPayload["username"].get < std::string > () ;
						auto password = reqPayload["password"].get < std::string > () ;

						// Search the specified username in the database
						json dbUser = db.getUser( username ) ;

						if( ! dbUser["username"].is_string() ) {
								res.status = 400 ; // 400 Bad Request
								return ;
							}

						auto passwordHash = dbUser["passwordHash"].get < std::string > () ;

						// Stub functions, TODO actually implement
						auto getHash = [&]( std::string )
							{ return db.getUser( username )["passwordHash"].get < std::string > () ; } ;

						auto genAuthToken = [](){ return "AVHJHGHDGGDHJ" ; } ;

						// Check password
						if( passwordHash != getHash( password ) ) {
								// TODO: Set appropriate headers
								res.status = 401 ; // 401 Unauthorized
								return ;
							}

						// If auth succeed register a new user session

						std::string authToken = genAuthToken() ;

						// db.setSession( username, authToken, keepAlive ) ;

						std::string authCookie( "authToken=" ) ; authCookie += authToken ;
						res.headers.insert( std::make_pair( "Set-Cookie", authCookie ) ) ;
					}) ;

				srv.Post( "/db/getUser", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						if( reqPayload["username"].empty() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						auto username = reqPayload["username"].get < std::string > () ;

						res.set_content( db.getUser( username ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getSession", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						if( reqPayload["authToken"].empty() ) {
								reportMalformedPayload( res ) ;
								return ;
							}

						auto authToken = reqPayload["auth_token"].get < std::string > () ;

						res.set_content( db.getSession( authToken ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getDegree", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						if( reqPayload["id"].empty() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						auto id = reqPayload["id"].get < std::string > () ;

						res.set_content( db.getDegree( id ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getRegistrationSeason", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						Year year ;
						if( reqPayload["year"].is_number_integer() )
								year = reqPayload["year"].get < Year > () ;
							else
								{
									reportMalformedPayload( res );
									return ;
								} ;

						res.set_content( db.getRegistrationSeason( year ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getMembership", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						Year year ; MembershipId id ;
						bool makeBrief = false ;

						if( reqPayload["year"].is_number_integer() )
								year = reqPayload["year"].get < Year > () ;
							else
								{
									reportMalformedPayload( res );
									return ;
								} ;

						if( reqPayload["id"].is_number_integer() )
								id = reqPayload["id"].get < MembershipId > () ;
							else
								{
									reportMalformedPayload( res );
									return ;
								} ;

						if( reqPayload["makeBrief"].is_boolean() )
								makeBrief = reqPayload["makeBrief"].get < bool > () ;

						res.set_content( db.getMembership( year, id, makeBrief ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getUsers", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;
						res.set_content( db.getUsers().dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getSessions", HTTPLIB_HANDLER_LAMBDA
					{
						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						std::optional < std::string > username = DbCallTags::anyUser ;

						if( reqPayload["username"].is_string() )
							username = reqPayload["username"].get < std::string > () ;

						res.set_content( db.getSessions( username ).dump(), "" ) ;
					}) ;

				srv.Post( "/db/getLog", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						std::optional < std::string > username  = DbCallTags::anyUser ;
						std::optional < Date        > startDate = DbCallTags::anyDate ;
						std::optional < Date        > endDate   = DbCallTags::anyDate ;

						if( reqPayload["username"].is_string() )
							username = reqPayload["username"].get < std::string > () ;

						if( reqPayload["startDate"].is_string() )
							startDate = reqPayload["startDate"].get < Date > () ;

						if( reqPayload["endDate"].is_string() )
							endDate = reqPayload["endDate"].get < Date > () ;

						res.set_content( db.getLog( username, startDate, endDate ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getDegrees", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						bool includeHistorical = false ;

						if( reqPayload["includeHistorical"].is_boolean() )
								includeHistorical = reqPayload["includeHistorical"].get < bool > () ;

						res.set_content( db.getDegrees( includeHistorical ).dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getRegistrationSeasons", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						res.set_content( db.getRegistrationSeasons().dump(), "application/json" ) ;
					}) ;

				srv.Post( "/db/getMemberships", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						std::optional < Year         > year       = DbCallTags::anyYear       ;
						std::optional < MembershipId > id         = DbCallTags::anyId         ;
						std::optional < std::string  > name       = DbCallTags::anyUser       ;
						std::optional < std::string  > surname    = DbCallTags::anyUser       ;
						std::optional < std::string  > degreeId   = DbCallTags::anyDegree     ;
						std::optional < std::string  > profession = DbCallTags::anyProfession ;
						std::optional < std::string  > mail       = DbCallTags::anyMail       ;
						bool makeBrief = false ;

						if( reqPayload["year"].is_number_integer() )
							year = reqPayload["year"].get < Year > () ;

						if( reqPayload["id"].is_number_integer() )
							id = reqPayload["id"].get < MembershipId > () ;

						if( reqPayload["name"].is_string() )
							name = reqPayload["name"].get < std::string > () ;

						if( reqPayload["surname"].is_string() )
							surname = reqPayload["surname"].get < std::string > () ;

						if( reqPayload["degreeId"].is_string() )
							degreeId = reqPayload["degreeId"].get < std::string > () ;

						if( reqPayload["profession"].is_string() )
							profession = reqPayload["profession"].get < std::string > () ;

						if( reqPayload["mail"].is_string() )
							mail = reqPayload["mail"].get < std::string > () ;

						if( reqPayload["makeBrief"].is_boolean() )
							makeBrief = reqPayload["makeBrief"].get < bool > () ;

						res.set_content(
								db.getMemberships(
										year, id, name, surname, degreeId,profession, mail
									).dump() ,

								"application/json"
							) ;
					}) ;

				srv.Post( "/db/createMember", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					if( ! checkAuth( req, res, db ) ) return ;

					json reqPayload = safeParse( req.body ) ;
					if( ! reqPayload.is_object() ) {
							reportMalformedPayload( res );
							return ;
						} ;

					Year year;
					MembershipId id;
					std::string uniNumber;
					std::string name;
					std::string surname;
					std::string mail;
					int quote;

					std::optional <std::string_view> degreeId = std::nullopt;
					std::optional <std::string_view> profession = std::nullopt;
					std::optional <std::string_view> phone = std::nullopt;

					if( reqPayload["year"].is_number_integer() )
							year = reqPayload["year"].get < Year > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["id"].is_number_integer() )
							id = reqPayload["id"].get < MembershipId > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["uniNumber"].is_string() )
							uniNumber = reqPayload["uniNumber"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["name"].is_string() )
							name = reqPayload["name"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["surname"].is_string() )
							surname = reqPayload["surname"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["mail"].is_string() )
							mail = reqPayload["mail"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["quote"].is_number_integer() )
							quote = reqPayload["quote"].get < Year > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["degreeId"].is_string() )
						degreeId = reqPayload["degreeId"].get < std::string > () ;

					if( reqPayload["profession"].is_string() )
						profession = reqPayload["profession"].get < std::string > () ;

					if( reqPayload["phone"].is_string() )
						phone = reqPayload["phone"].get < std::string > () ;

					res.set_content(
							db.createMember(
									year, id, uniNumber, name, surname, mail, quote, degreeId, profession, phone
								).dump() ,

							"application/json"
						) ;
				}) ;

				srv.Post( "/db/createRegistration", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					if( ! checkAuth( req, res, db ) ) return ;

					json reqPayload = safeParse( req.body ) ;
					if( ! reqPayload.is_object() ) {
							reportMalformedPayload( res );
							return ;
						} ;

					int year;
					std::string opening;

					if( reqPayload["year"].is_number_integer() )
							year = reqPayload["year"].get < Year > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["opening"].is_string() )
							opening = reqPayload["opening"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					res.set_content(
							db.createRegistration(
									year, opening
								).dump() ,

							"application/json"
						) ;
				});

				srv.Post( "/db/createAdmin", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					if( ! checkAuth( req, res, db ) ) return ;

					json reqPayload = safeParse( req.body ) ;
					if( ! reqPayload.is_object() ) {
							reportMalformedPayload( res );
							return ;
						} ;

					std::string username;
					std::string password;

					if( reqPayload["username"].is_string() )
							username = reqPayload["username"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["password"].is_string() )
							password = reqPayload["password"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					res.set_content(
							db.createAdmin(
									username, password
								).dump() ,

							"application/json"
						) ;
				});

				srv.Post( "/db/createDegree", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					if( ! checkAuth( req, res, db ) ) return ;

					json reqPayload = safeParse( req.body ) ;
					if( ! reqPayload.is_object() ) {
							reportMalformedPayload( res );
							return ;
						} ;

					std::string id;
					std::string degree;

					if( reqPayload["id"].is_string() )
							id = reqPayload["id"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["degree"].is_string() )
							degree = reqPayload["degree"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					res.set_content(
							db.createDegree(
									id, degree
								).dump() ,

							"application/json"
						) ;
				});

				srv.Post( "/db/createSession", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					res.set_content(
							"{\"ok\": \"ok\"}",

							"application/json"
						) ;
				});

				srv.Post( "/db/closeRegistration", HTTPLIB_HANDLER_LAMBDA
				{
					res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

					if( ! checkAuth( req, res, db ) ) return ;

					json reqPayload = safeParse( req.body ) ;
					if( ! reqPayload.is_object() ) {
							reportMalformedPayload( res );
							return ;
						} ;

					int year;
					std::string closing;

					if( reqPayload["year"].is_number_integer() )
							year = reqPayload["year"].get < int > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					if( reqPayload["closing"].is_string() )
							closing = reqPayload["closing"].get < std::string > () ;
						else
							{
								reportMalformedPayload( res );
								return ;
							} ;

					res.set_content(
							db.closeRegistration(
									year, closing
								).dump() ,

							"application/json"
						) ;
				});

				srv.Post( "/db/deleteMember", HTTPLIB_HANDLER_LAMBDA
					{
						res.headers.insert( std::make_pair( "Access-Control-Allow-Origin", "*" ) ) ;

						if( ! checkAuth( req, res, db ) ) return ;

						json reqPayload = safeParse( req.body ) ;
						if( ! reqPayload.is_object() ) {
								reportMalformedPayload( res );
								return ;
							} ;

						Year year ; MembershipId id ;

						if( reqPayload["year"].is_number_integer() )
								year = reqPayload["year"].get < Year > () ;
							else
								{
									reportMalformedPayload( res );
									return ;
								} ;

						if( reqPayload["id"].is_number_integer() )
								id = reqPayload["id"].get < MembershipId > () ;
							else
								{
									reportMalformedPayload( res );
									return ;
								} ;

						res.set_content( db.deleteMember( year, id ).dump(), "application/json" ) ;
					}) ;

			}

		#undef HTTPLIB_HANDLER_LAMBDA
	}
