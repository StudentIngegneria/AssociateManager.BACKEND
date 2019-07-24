// Copyright (c) 2019 StudentIngegneria
//
// GNU AFFERO GENERAL PUBLIC LICENSE
//    Version 3, 19 November 2007
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include "driver.hpp"

static bool jsonHas(json j, std::string_view param) {
	return j.find( param ) != j.end() ;
}

#define PARAMETERS_HAS(x) jsonHas(parameters, x)

// Call Failure Is Not An Error
#define CFINAE_CALL( function, paramType, param ) \
	if( PARAMETERS_HAS( param ) )                   \
		return this-> function ( parameters[ param ].get < paramType > () ) ;

json DatabaseDriver::get(DBDataType type, json parameters) {
	switch (type) {

		case DBDataType::user :

			CFINAE_CALL( getUser, std::string_view, "username" ) ;
			break ;

		case DBDataType::session :

			CFINAE_CALL( getSession, std::string_view, "auth_token" ) ;
			break ;

		case DBDataType::action:

			CFINAE_CALL( getAction, std::string_view, "date" ) ;
			break ;

		case DBDataType::degree :

			CFINAE_CALL( getDegree, std::string_view, "id" ) ;
			break;

		case DBDataType::membership :

			if( PARAMETERS_HAS( "year" ) ) {

				if( PARAMETERS_HAS( "id" ) ) {
					return this->getMembership(
						parameters[ "year" ].get < int > () ,
						parameters[ "id"   ].get < std::string_view > ()
					);
				}

				if( PARAMETERS_HAS( "name" ) && PARAMETERS_HAS( "surname" ) )
					return this->getMembershipByName(
						parameters[ "year"    ].get < int > () ,
						parameters[ "name"    ].get < std::string_view > () ,
						parameters[ "surname" ].get < std::string_view > ()
					);

				if( PARAMETERS_HAS( "mail" ) )
					return this->getMembershipByMail(
						parameters[ "year" ].get < int > (),
						parameters[ "mail" ].get < std::string_view > ()
					);
			}
		}
		return json();
	}

json DatabaseDriver::getAll(DBDataType type, json parameters) {
	switch( type ) {

		case DBDataType::user :

			return this->getUsers() ;
			break ;

		case DBDataType::session :

			return this->getSessions() ;
			break ;

		case DBDataType::userSession :

			CFINAE_CALL( getUserSessions, std::string_view, "username" ) ;
			break ;

		case DBDataType::userAction :

			CFINAE_CALL( getUserActions, std::string_view, "username" ) ;
			break ;

		case DBDataType::degree :

			CFINAE_CALL( getDegrees, bool, "includeHistorical" ) ;
			break ;

		case DBDataType::registration :

			return this->getRegistrations() ;
			break ;

		case DBDataType::membership :

			CFINAE_CALL( getMemberships     , std::string_view, "year" ) ;
			CFINAE_CALL( getMembershipsByMail, std::string_view, "mail" ) ;

			if( PARAMETERS_HAS( "year" ) && PARAMETERS_HAS( "id_degree" ) )
				return this->getMembershipsByDegree(
					parameters[ "year"      ].get < int > () ,
					parameters[ "id_degree" ].get < std::string_view > ()
				) ;

			break;
	}

	return json::array() ;
}
