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
		make -j$(shell nproc)

release.no-e1:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBILLING_WITHOUT_E1=ON .. && \
		make -j$(shell nproc)

release.no-e1-anti_clone_ip-anti_clone_hw:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBILLING_WITHOUT_E1=ON \
		-DBILLING_WITHOUT_ANTI_CLONE_IP=ON \
		-DBILLING_WITHOUT_ANTI_CLONE_HW=ON \
		.. && \
		make -j$(shell nproc)

clean:
	git clean -xdf --exclude=build/cmake_modules bin/Billing build

update:
	git pull
	git submodule update --init --recursive

test.client:
	cd tests && make client

sys:
	cd deps/ee/tests && python system.py

