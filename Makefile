main: debug
	cd bin && \
		./Billing

debug:
	mkdir -p bin
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Debug .. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

release:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release .. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

clean:
	git clean -xdf --exclude=build/cmake_modules bin/Billing build

update:
	git pull
	git submodule update --init --recursive

test.client:
	cd tests && make client

