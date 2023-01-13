#include <gdev/ValueIndexer.hpp>
#include <gdev/ValueWrapper.hpp>
#include <gdev/GodotConversions.hpp>

namespace godot {
	void ValueIndexer::_register_methods() {
		
	}
	void ValueIndexer::_init() {
		
	}

	bool ValueIndexer::_iter_init() {
		current = 0;
		end = elements();
		return current < end;
	}
	bool ValueIndexer::_iter_next() {
		++current;
		return current < end;
	}
	Variant ValueIndexer::_iter_get() {
		return {};
	}

	ValueIndexer* ValueIndexer::index(Variant i0, Variant i1, Variant i2, Variant i3) {
		return nullptr;
	}

	ValueIndexer* ValueIndexer::slice(Variant idx) {
		return nullptr;
	}
	ValueIndexer* ValueIndexer::row(Variant idx) {
		return nullptr;
	}
	ValueIndexer* ValueIndexer::col(Variant idx) {
		return nullptr;
	}

	void ValueIndexer::set(size_t i0, size_t i1, size_t i2, size_t i3, Variant var) {
		
	}
	void ValueIndexer::set_flat(size_t i, Variant var) {
		
	}

	Variant ValueIndexer::get(size_t i0, size_t i1, size_t i2, size_t i3) {
		if (source.is_null()) {
			ERR_PRINT("ValueIndexer source was null!");
			return {};
		}
		//return source->get(indices[0][i0])
		return {};
	}
	Variant ValueIndexer::get_flat(size_t i) {
		//return gdev::visitors::single_visit<VariantCast>(data.type(), data[i]);
		return {};
	}

	String ValueIndexer::vtype() const noexcept {
		if (source.is_null()) {
			ERR_PRINT("ValueIndexer source was null!");
			return "";
		}
		return source->vtype();
	}
	size_t ValueIndexer::elements() const {
		return cdims.elements();
	}
	Array ValueIndexer::dims() const {
		return gdev::to_variant(cdims);
	}
}