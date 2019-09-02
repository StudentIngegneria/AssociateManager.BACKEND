/* Copyright (c) 2019 StudentIngegneria
 *
 * GNU AFFERO GENERAL PUBLIC LICENSE
 *    Version 3, 19 November 2007
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <optional>
#include <string_view>
#include <nlohmann/json.hpp>

namespace AssociateManager {

		using json = nlohmann::json ;

		using Date         = std::string_view ;
		using Year         = uint16_t         ;
		using MembershipId = uint16_t         ;

		/**
		 * A lvalue collection to make expressive function callsites
		 * @namespace DbCallTags
		 */
		namespace DbCallTags {

				/// Intended to be used in getDegrees() call sites
				static constexpr bool includeHistorical = true ;

				/// Intended to be used in membership-related function call sites
				static constexpr bool makeBrief = true ;

				/// Intended to be used in getSessions() and getLog() call sites
				static constexpr auto & anyUser = std::nullopt ;

				/// Intended to be used in getLog() call sites
				static constexpr auto & anyDate = std::nullopt ;

				///Intended to be used in getMemberships() call sites
				static constexpr auto & anyYear = std::nullopt ;

				///Intended to be used in getMemberships() call sites
				static constexpr auto & anyId = std::nullopt ;

				///Intended to be used in getMemberships() call sites
				static constexpr auto & anyDegree = std::nullopt ;

				///Intended to be used in getMemberships() call sites
				static constexpr auto & anyProfession = std::nullopt ;

				///Intended to be used in getMemberships() call sites
				static constexpr auto & anyMail = std::nullopt ;
			}

		/**
		 * The universal database interface
		 * All database drivers must inherit from this interface
		 * @struct DbInterface
		 */
		struct DbInterface {

				/**
				 * Close the db connection and cleanup
				 * @method DbInterface
				 */
				virtual ~DbInterface() = default ;

				/**
				 * Get details about a registered user
				 * @method getUser
				 * @param  username
				 * @return A json structure describing a user
				 */
				virtual json getUser( const std::string_view & username ) const = 0 ;

				/**
				 * Get details about an active user session
				 * @method getSession
				 * @param  authToken  An authentican token
				 * @return A json structure describing a session
				 */
				virtual json getSession( const std::string_view & authToken ) const = 0 ;

				/**
				 * Get details about a degree course
				 * @method getDegree
				 * @param  id        A degree course id
				 * @return A json structure describing a degree course
				 */
				virtual json getDegree( const std::string_view & id ) const = 0 ;

				/**
				 * Get details about a registration season
				 * @method getRegistrationSeason
				 * @param  year                  A registration season year
				 * @return A json structure describing a registration season
				 */
				virtual json getRegistrationSeason( const Year & year ) const = 0 ;

				/**
				 * Get details about a membership
				 * @method getMembership
				 * @param  year          A registration season year
				 * @param  id            A member id
				 * @param  makeBrief     A switch to control verbosity
				 * @return A json structure describing a membership
				 */
				virtual json getMembership(
						const Year         & year              ,
						const MembershipId & id                ,
						const bool         & makeBrief = false
					) const = 0 ;

				/**
				 * Get the list of the users registed in the system
				 * @method getUsers
				 * @return A json array of objects describing users
				 */
				virtual json getUsers() const = 0 ;

				/**
				 * Get the list of active user sessions
				 * Optionally filtered by username
				 * @method getSessions
				 * @param  username
				 * @return A json array of objects describing a session
				 */
				virtual json getSessions(
						const std::optional < std::string_view > & username = DbCallTags::anyUser
					) const = 0 ;

				/**
				 * Get a log of the actions executed on the database
				 * Arguments specify filters and are all optional
				 * @method getLog
				 * @param  username
				 * @param  startDate A partial ISO8601 timestamp
				 * @param  endDate   A partial ISO8601 timestamp
				 * @return A json array of objects describing actions
				 */
				virtual json getLog(
						const std::optional < std::string_view > & username  = DbCallTags::anyUser ,
						const std::optional < Date             > & startDate = DbCallTags::anyDate ,
						const std::optional < Date             > & endDate   = DbCallTags::anyDate
					) const = 0 ;

				/**
				 * Get the list of degrees known to the system
				 * @method getDegrees
				 * @param  includeHistorical A switch to control inclusion of courses that aren't taught anymore
				 * @return A json array of objects describing a degree course
				 */
				virtual json getDegrees( const bool & includeHistorical = false ) const = 0 ;

				/**
				 * Get the list of registration seasons known to the system
				 * @method getRegistrationSeasons
				 * @return A json array of objects describing a registration season
				 */
				virtual json getRegistrationSeasons() const = 0 ;

				/**
				 * Get a list of memberships matching the input criteria
				 * @method getMemberships
				 * @param  year           A registration season year
				 * @param  id             A member id
				 * @param  name           A member name
				 * @param  surname        A member surname
				 * @param  degreeId       A degree course id
				 * @param  profession     A profession
				 * @param  mail           An email address
				 * @param  makeBrief      A switch to control verbosity
				 * @return A json array of objects describing a membership
				 */
				virtual json getMemberships(
						const std::optional < Year             > & year       = DbCallTags::anyYear       ,
						const std::optional < MembershipId     > & id         = DbCallTags::anyId         ,
						const std::optional < std::string_view > & name       = DbCallTags::anyUser       ,
						const std::optional < std::string_view > & surname    = DbCallTags::anyUser       ,
						const std::optional < std::string_view > & degreeId   = DbCallTags::anyDegree     ,
						const std::optional < std::string_view > & profession = DbCallTags::anyProfession ,
						const std::optional < std::string_view > & mail       = DbCallTags::anyMail       ,
						const bool & makeBrief = false
					) const = 0 ;
			};

	}
