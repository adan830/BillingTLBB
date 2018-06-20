server:
	g++ -o server.exe \
		-std=c++11 \
		-I../deps/asio/asio/include \
		-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO \
		server.cpp \
		-lpthread -ldl

server_bug:
	g++ -o server.exe \
		-std=c++11 \
		-I../deps/asio/asio/include \
		-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO \
		server_bug.cpp \
		-O2 \
		-lpthread -ldl

client:
	python client.py

database:
	g++ -o database.exe \
		-std=c++11 \
		-I../include \
		-I../deps/spdlog/include \
		-I../deps/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/include \
		-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO \
		database.cpp \
		../src/database/models/Account.cpp \
		../src/database/Model.cpp \
		../src/database/Connector.cpp \
		../src/Log.cpp \
		../src/Config.cpp \
		-lpthread -ldl \
		-L../deps/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/lib/libmysqlclient.a

