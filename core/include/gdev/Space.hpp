#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>
#include <gdev/Value.hpp>

namespace gdev {
	// A dictionary containing the possibilities for actions or observations
	class Space {
	public:
		struct value_type {
			std::string name;
			Value value;
		};

		using map_t = std::unordered_map<std::size_t, std::size_t>;
		using container_t = std::vector<value_type>;
		using iterator = typename container_t::iterator;
		using const_iterator = typename container_t::const_iterator;

		Space() = default;
		Space(const Space&) = default;
		Space(Space&&) noexcept = default;
		Space& operator=(const Space&) = default;
		Space& operator=(Space&&) noexcept = default;

		void reserve(std::size_t count);

		std::size_t size() const noexcept;
		bool empty() const noexcept;

		bool contains(std::string_view name) const;

		void clear();

		std::optional<iterator> insert(std::string_view name, Value&& def);
		std::optional<iterator> insert(std::string_view name, const Value& def);

		std::pair<iterator, bool> insert_or_assign(std::string_view name, Value&& obj);

		iterator find(std::string_view name);
		const_iterator find(std::string_view name) const;

		Value& operator[](std::string_view name);

		iterator erase(iterator pos);
		iterator erase(const_iterator pos);
		bool erase(std::string_view name);

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		bool operator==(const Space& other) const noexcept;
		bool operator!=(const Space& other) const noexcept;
	private:
		map_t map;
		container_t data;
	};
}