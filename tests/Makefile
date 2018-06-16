server:
	g++ -o server_test.exe \
		-std=c++11 \
		-I../deps/asio/asio/include \
		-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO \
		server_test.cpp \
		-lpthread -ldl

client:
	python client_test.py

