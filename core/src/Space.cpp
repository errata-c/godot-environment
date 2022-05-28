#include <gdev/Space.hpp>
#include <utility>
#include <stdexcept>
#include <fmt/format.h>

namespace gdev {
	using iterator = Space::iterator;
	using const_iterator = Space::const_iterator;

	void Space::reserve(std::size_t count) {
		map.reserve(count);
		data.reserve(count);
	}

	std::size_t Space::size() const noexcept {
		return data.size();
	}
	bool Space::empty() const noexcept {
		return data.empty();
	}

	bool Space::contains(std::string_view name) const {
		return map.find(std::hash<std::string_view>{}(name)) != map.end();
	}

	void Space::clear() {
		map.clear();
		data.clear();
	}


	std::optional<iterator> Space::insert(std::string_view name, Value&& def) {
		if (contains(name)) {
			return {};
		}
		else {
			data.push_back(value_type{std::string(name), std::move(def)});
			value_type & val = data.back();
			map.insert(map_t::value_type{ std::hash<std::string_view>{}(name), data.size()-1});
			return data.end()-1;
		}
	}
	std::optional<iterator> Space::insert(std::string_view name, const Value& def) {
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

	std::pair<iterator, bool> Space::insert_or_assign(std::string_view name, Value&& obj) {
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
	iterator Space::find(std::string_view name) {
		map_t::iterator iter = map.find(std::hash<std::string_view>{}(name));
		if (iter != map.end()) {
			return data.begin() + iter->second;
		}
		else {
			return data.end();
		}
	}
	const_iterator Space::find(std::string_view name) const {
		map_t::const_iterator iter = map.find(std::hash<std::string_view>{}(name));
		if (iter != map.end()) {
			return data.begin() + iter->second;
		}
		else {
			return data.end();
		}
	}

	Value& Space::operator[](std::string_view name) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit != map.end()) {
			return data[mit->second].value;
		}
		else {
			auto opt = insert(name, Value{});
			return opt.value()->value;
		}
	}

	Value& Space::at(std::string_view name) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit != map.end()) {
			return data[mit->second].value;
		}
		else {
			throw std::out_of_range(
				fmt::format("No value in gdev::Space with name '{}'!", name)
			);
		}
	}
	const Value& Space::at(std::string_view name) const {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit != map.end()) {
			return data[mit->second].value;
		}
		else {
			throw std::out_of_range(
				fmt::format("No value in gdev::Space with name '{}'!", name)
			);
		}
	}

	iterator Space::erase(iterator pos) {
		return erase(const_iterator(pos));
	}
	iterator Space::erase(const_iterator pos) {
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
	bool Space::erase(std::string_view name) {
		auto mit = map.find(std::hash<std::string_view>{}(name));
		if (mit == map.end()) {
			return false;
		}
		else {
			erase(data.begin() + mit->second);
			return true;
		}
	}

	iterator Space::begin() {
		return data.begin();
	}
	iterator Space::end() {
		return data.end();
	}

	const_iterator Space::begin() const {
		return data.begin();
	}
	const_iterator Space::end() const {
		return data.end();
	}

	bool Space::operator==(const Space& other) const noexcept {
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
	bool Space::operator!=(const Space& other) const noexcept {
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