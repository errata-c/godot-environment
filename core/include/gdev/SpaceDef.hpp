#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <gdev/ValueDef.hpp>

namespace gdev {
	class Space;

	// A dictionary containing the possibilities for actions or observations
	class SpaceDef {
	public:
		struct value_type {
			std::string name;
			ValueDef value;
		};

		using map_t = std::unordered_map<std::size_t, std::size_t>;
		using container_t = std::vector<value_type>;
		using iterator = typename container_t::iterator;
		using const_iterator = typename container_t::const_iterator;

		SpaceDef() = default;
		SpaceDef(const SpaceDef&) = default;
		SpaceDef(SpaceDef&&) noexcept = default;
		SpaceDef& operator=(const SpaceDef&) = default;
		SpaceDef& operator=(SpaceDef&&) noexcept = default;

		Space instance() const;

		bool matches(const Space & space) const;

		void reserve(std::size_t count);

		std::size_t size() const noexcept;
		bool empty() const noexcept;

		bool contains(std::string_view name) const;

		void clear();

		std::optional<iterator> insert(std::string_view name, ValueDef && def);
		std::optional<iterator> insert(std::string_view name, const ValueDef& def);

		std::pair<iterator, bool> insert_or_assign(std::string_view name, ValueDef&& obj);

		iterator find(std::string_view name);
		const_iterator find(std::string_view name) const;

		ValueDef & operator[](std::string_view name);

		iterator erase(iterator pos);
		iterator erase(const_iterator pos);
		bool erase(std::string_view name);

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;
	private:
		map_t map;
		container_t data;
	};
}