#include <httplib.h>
#include "database/interface.hpp"

namespace AssociateManager {

		using Server = httplib::Server ;

		void setupServer( Server & srv, DbInterface & db ) ;
	}
