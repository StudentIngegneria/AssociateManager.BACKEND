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
#include <algorithm>
#include <optional>
#include <string_view>
#include <string>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <nlohmann/json.hpp>
#include "../interface.hpp"
#include "sqlite3-driver.hpp"
#include <sqlite_modern_cpp.h>

namespace AssociateManager {
	using json = nlohmann::json;
	using namespace sqlite;

	// DB SCHEMA MAY BE BLANK, BUT IN THE FUTURE, THIS FUNCTION WILL NEED AN ACTUAL DB
	// SCHEMA. WE'LL STILL PROVIDE A DEFAULT SCHEMA THAT MATCHES THE CURRENTLY HARDCODED
	// VALUES
	Sqlite3Driver::Sqlite3Driver(
		const std::string_view & filename,
		const json & dbSchema
	): db(std::string(filename)) {
		this->dbSchema = json({});
	};

	json Sqlite3Driver::getUser( const std::string_view & username ) const {
		json result;
		database db = database(this->db);
		auto stmt = db << "SELECT username, canCreateUsers, active FROM Admin WHERE username=?;";
		stmt << std::string(username);
		stmt >> [&](
			std::string username,
			bool canCreateUsers,
			bool active
		) {
			result = {
				{"username", username},
				{"canCreateUsers", canCreateUsers},
				{"active", active},
				{"passwordHash", ""}
			};
		};
		return result;
	};

	json Sqlite3Driver::getSession( const std::string_view & authToken ) const {
		json result;
		database db = database(this->db);
		auto stmt = db << "SELECT auth_token, creazione, scadenza, utente FROM Sessioni WHERE auth_token=?;";
		stmt << std::string(authToken);
		stmt >> [&](
			std::string auth_token,
			std::string created,
			std::string expires,
			std::string username
		) {
			result = {
				{"auth_token", auth_token},
				{"created", created},
				{"expires", expires},
				{"username", username}
			};
		};
		return result;
	};

	json Sqlite3Driver::getDegree( const std::string_view & id ) const {
		json result;
		database db = database(this->db);
		auto stmt = db << "SELECT id, nomeCorso, storico FROM CorsiDiLaurea WHERE id=?;";
		stmt << std::string(id);
		stmt >> [&](
			int id,
			std::string course,
			bool historical
		) {
			result = {
				{"id", id},
				{"course", course},
				{"historical", historical}
			};
		};
		return result;
	};

	json Sqlite3Driver::getRegistrationSeason( const Year & year ) const {
		json result;
		database db = database(this->db);
		auto stmt = db << "SELECT anno, apertura, chiusura FROM Tesseramenti WHERE anno=?;";
		stmt << year;
		stmt >> [&](
			int year,
			std::string opened,
			std::string closed
		) {
			result = {
				{"year", year},
				{"opened", opened},
				{"closed", closed}
			};
		};
		return result;
	};

	json Sqlite3Driver::getMembership(
			const Year         & year              ,
			const MembershipId & id                ,
			const bool         & makeBrief
	) const {
		json result;
		database db = database(this->db);

		auto stmt = db << "SELECT numeroTessera, tesseramento, corsoDiLaurea, nome, cognome, matricola, email, cellulare, professione, cestinato, blacklist FROM Soci WHERE tesseramento=? AND numeroTessera=?;";
		stmt << year;
		stmt << id;
		stmt >> [&](
			int membershipNumber,
			int year,
			std::string degreeId,
			std::string name,
			std::string surname,
			std::string uniNumber,
			std::string mail,
			std::string phone,
			std::string profession,
			bool deleted,
			bool blacklist
		) {
			result = {
				{"membershipNumber", membershipNumber},
				{"year", year},
				{"name", name},
				{"surname", surname},
				{"uniNumber", uniNumber},
				{"mail", mail},
				{"deleted", deleted}
			};
			if (!makeBrief) {
				result["phone"] = phone;
				result["profession"] = profession;
				result["blacklist"] = blacklist;
			}
		};

		return result;
	};

	json Sqlite3Driver::getUsers() const {
		database db = database(this->db);
		json result = json::array();

		auto stmt = db << "SELECT username, canCreateUsers, active FROM Admin;";
		stmt >> [&](
			std::string username,
			bool canCreateUsers,
			bool active
		){
			result.push_back({
				{"username", username},
				{"canCreateUsers", canCreateUsers},
				{"active", active},
				{"passwordHash", ""}
			});
		};
		return result;
	};

	json Sqlite3Driver::getSessions(
		const std::optional < std::string_view > & username
	) const {
		database db = database(this->db);
		json result = json::array();

		auto lambda = [&](
			std::string auth_token,
			std::string created,
			std::string expires,
			std::string username
		){
			result.push_back({
				{"auth_token", auth_token},
				{"created", created},
				{"expires", expires},
				{"username", username}
			});
		};

		if (username){
			auto stmt = db << "SELECT auth_token, creazione, scadenza, utente FROM Sessioni WHERE username=?;";
			stmt << std::string(username.value());
			stmt >> lambda;
		} else {
			auto stmt = db << "SELECT auth_token, creazione, scadenza, utente FROM Sessioni;";
			stmt >> lambda;
		}
		return result;
	};

	json Sqlite3Driver::getLog(
		const std::optional < std::string_view > & username ,
		const std::optional < Date             > & startDate ,
		const std::optional < Date             > & endDate
	) const {
		/*
			TODO Logging
		*/
		return json::array();
	};

	json Sqlite3Driver::getDegrees( const bool & includeHistorical ) const {
		json result = json::array();
		database db = database(this->db);
		std::string query;
		if (includeHistorical){
			query = "SELECT id, nomeCorso, storico FROM CorsiDiLaurea;";
		} else {
			query = "SELECT id, nomeCorso, storico FROM CorsiDiLaurea WHERE storico=0;";
		}
		auto stmt = db << query;
		stmt >> [&](
			int id,
			std::string course,
			bool historical
		) {
			result.push_back({
				{"id", id},
				{"course", course},
				{"historical", historical}
			});
		};
		return result;
	};

	json Sqlite3Driver::getRegistrationSeasons() const {
		json result = json::array();
		database db = database(this->db);
		auto stmt = db << "SELECT anno, apertura, chiusura FROM Tesseramenti;";
		stmt >> [&](
			int year,
			std::string opened,
			std::string closed
		) {
			result.push_back({
				{"year", year},
				{"opened", opened},
				{"closed", closed}
			});
		};
		return result;
	};

	json Sqlite3Driver::getMemberships(
			const std::optional < Year             > & year       ,
			const std::optional < MembershipId     > & id         ,
			const std::optional < std::string_view > & name       ,
			const std::optional < std::string_view > & surname    ,
			const std::optional < std::string_view > & degreeId   ,
			const std::optional < std::string_view > & profession ,
			const std::optional < std::string_view > & mail       ,
			const bool & makeBrief
	) const {
		database db = database(this->db);
		json result = json::array();
		std::vector<std::pair<int, int>> keys;

		std::stringstream queryStream;

		struct QueryCheckStrategy {
			bool firstQuery = true;

			std::string get(){
				if (this->firstQuery){
					return " WHERE ";
					this->firstQuery = false;
				} else {
					return " AND ";
				}
			}
		};

		QueryCheckStrategy q = QueryCheckStrategy();

		queryStream << "SELECT numeroTessera, tesseramento FROM Soci";
		if (year){
			queryStream << q.get() << "tesseramento=?";
		}
		if (id) {
			queryStream << q.get() << "numeroTessera=?";
		}
		if (name){
			queryStream << q.get() << "nome=?";
		}
		if (surname) {
			queryStream << q.get() << "cognome=?";
		}
		if (profession) {
			queryStream << q.get() << "professione=?";
		}
		if (mail){
			queryStream << q.get() << "mail=?";
		}
		if (degreeId){
			queryStream << q.get() << "corsoDiLaurea=?";
		}
		queryStream << ";";

		auto stmt_socio = db << queryStream.str();
		if (year) stmt_socio << year;
		if (id) stmt_socio << id;
		if (name) stmt_socio << std::string(name.value());
		if (surname) stmt_socio << std::string(surname.value());
		if (profession) stmt_socio << std::string(profession.value());
		if (mail) stmt_socio << std::string(mail.value());
		if (degreeId) stmt_socio << std::string(degreeId.value());

		stmt_socio >> [&](int id, int year) {
			keys.push_back({id, year});
		};

		for (std::pair<int, int> key: keys) {
			result.push_back(
				getMembership(key.second, key.first, makeBrief)
			);
		}

		return result;
	};

	json Sqlite3Driver::createMember(
		const Year & year,
		const MembershipId & id,
		const std::string_view & uniNumber,
		const std::string_view & name,
		const std::string_view & surname,
		const std::string_view mail,
		const std::optional < std::string_view > & degreeId,
		const std::optional < std::string_view > & profession,
		const std::optional < std::string_view > & phone
	) const {
		// Check
		if (!getMembership(year, id).empty()) {
			return json({{"error", "duplicate"}});
		}

		database db = database(this->db);

		db << "INSERT INTO Soci(numeroTessera, tesseramento, corsoDiLaurea, nome, cognome, matricola, professione, email, cellulare) values(?,?,?,?,?,?,?,?,?);"
			 << id
			 << year
			 << (degreeId?std::string(degreeId.value()):nullptr)
			 << std::string(name)
			 << std::string(surname)
			 << std::string(uniNumber)
			 << (profession?std::string(profession.value()):nullptr)
			 << std::string(mail)
			 << (phone?std::string(phone.value()):nullptr);

		return json({"ok", "ok"});
	};

	json Sqlite3Driver::createRegistration(const Year & year, const std::string_view & opening) const {
		if (!getRegistrationSeason(year).empty()) {
			return json({{"error", "duplicate"}});
		}

		database db = database(this->db);

		db << "INSERT INTO Tesseramenti(anno, apertura) values(?, ?);"
			 << year
			 << std::string(opening);

		return json({"ok", "ok"});
	};

	json Sqlite3Driver::createAdmin(const std::string_view & username, const std::string_view & password) const {
		if (!getUser(username).empty()) {
			return json({{"error", "duplicate"}});
		}

		database db = database(this->db);

		db << "INSERT INTO Admin(username, passwordHash) values(?, ?);"
			 << std::string(username)
			 << std::string(password);

		return json({"ok", "ok"});
	};

	json Sqlite3Driver::createDegree(const std::string_view & id, const std::string_view & degree) const  {
		if (!getDegree(id).empty()) {
			return json({{"error", "duplicate"}});
		}

		database db = database(this->db);

		db << "INSERT INTO CorsiDiLaurea(id, nomeCorso) values(?, ?);"
			 << std::string(id)
			 << std::string(degree);

		return json({"ok", "ok"});
	};

	json Sqlite3Driver::createSession(const std::string_view & username) const {
		// TODO Session Management in the DB
		return json({});
	};

	json Sqlite3Driver::closeRegistration(const Year & year, const std::string_view & closing) const {
		if (getRegistrationSeason(year).empty()){
			return json({{"error", "no such registration season"}});
		}

		database db = database(this->db);

		db << "UPDATE Tesseramenti SET chiusura=? WHERE anno=?;"
			 << std::string(closing)
			 << year;

		return json({"ok", "ok"});
	};

	json Sqlite3Driver::deleteMember(const Year & year, const MembershipId & membershipId) const {
		database db = database(this->db);

		db << "DELETE FROM Soci WHERE tesseramento=? AND numeroTessera=?;"
			 << year
			 << membershipId;

		return json({"ok", "ok"});

	};

};
