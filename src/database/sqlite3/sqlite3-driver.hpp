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
#include "../interface.hpp"
#include <sqlite_modern_cpp.h>

namespace AssociateManager {
    using json = nlohmann::json;
    using namespace sqlite;

		class Sqlite3Driver: public DbInterface{
			private:
				database db ;
				json dbSchema ;

      public:
        Sqlite3Driver(const std::string_view & filename, const json & dbSchema);

				json getUser( const std::string_view & username ) const override ;
				json getSession( const std::string_view & authToken ) const override ;
				json getDegree( const std::string_view & id ) const override ;
				json getRegistrationSeason( const Year & year ) const override ;
				json getMembership(
						const Year         & year              ,
						const MembershipId & id                ,
						const bool         & makeBrief = false
					) const override ;
				json getUsers() const override ;
				json getSessions(
						const std::optional < std::string_view > & username
					) const override ;
				json getLog(
						const std::optional < std::string_view > & username ,
						const std::optional < Date             > & startDate ,
						const std::optional < Date             > & endDate
					) const override ;
				json getDegrees( const bool & includeHistorical = false ) const override ;
				json getRegistrationSeasons() const override ;
				json getMemberships(
						const std::optional < Year             > & year       ,
						const std::optional < MembershipId     > & id         ,
						const std::optional < std::string_view > & name       ,
						const std::optional < std::string_view > & surname    ,
						const std::optional < std::string_view > & degreeId   ,
						const std::optional < std::string_view > & profession ,
						const std::optional < std::string_view > & mail       ,
						const bool & makeBrief
					) const override ;

          json createMember(
                const Year & year,
                const MembershipId & id,
                const std::string_view & uniNumber,
                const std::string_view & name,
                const std::string_view & surname,
                const std::string_view & mail,
                const int & quote,
                const std::optional < std::string_view > & degreeId,
                const std::optional < std::string_view > & profession,
                const std::optional < std::string_view > & phone
          ) const override;

          json createRegistration(const Year & year, const std::string_view & opening) const override;
          json createAdmin(const std::string_view & username, const std::string_view & password) const override;
          json createDegree(const std::string_view & id, const std::string_view & degree) const override;
          json createSession(const std::string_view & username) const override;

          json closeRegistration(const Year & year, const std::string_view & closing) const override;
          json deleteMember(const Year & year, const MembershipId & membershipId) const override;
			};

	}
