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
#pragma once

// Includes
#include <string_view>
#include <nlohmann/json.hpp>

using json = nlohmann::json ;

enum class DBDataType {
	user, session, userSession, action, userAction, degree, registration, membership
} ;


class DatabaseDriver {

	public:
		/**
		 * Constructor for a DatabaseDriver
		 * Constructs the object and creates the connection if possible
		 * @param databaseAccessString The string representing the location of the database (i.e file location for sqlite3)
		 */
		explicit DatabaseDriver(std::string_view databaseAccessString) {};

		/**
		 * Closes the connection
		 */
		virtual void close() = 0;

		/**
		 * @param type The type of data to retrieve
		 * @param parameters JSON data to pass, see the various other functions
		 * @see DatabaseDriver
		 * @return A single element of the dataType
		 */
		json get(DBDataType type, json parameters);

		/**
		 * @param type The type of data to retrieve
		 * @param parameters JSON data to pass, see the various other functions
		 * @see DatabaseDriver
		 * @return A single element of the dataType
		 */
		json getAll(DBDataType type, json parameters);

	protected:

		/**
		 * @return List of all usernames
		 */
		virtual json getUsers() = 0 ;

		/**166:2
		 * @param username The user's username
		 * @return The user's data
		 */
		virtual json getUser(std::string_view username) = 0 ;

		/**
		 * @return The list of active sessions
		 */
		virtual json getSessions() = 0 ;

		/**
		 * @param auth_token The authentication token
		 * @return The user's data
		 */
		virtual json getSession(std::string_view auth_token) = 0 ;

		/**
		 * @param username The user's username
		 * @return The user's data
		 */
		virtual json getUserSessions(std::string_view username) = 0 ;

		/**
		 * @return The user's data
		 */
		virtual json getLog() = 0 ;

		/**
		 * @param date ISO8601 timestamp
		 * @return The action log in the specified time frame
		 */
		virtual json getAction(std::string_view date) = 0 ;

		/**
		 * @param username The user's username
		 * @return The action log filtered by user
		 */
		virtual json getUserActions(std::string_view username) = 0 ;

		/**
		 * @param includeHistorical Flag to enable inclusion of courses that aren't taught anymore
		 * @return A list of valid degree selections
		 */
		virtual json getDegrees(bool includeHistorical) = 0 ;

		/**
		 * @param id A degree course id
		 * @return A degree description
		 */
		virtual json getDegree(std::string_view id) = 0 ;

		/**
		 * @return A list of registrations
		 */
		virtual json getRegistrations() = 0 ;

		/**
		 * @param year A registration year
		 * @return A list of memberships
		 */
		virtual json getMemberships(std::string_view year) = 0 ;

		/**
		 * @param mail An e-mail address
		 * @return A list of memerships associated to the same e-mail address
		 */
		virtual json getMembershipsByMail(std::string_view mail) = 0 ;

		/**
		 * @param year A registration year
		 * @param id A member id
		 * @return A membership
		 */
		virtual json getMembership(short year, std::string_view id) = 0 ;

		/**
		 * @param year A registration year
		 * @param name A member's name
		 * @param surname A member's surname
		 * @return A membership
		 */
		virtual json getMembershipByName(short year, std::string_view name, std::string_view surname) = 0 ;

		/**
		 * @param year A registration year
		 * @param mail An e-mail address
		 * @return A membership
		 */
		virtual json getMembershipByMail(short year, std::string_view mail) = 0 ;

		/**
		 * @param year A registration year
		 * @param id_degree A degree i
		 * @return A list of memerships filtered by degree
		 */
		virtual json getMembershipsByDegree(short year, std::string_view id_degree) = 0 ;
};
