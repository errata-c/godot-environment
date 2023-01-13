#pragma once
#include <array>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>
#include <Array.hpp>

#include <gdev/Value.hpp>

namespace godot {
	class ValueWrapper;

	/*
	Class that wraps a gdev::Value tensor for use in godot more directly.
	*/
	class ValueIndexer: public Reference {
		GODOT_CLASS(ValueIndexer, Reference)
	public:
		static void _register_methods();

		void _init();

		bool _iter_init();
		bool _iter_next();
		Variant _iter_get();

		ValueIndexer* index(Variant i0, Variant i1, Variant i2, Variant i3);
		ValueIndexer* index_flat(Variant idx);

		ValueIndexer* slice(Variant idx);
		ValueIndexer* row(Variant idx);
		ValueIndexer* col(Variant idx);

		void set(size_t i0, size_t i1, size_t i2, size_t i3, Variant var);
		void set_flat(size_t i, Variant var);

		Variant get(size_t i0, size_t i1, size_t i2, size_t i3);
		Variant get_flat(size_t i);

		String vtype() const noexcept;

		size_t elements() const;
		Array dims() const;
	private:
		Ref<ValueWrapper> source;

		size_t current = 0, end = 0;

		gdev::Dims cdims;

		bool flat = false;
		std::array<Array, 4> indices;
	};
}