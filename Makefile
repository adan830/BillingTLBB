main: debug
	cd bin && \
		./Billing

debug:
	mkdir -p bin
	cd build && \
		cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBILLING_EE=ON \
		.. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

release:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake \
		-DBILLING_STAND_ALONE=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-DBILLING_EE=ON \
		.. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

release.standalone:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBILLING_STAND_ALONE=ON \
		-DBILLING_EE=ON \
		.. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

release.no-e1:
	mkdir -p bin
	git clean -xdf bin build
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release \
		-DBILLING_EE=ON \
		-DBILLING_WITHOUT_E1=ON \
		.. && \
		make -j$(shell nproc) compile_commands ; \
		make -j$(shell nproc)

clean:
	git clean -xdf --exclude=build/cmake_modules bin/Billing build

update:
	git pull
	git submodule update --init --recursive
	git submodule foreach git checkout master
	git submodule foreach git pull

test.client:
	cd tests && make client

sys:
	cd deps/ee/tests && python system.py

