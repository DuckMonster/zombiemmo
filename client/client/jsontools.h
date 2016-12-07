#pragma once
#include <json.hpp>
using json = nlohmann::json;

namespace jsontools {
	template<typename T>
	json::iterator FindKeyValue( json::iterator first, json::iterator end, std::string key, T value ) {
		for (json::iterator i = first; i != end; i++) {
			json& obj = i.value( );
			json::iterator keyObj = obj.find( key );

			if (keyObj != obj.end( )) {
				if (keyObj->get<T>( ) == value)
					return i;
			}
		}

		return end;
	}
}