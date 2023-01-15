#pragma once
#include <Godot.hpp>
#include <Reference.hpp>
#include <Object.hpp>
#include <Variant.hpp>
#include <Array.hpp>
#include <Dictionary.hpp>

#include <gdev/MessageContext.hpp>

namespace godot {
	class TestAgent : public Reference {
		GODOT_CLASS(TestAgent, Reference)
	public:
		static void _register_methods();

		void _init();

		TestAgent();
		
		bool connect_environment(int port);
		Dictionary reset(Variant init_dict);
		Dictionary step(Dictionary action);

		gdev::MessageContext mcontext;
		std::string mbuffer;
	};
}