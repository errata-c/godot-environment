#include <catch2/catch_all.hpp>

#include <thread>

#include <gdev/Value.hpp>
#include <gdev/Space.hpp>

#include <gdev/MessageContext.hpp>

using namespace gdev;

TEST_CASE("IPC connection", "[ipc]") {
	bool r0 = false, r1 = false;

	std::thread th0([&](){
		MessageContext from(Com::Agent);
		r0 = from.connect(1024);

		std::this_thread::sleep_for(std::chrono::milliseconds(120));
	});
	std::thread th1([&]() {
		MessageContext to(Com::Environment);
		r1 = to.connect(1024);

		std::this_thread::sleep_for(std::chrono::milliseconds(120));
	});

	th0.join();
	th1.join();
	
	REQUIRE(r0);
	REQUIRE(r1);
}

TEST_CASE("IPC stepping", "[ipc]") {
	std::string req("request"), rep("reply");

	std::string v0, v1;

	bool sent_req = false;
	bool recv_req = false;

	bool sent_rep = false;
	bool recv_rep = false;

	std::thread th0([&]() {
		MessageContext from(Com::Agent);
		from.connect(1024);

		std::this_thread::sleep_for(std::chrono::milliseconds(120));

		sent_req = from.send(req);
		recv_rep = from.recv(v0);
	});
	std::thread th1([&]() {
		MessageContext to(Com::Environment);
		to.connect(1024);

		std::this_thread::sleep_for(std::chrono::milliseconds(120));

		recv_req = to.recv(v1);
		sent_rep = to.send(rep);
	});

	th0.join();
	th1.join();

	REQUIRE(sent_req);
	REQUIRE(recv_rep);

	REQUIRE(recv_req);
	REQUIRE(sent_rep);

	REQUIRE(v0 == rep);
	REQUIRE(v1 == req);
}