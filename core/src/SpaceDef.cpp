#include <gdev/SpaceDef.hpp>
#include <gdev/Space.hpp>
#include <utility>

namespace gdev {
	using iterator = SpaceDef::iterator;
	using const_iterator = SpaceDef::const_iterator;

	void SpaceDef::reserve(std::size_t count) {
		map.reserve(count);
		data.reserve(count);
	}

	std::size_t SpaceDef::size() const noexcept {
		return data.size();
	}
	bool SpaceDef::empty() const noexcept {
		return data.empty();
	}

	bool SpaceDef::contains(std::string_view name) const {
		return map.find(std::hash<std::string_view>{}(name)) != map.end();
	}

	void SpaceDef::clear() {
		map.clear();
		data.clear();
	}

	std::optional<iterator> SpaceDef::insert(std::string_view name, ValueDef&& def) {
		if (contains(name)) {
			return {};
		}
		else {
			data.push_back(value_type{std::string(name), std::move(def)});
			value_type & val = data.back();
			map.insert(map_t::value_type{ std::hash<std::string_view>{}(name), data.size() - 1 });
			return data.end()-1;
		}
	}
	std::optional<iterator> SpaceDef::insert(std::string_view name, const ValueDef& def) {
		if (contains(name)) {
			return {};
		}
		else {
			data.push_back(value_type{ std::string(name), def });
			value_type& val = data.back();
			map.insert(map_t::value_type{ std::hash<std::string_view>{}(name), data.size() - 1 });
			return data.end() - 1;
		}
	}

	std::pair<iterator, bool> SpaceDef::insert_or_assign(std::string_view name, ValueDef&& obj) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit == map.end()) {
			data.push_back(value_type{std::string(name), obj});
			map.insert(mit, map_t::value_type{ std::hash<std::string_view>{}(name), data.size()-1});
			return {data.end() -1, true};
		}
		else {
			iterator it = data.begin() + mit->second;
			it->value = std::move(obj);
			return {it, false};
		}
	}

	iterator SpaceDef::find(std::string_view name) {
		map_t::iterator iter = map.find(std::hash<std::string_view>{}(name));
		if (iter != map.end()) {
			return data.begin() + iter->second;
		}
		else {
			return data.end();
		}
	}
	const_iterator SpaceDef::find(std::string_view name) const {
		map_t::const_iterator iter = map.find(std::hash<std::string_view>{}(name));
		if (iter != map.end()) {
			return data.begin() + iter->second;
		}
		else {
			return data.end();
		}
	}

	ValueDef& SpaceDef::operator[](std::string_view name) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit != map.end()) {
			return data[mit->second].value;
		}
		else {
			auto opt = insert(name, ValueDef{});
			return opt.value()->value;
		}
	}

	iterator SpaceDef::erase(iterator pos) {
		return erase(const_iterator(pos));
	}
	iterator SpaceDef::erase(const_iterator pos) {
		std::string_view name = pos->name;
		auto mit = map.find(std::hash<std::string_view>{}(name));
		std::ptrdiff_t index = mit->second;

		std::swap(data[index], data.back());
		data.pop_back();

		map.erase(mit);

		// fix indices in the map now.
		if (index < data.size()) {
			auto & element = data[index];
			map[std::hash<std::string_view>{}(name)] = index;
		}
		return data.begin() + index;
	}
	bool SpaceDef::erase(std::string_view name) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit == map.end()) {
			return false;
		}
		else {
			erase(data.begin() + mit->second);
			return true;
		}
	}

	iterator SpaceDef::begin() {
		return data.begin();
	}
	iterator SpaceDef::end() {
		return data.end();
	}

	const_iterator SpaceDef::begin() const {
		return data.begin();
	}
	const_iterator SpaceDef::end() const {
		return data.end();
	}

	Space SpaceDef::instance() const {
		Space ret;
		ret.reserve(size());

		for (const value_type & element : *this) {
			ret.insert(element.name, element.value.instance());
		}

		return ret;
	}

	bool SpaceDef::matches(const Space& space) const {
		for (const value_type & element : *this) {
			auto found = space.find(element.name);
			if (found == space.end()) {
				return false;
			}

			if (!element.value.contains(found->value)) {
				return false;
			}
		}

		return true;
	}

	bool SpaceDef::operator==(const SpaceDef& other) const noexcept {
		if (size() != other.size()) {
			return false;
		}
		for (const auto & elem : *this) {
			auto it = other.find(elem.name);
			if (it == other.end()) {
				return false;
			}
			else if(it->value != elem.value) {
				return false;
			}
		}
		return true;
	}
	bool SpaceDef::operator!=(const SpaceDef& other) const noexcept {
		if (size() != other.size()) {
			return true;
		}
		for (const auto& elem : *this) {
			auto it = other.find(elem.name);
			if (it == other.end()) {
				return true;
			}
			else if (it->value != elem.value) {
				return true;
			}
		}
		return false;
	}
}