#pragma once
#include <Godot.hpp>
#include <Reference.hpp>
#include <Object.hpp>
#include <Variant.hpp>
#include <Array.hpp>

#include <gdev/Value.hpp>

namespace godot {
	class ValueIndexer;
	
	/*
	Class that wraps a gdev::Value tensor for use in godot more directly.
	*/
	class ValueWrapper: public Reference {
		GODOT_CLASS(ValueWrapper, Reference)
	public:
		static void _register_methods();

		void _init();

		bool _iter_init();
		bool _iter_next();
		Variant _iter_get();

		void set(size_t i0, size_t i1, size_t i2, size_t i3, Variant var);
		void set_flat(size_t i, Variant var);

		Variant get(size_t i0, size_t i1, size_t i2, size_t i3);
		Variant get_flat(size_t i);

		void clear();
		void reset_uninitialized(Variant _dims, String _vtype);
		void reset(Variant _dims, String _vtype, Variant val);

		void assign(Variant val);
		void fill(Variant val);

		ValueIndexer* index_flat(Variant i0);
		ValueIndexer* index(Variant i0, Variant i1, Variant i2, Variant i3);

		ValueIndexer* slice(Variant idx);
		ValueIndexer* row(Variant idx);
		ValueIndexer* col(Variant idx);

		Array dims() const;
		size_t elements() const noexcept;

		String vtype() const;

		String _to_string() const;
	private:
		friend class ValueIndexer;

		// Iteration information
		size_t current, end;

		// Actual value
		gdev::Value data;
	};
}